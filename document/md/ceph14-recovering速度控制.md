###  ceph recovering速度控制


| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2021/03/29 |672152841 |

#### 问题背景
- 集群中剔除了一个osd,没有新加入，进行了一次pg的均衡，做完均衡后集群出现·` Degraded data redundancy: 256 pgs undersized`,为了保证集群的pg副本数为3，需要新添加一个osd来做pg的均衡
#### ceph 集群的状态

```
[root@node1 ~]# ceph -v
ceph version 14.2.18 (befbc92f3c11eedd8626487211d200c0b44786d9) nautilus (stable)
[root@node1 ~]# ceph -s
  cluster:
    id:     9f0ae51b-6218-4394-b83a-a97cd345f403
    health: HEALTH_WARN
            Degraded data redundancy: 256 pgs undersized
 
  services:
    mon: 3 daemons, quorum node1,node2,node3 (age 5d)
    mgr: node1(active, since 2d), standbys: node3, node2
    mds: cephfs:2 {0=node1=up:active,1=node3=up:active} 1 up:standby
    osd: 14 osds: 14 up (since 2d), 14 in (since 3d); 260 remapped pgs
 
  task status:
    scrub status:
        mds.node1: idle
        mds.node3: idle
 
  data:
    pools:   3 pools, 384 pgs
    objects: 51.80M objects, 14 TiB
    usage:   47 TiB used, 42 TiB / 89 TiB avail
    pgs:     638859/155396187 objects misplaced (0.411%)
             256 active+undersized+remapped
             124 active+clean
             4   active+remapped+backfilling
 
  io:
    recovery: 63 MiB/s, 220 objects/s
```

#### 当前ceph集群中osd的recovery的相关参数

```
[root@node1 ~]#  ceph daemon osd.0 config show  |grep recovery|grep osd
    "osd_allow_recovery_below_min_size": "true",
    "osd_async_recovery_min_cost": "100",
    "osd_debug_pretend_recovery_active": "false",
    "osd_debug_skip_full_check_in_recovery": "false",
    "osd_force_recovery_pg_log_entries_factor": "1.300000",
    "osd_min_recovery_priority": "0",
    "osd_recovery_cost": "20971520",
    "osd_recovery_delay_start": "0.000000",
    "osd_recovery_max_active": "3",
    "osd_recovery_max_chunk": "8388608",
    "osd_recovery_max_omap_entries_per_chunk": "8096",
    "osd_recovery_max_single_start": "1",
    "osd_recovery_op_priority": "3",
    "osd_recovery_op_warn_multiple": "16",
    "osd_recovery_priority": "5",
    "osd_recovery_retry_interval": "30.000000",
    "osd_recovery_sleep": "0.000000",
    "osd_recovery_sleep_hdd": "0.100000",
    "osd_recovery_sleep_hybrid": "0.025000",
    "osd_recovery_sleep_ssd": "0.000000",
    "osd_repair_during_recovery": "false",
    "osd_scrub_during_recovery": "false",
```

#### 加快Recovery的速度

```
// 集群中添加一个osd, ceph-deploy osd create  --bluestore node1 --data  /dev/sdg    --block-db cache/db1 --block-wal cache/wal1

// 查看每个osd节点的参数，或者通过  ceph daemon osd.2 config get osd_recovery_op_priority 查看单个osd的参数
ceph daemon osd.0 config show |egrep "osd_recovery_max_active|osd_recovery_op_priority|osd_max_backfills"

//每个osd节点执行如下的参数调整或者通过ceph daemon osd.2 config set osd_recovery_op_priority 1 来设置
ceph tell osd.* injectargs --osd_max_backfills=128
ceph tell osd.* injectargs --osd_recovery_op_priority=0
ceph tell osd.* injectargs --osd_recovery_max_active=64
ceph tell osd.* injectargs --osd_recovery_max_single_start=64
ceph tell osd.* injectargs --osd_recovery_sleep_hdd=0
```
#### 核心影响恢复速度的参数
- osd_max_backfills
  - 这个参数默认值10. 由于一个osd承载了多个pg,所以一个osd中的pg很大可能需要做recovery.这个参数就是设置每个osd最多能让osd_max_backfills个pg进行同时做backfill.
  - recovery做修复，通过pull或者push的backfills的操作数一般是分开的，所以一般会考虑设置这个值大一些，用于primary osd通过push修复replica osd或者primary osd 通过pull方式修复replica osd
- osd_recovery_op_priority
  - 默认值10. osd修复操作的优先级, 可小于该值;这个值越小，recovery优先级越高。高优先级会导致集群的性能降级直到recovery结束
- osd_recovery_max_active
  - 默认值15. 一个osd上可以承载多个pg, 可能好几个pg都需要recovery,这个值限定该osd最多同时有多少pg做recovery。
- osd_recovery_max_single_start
  - 默认值5. 这个值限定了每个pg可以启动recovery操作的最大数。
  - 第一种情况，配置osd_recovery_max_single_start=1，osd_recovery_max_active=3，这代表每个osd在某个时间会为一个pg最多启动1个恢复操作，并且最多可以由3个恢复操作处于活跃状态。
  - 第二种情况，配置osd_recovery_max_single_start=2，osd_recovery_max_active=3，这代表某个时间点osd会为一个pg启动2个恢复操作，并且最多能有3个恢复操作处于活跃状态。
- osd_recovery_sleep_hdd
  - 每个recovery操作之间的间隔时间，单位是ms

#### 调整完毕recovery速度
```
[root@node1 ~]# ceph -s
  cluster:
    id:     9f0ae51b-6218-4394-b83a-a97cd345f403
    health: HEALTH_WARN
            Reduced data availability: 17 pgs inactive, 17 pgs stale
            Degraded data redundancy: 251 pgs undersized
 
  services:
    mon: 3 daemons, quorum node1,node2,node3 (age 2h)
    mgr: node1(active, since 3d), standbys: node3, node2
    mds: cephfs:2 {0=node1=up:active,1=node3=up:active} 1 up:standby
    osd: 15 osds: 15 up (since 2h), 15 in (since 2h); 287 remapped pgs
 
  task status:
    scrub status:
        mds.node1: idle
        mds.node3: idle
 
  data:
    pools:   3 pools, 384 pgs
    objects: 51.80M objects, 14 TiB
    usage:   48 TiB used, 48 TiB / 96 TiB avail
    pgs:     4.427% pgs not active
             13412737/155396187 objects misplaced (8.631%)
             234 active+undersized+remapped
             80  active+clean
             48  active+remapped+backfilling
             17  stale+undersized+remapped+peered
             5   active+clean+remapped
 
  io:
    recovery: 308 MiB/s, 1.07k objects/s
 
  progress:
    Rebalancing after osd.1 marked in
      [========......................]
```
