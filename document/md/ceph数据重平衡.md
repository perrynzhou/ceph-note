### Ceph数据重平衡

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2020/12/01 |672152841 |

- 当集群出现数据分布不均匀，可以手动调整每个osd的reweight可以触发PG在osd之间进行迁移，以恢复数据平衡。这个操作可以先查看集群的空间利用率，其次找到磁盘利用率较高的osd,逐个进行调整
  ```
  //查看当前集群的磁盘录用率
  ceph osd df tree
  // 调整空间利用率较高的osd，然后进行调整
  ceph osd reweight {osd-id} {reweight}
  ```
 
- ceph支持批量调整，支持两种模式：一种是按照osd的空间利用率(reweight-by-utilization)。另外一种是按照pg在osd之间的分布(reweight-by-pg),为了防止影响前端业务，可以进行测试执行，这个会触发pg迁移数量的相关统计，方便合适进行调整。
  
  ```
  // overload:可选，整形类型，默认是120，这个值应该不小于100
  // max_change:可选，浮点类型在[0,1]之间，默认值受mon_reweight_max_change控制，默认是0.05.每次调整reweight的最大幅度，即调整上限。实际每个osd的调整幅度取决于自身空间利用率和集群平均空间利用率的偏差幅度，偏离越多，则调整幅度越大；反之就越小
  // max_osds:可选，整形类型，默认值受mon_reweight_max_osds控制，默认是4；每次最多调整的osd的数目
  // --no-increasing:可选字符类型
  ceph osd test-reweight-by-utilization {overload} {max_change} {max_osds} {--no-increasing}
  ```
- 使用例子
```
//测试执行
ceph osd test-reweight-by-utilization 110 0.2 2 --no-increasing

// 实际执行
ceph osd reweight-by-utilization 110 0.2 2 --no-increasing

```