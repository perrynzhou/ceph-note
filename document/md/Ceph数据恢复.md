### Ceph数据恢复

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2020/12/01 |672152841 |


#### Ceph数据恢复方式和差异点有哪些？

- ceph数据恢复有Recovery和Backfill.
- Recovery的恢复方式是针对副本能够PG日志进行恢复,只需要修复该副本上与权威日志不同步的那部分对象。
- Backfill的恢复方式是针对副本已经无法通过PG 日志进行恢复(比如OSD离线太久，大量客户端写或者是Ceph要做自身的均衡需要执行)然后以PG作为整体为目标的数据秦迁移过程。这里提到如果Ceph集群PG太少，涉及到的迁移数量会非常大，因此需要设置比较合理的PG数量
- Recovery可以理解为PG内部分对象的修复；Backfill可以理解为PG的全部对象恢复；所以Recovery的恢复时间要比Backfill时间要长。


#### 控制Recovery恢复的速度影响参数有哪些？
- osd_max_backfills
	- 单个OSD允许执行Recovery和Backfill的PG个数，这里包含了Primary和副本。同一个PG的Recovery和Backfill不能同时并发；但是不同PG的,Recovery和Backfill可以并发执行，一个PG执行Backfill,另外一个PG执行Recovery.
- osd_max_push_cost 和 osd_max_push_objects
	- Primary 对象数据修复副本数据时候单个请求能够携带的字节数和对象数

- osd_revocery_max_active
	- 每个OSD允许并发进行的Recovery/Backfill的对象数 	

- osd_revocery_op_priority
	- push/pull方式修复操作的优先级,这类OP操作进入op_shardedwq处理,会和客户端的OP产生竞争。这个值设置的越小，恢复速度越大，会影响客户端的OP

- osd_recovery_sleep_hdd
	- 针对HDD盘每次Recovery 或者 Backfill 的操作间的休眠时间，单位是ms

#### 如何进行对象修复?
- 对象修复是通过Pull或者Push来进行的，Pull是指Primary(主的pg)存在降级对象，由Primary按照missing_loc选择合适的副本拉取降级对象的权威版本到本地，然后完成修复。Push是指Primary感知到一个或者多个副本当前存在降级，然后Primary主动推送每个降级对象的权威版本对应的副本，再由副本本地完成修复的方式
- 为了修复副本，Primary必须完成自身的修复，通常先执行Pull操作，在执行Push操作。如果用户修改某个某个数据对象，在Primary上完成后，然后发现副本中这个对象处于降级状态，PG会强制通过Push方式修复该对象，以避免长时间阻塞客户端请求。


#### Recovery修复的方式有哪几种?

- Copy DataOBJECT
	- 除了DELETE和LOST_DELETE操作外，一般都会采用对象拷贝的方式进行修复。
	- 这种方式会完整从源端拷贝数据、扩展属性、omap在内的所有信息到目标端。这样拷贝包含数据量比较大，很难一次性交互完成修复，因此会在目标端见新建一个临时对象，等到对象完全拷贝完成后，在删除本地同名目标对象，然后重命名临时对象为目标对象。

- Delete DataObject
	- 这个是针对Delete和LOST_DELETE操作，采用删除对象的方式进行修复 