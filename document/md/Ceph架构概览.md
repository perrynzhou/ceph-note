## Ceph架构概览



#### Ceph架构是什么样的?

- 总体概览

![ceph-arc](../../document/images/ceph-arc.jpg)

- 架构模块

![ceph-arc2](../../document/images/ceph-arc2.JPG)

- ceph包括了底层最核心的rados,这个包含了monitor、mds、osd.
- rados之上有一层librados,提供底层服务的接口
- librados之上有三个服务，分别是cephfs、rbd、rgw
#### Ceph提供了哪几种类型的服务？

- 基于多语言的API接口
- RESTFul 接口，类似于(S3/Swift)
- 块设备接口
- 文件系统接口

#### 部署一个ceph都包含了哪些组件？各自都是干什么的？
- 部署一个Ceph集群，目前有如下进程组成
  - Ceph OSD Daemon,这是ceph用户处理客户端数据的进程，提供EC数据编码、数据均衡、数据恢复、监控等功能。目前每一个磁盘或者lvm对应一个osd进程，用于处理请求过来的数据
  - Ceph Monitor, Monitor进程维护了整个集群的各种map信息，包括osd map、mds map、pg map、 mon map等信息。整个集群采用了Paxos共识算法来保证整个集群维护数据的一致性。
  - Ceph Manager,Manager进程维护了PG的详细信息。处理来自客户端的只读的cli请求，比如pg的统计信息同时ceph也提供RESTful的监控API

#### 客户端是如何读取数据的？
- ceph客户端维护object id列表和object存储的pool信息。但是不维护object->osd的映射关系。为了读取数据，客户端需要访问monitor进程获取当前整个集群的cluster map信息。客户端提供object名称和pool名称给librados,librados计算object 所在的pg，根据crush算法在找到主的osd.客户端直接和主osd通信进行读写操作。

- osd存储数据是采用扁平的方式，每个对象有一个唯一的ID，同时包括了数据和元数据。

![ceph-object](../../document/images/ceph-object.jpg)

#### Ceph Pool有哪些？

![obj-pg](../../document/images/obj-pg.jpg)

- Pool Type,Ceph提供了2中Pool的类型，一种是三副本；另外一种是EC。Pool一旦创建就不能进行更改，数据也是基于Pool类型来进行存储
- Placement Groups,Ceph的Pool存储上亿个对象对象，执行各种各样的复杂操作，比如通过多副本或者EC进行数据持久化、数据的CRC检查、数据复制、负载均衡、恢复等。基于每个数据对象的管理的扩展性和性能会是一个瓶颈。Ceph是通过在pool中放置placement groups来解决这个问题。首先crush算法计算数据对象的placement group和placement group对应的一组osd，两层映射关系，然后通过crush算法把每个placement group存储在一组osd中。
- CRUSH Ruleset,CRUSH扮演这另外一个很重要的角色。crush可以检测osd的冗错域和性能。crush可以识别osd的存储介质和水平组织node、racks、rows等不同的域.crush算法可以把数据对象存储在不同的域，防止osd失效。比如一个数据存储在不同的nodes、racks，如果集群中有一些osd宕机，集群依然可以操作直到整个集群恢复。
- Durability,在大规模存储集群中，硬件故障是非常平常的事情。ceph提供了2种存储策略
  - Replica Pool,使用crush算法把数据对象放到跨冗错域的osd上。也就是数据对象放到不同的node或者racks下的node的磁盘上。
  - Erasure Code Pool,每个数据对象被划分为K+M的分片，K是分片是数据对象分片数，M是数据对象校验分片。

#### Ceph Placement Groups 是什么？

- Ceph采用Placement Group来存储数据对象，同时PG也是一个Pool的子集。Ceph把很多PG在分散Pool中，然后通过获取到集群的map信息和状态通过crush算法把pg伪随机分布到集群的osd中

#### Ceph Placement Groups怎么工作的？
- 当用户创建pool过程中,crush来创建这个pool中的pg数量。通常来讲PGs的数量应该是一个合理数值。例如每个osd中有100个PG，这就意味着每个pg包含了整个pool的1%的数据。
- PGs的数量会影响性能，这涉及到pg在osd之间的移动。如果出现了osd的故障或者异常情况，一个pool有少量的pg,ceph会迁移大量pg，到一个可用的osd中，同时pg中包含了大量的数据对象，这涉及的迁移数据量太多对于集群性能影响调大。如果一个pool中包含了非常多的pg,虽然迁移的数据量小了，但是会消耗更多的CPU和内存。一般会采用如下公式进行计算PG的数量

```
              (OSDs * 100)
   Total PGs =  ------------
                 pool size
```

#### Crush算法和PG在集群扩容或者osd失效情况下是什么样的行为？

- 集群扩容，当添加新机器或者新osd到集群，cluster map改变了。添加新的osd意味着crush算法伪随机把原来osd中的pg迁移到新的osd中，这个是自动的过程。
- osd宕机,当osd宕机，集群的状态改变了，采用副本或者EC。如果是主的osd宕机，从的osd立马充当主的osd,原来宕机的osd中的pg中的数据，通过crush算法迁移到其他的osd中
- 一旦ceph集群中cluster map或者cluster state改变，crush会自动计算哪一个osd改变了。比如客户端写入数据testdata到pool A,crush算法把testdata放到PG 1中，然后存储在osd5中。osd5和osd10、osd15互为副本。如果osd5宕机了，集群状态改变了，ceph客户端从pool A读取数据testdata,通过librados自动从osd10(osd5宕机，osd10成为了主osd)读取