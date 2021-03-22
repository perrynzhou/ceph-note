## 关于BlueStore一些疑惑


| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2020/12/01 |672152841 |

### BlueStore 管理的设备有哪些？

- BlueStore管理三种设备分别是Primary、WAL、DB。在默认的osd配置中，Primary设备一般是hdd的磁盘(osd所在的磁盘，一般是hdd).BlueStore存储了2部分的数据，一部分是OSD Metadata，这个一般是格式化xfs,存储少量的数据，比如osd的编号、所属集群、私有的key等。另外一部分是数据，剩下的所有的空间用于存储数据
- RocksDB中的DB部分的容量建议按照大于每个osd总容量的4%来设定

#### BlueStore能否把WAL和DB存储到高性能的磁盘上吗?

- BlueStore可以额外的增加高速磁盘或者LVM来分别存储WAL和DB.
- Wal Device,用于存储BlueStore内部journal或者wal日志。一般会以block.wal链接到某个高速的lvm或者磁盘。建议把wal存储在nvme或者sata ssd上。
- DB Device,用于存储BlueStore内部元数据,当前使用的RockDB作为元数据存储的DB.一般osd的裸盘管理以及osd上的对象的元数据都存储在RocksDB上，每个osd需要一个wal和db来存储，默认创建osd时候，没有指定wal和db,会使用osd(primary设备)的空间来作为后端rocksdb的存储，实际测试发现默认(rocksdb部署在primary device,一般是hdd),读写性能比较差


#### BlueStore 管理的元数据有哪些？
- S开头的，superblock meta,针对每个osd的superblock的信息管理
- B开头的,block allcation meta,块申请的元数据管理
- C开头的,collection name,具体是有cnode_t 表示
- O开头的,object name,具体是由onode_t表示
- L开头的,write-ahead-log 管理
- M开头的,omap 管理，一般omap存储每个对象超过attr属性后的kv，一般在每个osd的后端rocksdb中存储
```
// 目前在ceph/src/os/bluestore/BlueStore.cc中定义

// 保存每个osd的超级块信息
const string PREFIX_SUPER = "S";       // field -> value
//保存Collection的元数据信息
const string PREFIX_STAT = "T";        // field -> value(int64 array)
// Collocation对应PG在BlueStore中的内存数据结构，Cnode对应PG在BlueStore中的磁盘数据结构。
const string PREFIX_COLL = "C";        // collection name -> cnode_t
// 保存每个对象的元数据信息
const string PREFIX_OBJ = "O";         // object name -> onode_t
// 保存对象的omap信息
const string PREFIX_OMAP = "M";        // u64 + keyname -> value
const string PREFIX_PGMETA_OMAP = "P"; // u64 + keyname -> value(for meta coll)
// 保存write ahead log数据
const string PREFIX_DEFERRED = "L";    // id -> deferred_transaction_t
//保存块设备的空闲extent的数据
const string PREFIX_ALLOC = "B";       // u64 offset -> u64 length (freelist)
const string PREFIX_ALLOC_BITMAP = "b";// (see BitmapFreelistManager)
const string PREFIX_SHARED_BLOB = "X"; // u64 offset -> shared_blob_t
```
#### Ceph的数据对象有哪些组成？

- ceph的一个数据对象是有名称、数据、属性、omap组成。当属性超过一定的长度，大值属性会保存在omap中

#### Rocksdb在ceph中如何使用？
- RocksDB是内嵌到每个osd的实现中，实现每个磁盘的空间管理、对象元数据、对象名称、对象属性。当执行osd的相关查询操作时候，请求会被发送到主的osd,再由主的osd的rocksdb进行响应。每个写的请求会复制到所有从的osd(这个是由min_size指定)，当数据对象在osd之间进行移动，数据对象的omap也会被复制。