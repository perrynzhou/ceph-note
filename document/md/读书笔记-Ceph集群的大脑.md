## 读书笔记-Ceph集群的大脑

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2021/05/13 |672152841 |

##### Ceph集群的CRUSH Map是谁负责维护？

- Ceph集群中Crush Map是由Monitor负责维护和进行传播的
- Monitor是基于Paxos算法构建，具有强一致性的小型集群，主要负责维护和传播集群表（集群元数据)的权威副本。Ceph集群中的任何时刻，任何的客户端或者OSD都可以和集群中任意一个Monitor进行通信来更新或者获取最新的集群元数据。



##### Monitor有哪些类型？都负责了什么？

- Monitor仅仅提供一个基于Paxos的实现分布式一致性的一般性框架，实际上Ceph存在各种不同类型、需要依赖于Monitor进行集中式的管理的数据。
- Ceph中具体有如下几类Monitor
  - AuthMonitor,负责鉴权和授权，后者又负责了秘钥的分发和定时刷新等工作
  - HealthMonitor,负责监控Monitor的自身状态，产生相关警告，包括Monitor之间时钟是否同步、是否有Monitor是否有宕机
  - LogMonitor,负责按照管路员配置日志策略采集系统日志，并转发至指定的日志频道或者日志服务器
  - MDSMonitor,负责监测元数据服务器(MDS)的状态变化,处理文件系统相关的命令
  - OSDMonitor,负责监控OSD的状态维护集群表
  - PGMonitor,负责收集PG相关统计，处理PG相关命令等。
- Monitor负责为Rados集群提供包括鉴权、授权、设备(osd)管理、日志管理、告警管理在内的集群管理服务，同时通过与集群中所有OSD建立联系并周期的交换和传播OSD状态信息，Monitor能欧提供可信赖和可扩展的集群状态服务。同时Monitor提供丰富的CLI命令，通过这些CLI命令可以结合集群自身的特征对集群进行深度定制，从而最大化资源利用率，节省成本。

##### Ceph集群表包含了哪些信息？

- Ceph集群表主要是又两部分组成，一是集群拓扑结构和用于计算寻址的CRUSH规则，由于两者结合的比较紧密，所以在具体实现上抽象为一张独立的CRUSH Map同意进行管理；二是所有OSD的身份和状态信息。Crush Map围绕OSD构建，比如OSD是集群拓扑结构中基本元素、CRUSH 的输出是一组OSD的集合，因此集群表业成为OSDMap


##### 如何修改OSDMap?

- 如果想修改OSDMap，只能通过Monitor,比如针对集群执行一些管理操作，例如调整集群的拓扑结构、创建或者删除存储池、OSD的管理等。最方便的就是直接通过Monitor提供的CLI命令来完成。



##### Monitor负责了哪些告警？

- OSD状态告警和PG状态告警分别是有OSDMonitor和PGMonitor负责监控和上报的

- OSD状态告警
  - OSD宕机
  - OSD设置了某些特殊标记，例如NOUP/NODOWN/NOIN/NOOUT等
  - OSD的空间使用率超过了某些阈值，同时会导致对应存储池也产生相关告警
  - OSD出现Slow Request等
- PG状态告警
  - PG降级
  - PG宕机
  - PG出现数据损坏
