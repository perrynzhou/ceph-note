## Ceph14或者更高版本硬件和内核选型

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2021/01/01 |672152841 |

### Ceph基本功能

- Ceph提供对象存储/块存储/文件存储的功能。一个Ceph就请你中至少包括Ceph Monitor、Ceph Manager、Ceph OSD，如果不熟了CephFS也需要一个MetaData Server组件。Ceph是以在Pool中存储数据对象的形式存储数据，首先ceph把应用端的文件先切若干个分ceph配置的标准对象大小的数据对象，然后针对这些数据对象进行哈希计算找到每个对象应该存储的PG，然后通过CRUSH算法和PG信息获取一组健康的OSD(一组OSD的数量和多副本或者纠删码相关)，最终把这些数据对象根据归属的pg，写入到这一组OSD中

  ![截屏2021-03-31 上午6.39.34](../images/ceph-chos.png)

- Monitor 

  - Monitor在ceph集群以ceph-mon进程呈现，Ceph Monitor维护集群状态的一系列map信息，包括monitor map、manager map、osd map、crush map、 mds map等信息、Monitor负责管理认证和客户端。在生产环境要保证Mon的高可用，一般建议是基数个Monitor.

- Manager

  - Manager在集群中以ceph-mgr进程呈现，它主要负责追踪集群的运行时的metric和当前集群的状态，包括存储利用率、当前性能指标、系统负载。Manager是以python-based modules来管理和暴露集群的信息，包括ceph dashboard和REST Api.在生产至少需要2个Manager来保证出现单点故障

- OSD

  - OSD在集群中以ceph-osd来呈现，它主要是存储数据、处理数据副本、恢复、均衡、上报自身状态信息给Monitor和Manager.

- MDS

  - MDS在集群中是以ceph-mds来呈现，它主要是存储cephfs的元数据(块和对象是不需要用到mds).ceph metadata server提供标准的支持Posix 语义的文件系统能开给用户

### Ceph硬件选型

#### CPU

- MDS,Ceph metadata Server是CPU密集型的服务，因此需要比较强的计算能力。
- OSD,Ceph OSD运行了RADOS服务，通过CRUSH算法来计算PG，同时需要进行数据复制、维护自身以及副本的Cluster map信息，因此OSD也比较消耗CPU资源。
- Monitor,仅仅维护master集群的Cluster Map的副本信息，因此它不是CPU Intensive的

#### 内存
- MDS
  - 元数据服务内存用多少取决于缓存了多少元数据，在生产环境强烈建议至少配置1G内存。
- OSD
  - OSD一般使用BlueStore作为后端存储，一般需要3G~5G的内存。我们可以通过osd_memory_target调整OSD中bluestore内存消耗.当使用FileStore作为后端，操作系统的PageCache已经帮忙Cache数据了，不需要额外的调优，在这种情况下OSD内存消耗多少在与PG数量相关
- Monitor和Manager
  - 这两个服务随着集群规模的增长，内存资源消耗也在增长,一般在1G~2G左右。如果是一个非常大的集群建议配置5G~10G内存。通常可以通过mon_osd_cache_size或者rocksdb_cache_size来调整。

- Data Storage
  - 通常1TB 存储需要消耗OSD的1G内存，不建议在同一个磁盘上多分分区来部署多个OSD
- Network
  - 强烈推荐主机(OSD所在机器)配置2个网卡，每个网卡至少是1Gbps。Ceph集群有2个网络，一个是public network，另外一个是cluster network.cluster network处理复制数据。在1Gbps网络中，复制1T需要3个小时；如果在10Gbps网络中，复制数据需要20分钟

###  OS 内核版本

#### Ceph 内核客户端

- 如果使用内核客户端来映射RBD或者cephfs.建议使用kernel-4.x的长期支持版本，例如4.19.x或者4.14.x内核。比较旧的内核不支持Crush tunables。