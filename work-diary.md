# 工作日志

## 2013-08-13
09:00 ~ 11:27 
收发邮件，换取设备，和破云交流汇金的学习任务  
11:28 ~ 18:00  

- mvn clean compile, mvn clean test, mvn clean package, mvn clean install, mvn-shade-plugin
- 独立做一遍代码检出和运行
- 和破云讨论了一下学习任务的优先级，已确定先从业务开始，其它延后
- 尝试访问测试环境
- 5W2H on 汇金整体架构
- 5W2H on 代码结构
- github 上增加 MavenSpring 库，将 MavenSpringHibernante 中只跟 spring 本身相关的工程加入进来，然后，再增加 Spring_3600_MyBatis 工程，并且将其 pom.xml 文件中的 JUnit 依赖关系改为 4.x
- 从子系统/模块/层次/入口等不同角度理解 hjfinance 的代码
- 学习一下 SVN 的基本操作
- 学习一下项目中涉及的 mvn eclipse 命令

## 2013-08-14
09:00 ~ 15:04 修正了自己以前做的一些 Spring IoC 和 AOP 相关的模板工程，以方便以后开发时查看或使用

15:05 ~ 18:00 学习 mvn 打包 ，遇到问题：
org.xml.sax.SAXParseException: schema_reference.4: Failed to read schema documen
t 'http://www.springframework.org/schema/beans/spring-beans-3.1.xsd', because 1)
could not find the document; 2) the document could not be read; 3) the root ele
ment of the document is not <xsd:schema>.

原因分析与解决方案：
[http://blog.csdn.net/leonzhouwei/article/details/9970985](http://blog.csdn.net/leonzhouwei/article/details/9970985)

## 2013-08-15
- 命令行可运行的项目，如何支持在构建以后对 jar 包中的资源文件进行读取

## 2013-08-16
- 理解需求
- 学习开发所需技术
- 理解现有代码

http://ops.jm.taobao.net/service-manager/
http://gitlab.alibaba-inc.com/wuxiang/tbbpm/wikis/home
http://cwf.cloud.tmall.com/update_site/

Q&A  
1. SubRefundService.applyRefund 不存在
2. 如何通过 .bpm 节点定位到处理流程中的某个点对应的类
3. 

Q: 如何理解一个工程？

A:       
  
- 1. 系统如何被启动
- 2. 功能如何被触发

## 2013-08-19
- 理解退款工作流是如何注册、被使用的
- BizType WorkflowTypeEnum.REFUND(1L, "退款")
- BizCode RefundTypeEnum.REFUND_SUB_ORDER(2L, "订购退款")
- NodeType NodeTypeEnum.APPLY(1L, "申请")

## 2013-08-20
- 退款工作流是如何被定位到的，即 refund_isv_self.bpm 是怎么被定位到的
- select * from hj_workflow_config where type=1 and code=2 and node_type=1;
    > id: 300  
    > code: 2-NISV-*  
    > workflow_code: com.taobao.huijin.biz.refund.refund_isv_self  
    > group_id:  
    > gmt_create: 2012-09-12 19:42:03  
    > gmt_modified: 2012-09-12 19:49:46  
    > node_type: 1  
    > auto_pay: 0  
    > status: 1  
    > type: 1  

## 2013-08-21
- 理解和熟悉 BizType（业务类型）, BizCode（业务编码）, WorkflowCode（工作流编码）, WorkflowNode（工作流节点） 概念，及其和数据库表字段的对应
- 理解现有的审核后退款逻辑
    > - AbstractWorkflowTaskServiceImpl.update(Map<String, Object>)
    > - SubRefundServiceImpl.alipayRefund(String, Long)

## 2013-08-22
- 理解现有的审核后退款逻辑
    > - AbstractWorkflowTaskServiceImpl.update(Map<String, Object>)
    > - SubRefundServiceImpl.alipayRefund(String, Long)
- 问题  
    > - alipayRefundService(String refundOrderId, Long instanceId) 这里的 instanceId 如何理解？是当前工作流实例的唯一标识？如何生成的？
    > - 在 refund_sub_std.bpm 中，“出纳审核同意”节点 到 “打款”节点，是如何对接的？
- 回答
    > instanceId 即为工作流实例的唯一标识，由工作流引擎自动生成
    > bpm 文件在编译后就会自动生成对接逻辑

## 2013-08-26
- bizCode 起什么作用？（路由到对应的工作流）如何使用？
- 设计前置条件为允许执行的 N天无理由退款 的工作流逻辑
    - 注意点
        1. 异步操作的必然性，即，用户对同一个订单多次提交了无理由退款请求(通过设计无理由退款状态来避免，大致分为可开启，待退款，处理中，退款失败，关闭?)     
- 设计管理员查询/操作任意 N天无理由退款 的逻辑
- 七天无理由退款：
    > - 是否需要对金额上限(例如1,000,000)作出控制，即使其小于等于系统建议的金额
    > - 对重复的无理由退款要能够进行检测和去重（有）
    > - 必须有无理由退款日志
    > - 能够通过用户界面查询出符合条件的无理由退款的记录，且同时显示针对本次查询的统计信息
    > - 管理员能够通过用户界面随时停止/开启无理由退款服务
    > - 指定一个周期或者时间点，系统会自动发送无理由退款统计邮件

## 2013-08-27 
- 理解7天无理由退款工作流的实现细节，理解从用户发出请求到收到相应的完整通路的全部实现细节，尤其是数据！数据！数据！

## 2013-08-28
Q&A:

- Q: 7天无理由退款的要求之一是，“发起退款的时间在订购记录的开始时间后的7天（含7天）内”。那么，“发起退款的时间”我可以认为是指上图中的“冻结时间”吗？
- A:  
- 理解7天无理由退款工作流的实现细节，理解从用户发出请求到收到相应的完整通路的全部实现细节，尤其是数据！数据！数据！

## 2013-08-29
Q&A:

- Q: 7天无理由退款的要求之一是，“发起退款的时间在订购记录的开始时间后的7天（含7天）内”。那么，“发起退款的时间”我可以认为是指上图中的“冻结时间”吗？
- A:  

## 2013-08-30
- 理解现有退款的从用户发出请求到收到相应的完整通路的全部实现细节，尤其是数据！数据！数据！
- 和律齐确认了7天无理由退款只需要修改现有工作流，即，7天无理由退款不需要经过现有的“财务审核”节点
- 向园葵请教 upp\_prod\_subscription 各字段的含义和用途

## 2013-09-02
Q&A:

- Q: 7天无理由退款的要求之一是，“发起退款的时间在订购记录的开始时间后的7天（含7天）内”。那么，“发起退款的时间”我可以认为是指上图中的“冻结时间”吗？
- A: YES
- 7天无理由退款指会员发起退款的时间在订购记录的开始时间后的7天，那么，在实现的时候，是应该使用 冻结时间 还是 getRefundTime() 得出的退款时间？目测使用getRefundTime() 得出的退款时间保持逻辑一致比较好。

## 2013-09-03
    getOldOPConfig("DEFAULT_APP_ORDER", "DEFAULT_OP_ORDER");
        getAppConfByCodeAndType("DEFAULT_APP_ORDER=DEFAULT_OP_ORDER", "OLD_APP_CONF");

- Q: 哪些业务类型可以允许进行无理由退款
- A:  
- 理解现有退款的从用户发出请求到收到相应的完整通路的全部实现细节，尤其是数据！数据！数据！
- 整个退款逻辑，比如分层
- 完全理解现有退款流程的通路

## 2013-09-04
- 修改实习生转正审核申请表

## 2013-09-05
- 重温汇金系统的基础知识
- 撰写转正面试 PPT
- 修改简历
- 颐和MM
- logback 官方英文文档再学习其术语与使用，重点关注配置文件的思维模型，根据包或者开发人员输出特定的日志文件以及日志自动归档压缩

## 2013-09-06

## 2013-09-09
- 自测环境部署
    1. 发布修改后的工作流
    2. 合并代码
    3. ？

## 2013-09-10

## 2013-09-11
- 七天无理由退款的优化，只需要修改两个点，一个是系统建议退款金额，另一个是其只要走直接打款即可，不需要走工作流。

## 2013-09-12

## 2013-09-15
- 再试商家自助退款，没有成功，需要靖雨/恒晔帮看一下
- 退款金额超出应退金额测试, 66%

## 2013-09-16
- 7天无理由退款边界条件测试
- 7天无理由退款边界异常工作流测试
- 国庆前业务需求暂停申请

## 2013-09-17
- 账本和账单的区别是什么？
- 为什么会有异步和同步两种消息网关，分别对应什么应用场景？
- billing server 和 finance 都有退款，区别是什么？
- 配置中心的各项规则的实现，是哪个子系统？
- 不同场景中，如何决定是发送 HSF 消息，还是发送 Notify 消息？
- 业务流程服务是什么？
- 定时调度服务是什么？
- 汇金介绍的标准用例模型中，消息发送(notify/hsf)，hsf 的消息可以被消息网关拦截？

## 2013-09-22
- 汇金系统组成
- 7天无理由退款优化前和优化后的不同
- 7天无理由退款优化前的退款业务的技术实现逻辑
    - 角色划分
        - 小二
            - 用户事件触发的动作： SubRefundApplyAction.doApply(@FormGroup( "subRefundApply") SubRefundDTO subRefundDTO,
            @Param( "punishFlag") Long punishFlag, Navigator nav, TurbineRunData rundata,
            Context context)
        - 非小二
            - 注册到 HSF 上的服务实现：ProdSubRefundServiceImpl.applyRefund(RefundRequestDTO refundRequestDTO)
- 7天无理由退款优化的自测用输入数据制造
- 为什么7天无理由退款优化的复杂度超出预期？
    - 理解优化前退款逻辑
        - 系统架构、技术组成，破云只是略微说了一下，然后给了链接让自己看
        - 破云并不了解优化前的退款逻辑，而优化前的退款逻辑则是董框开发的，因此，很多时候是和董框在进行交流
        - 团队业务较多，向董框询问不能理解的代码或者逻辑时，常常一天以后才会有答复
    - 编码
        - 因开发任务较重，第一次开发完成以后开始自测时，董框才了解了下优化逻辑，然后认为新增代码只覆盖了一半的入口，需要重构
    - 自测 
        - 向测试同学请教如何制造测试用数据库记录时，才了解到该需求同时涉及了服务和汇金两个平台，因此制造测试用数据其实也是比较麻烦的（测试同学为了模拟出商家自助退款分支，花了约3小时来设计测试链接和软件商品）
- 模仿 GFS 中，data flow 和 control flow 的流动，在纸上画出信息的流动

## 2013-09-23
- 审批退款，工作流状态在哪张表中(hj_workflow_info) 
- hjfinance 如何向 HSF 注册服务实现
    - 必选
        - 服务定义 (service interface)
        - 服务实现 (target)
    - 可选
        - 版本信息
## 2013-09-24
- HSF 是什么？（简而言之，就是一个可以做到分布式的 RPC 框架，即，多个 HSF 实例可以构成一个分布式 RPC 集群）
- HSF 术语
    - 服务（定义）
    - （服务）发布者
    - （服务）消费者 
- HSF 服务发布代码分析
    1. 出发点: HSFSpringProviderBean
    - 注册服务接口并获得一个 com.taobao.config.client.Publisher 实例
        - unique service name ("<接口全名\>:<版本号\>")
        - publisher id ("HSFProvider-<${unique service name}\>")
    - com.taobao.config.client.Publisher 实例发布服务实现 
        - "<ip\>:<port\>?[<name\>=<value\>[&...]]"
    - 源码

            private Publisher<String> doPublish(ServiceMetadata metadata) {
                final String serviceUniqueName = metadata.getUniqueName();
                final String data = HSFServiceTargetUtil.getTarget(
                		configService.getHSFServerPort(), metadata);
                // 根据服务类型，注册服务提供者，保证服务在本机的唯一性
                final String publisherId = PUBLISHER_PREFIX + serviceUniqueName;
                PublisherRegistration<String> registration = new PublisherRegistration<String>(
                		publisherId, serviceUniqueName);
                registration.setGroup(metadata.getGroup());
                // 将服务注册到 config server 上
                Publisher<String>  publisher = PublisherRegistrar.register(registration);
                publisher.publish(data);
                return publisher;
            }
- ConfigServer
- 袁鸣凯的 Java 面试题系列

## 转正面试准备内容
- 目的：主动地让面试官了解实习一个月所完成的业务、整理的文档，以及到目前为止，个人的能力、知识和经验（深度目前还是不够的，但 server 端开发、客户端开发所需广度积累比较均衡，且并非那种流于表面的广度，而是可以做到从想法到原型一气呵成的程度）
- 面试用思维导图
    - 必需：
        - 汇金系统组成、汇金各个子系统的定位、各个环境的区别、整理7天无理由退款的业务需求变更、如何新增工作流、如何向 HSF 注册服务接口与实现、HSF 是如何做到 Load Balance 的
        - 我的技术博客，例如 Actor 编程模型与 “不要使用共享内存来通信，而应该使用通信来共享内存”
        - JavaFX-based jjf
        - my hadoop learning projects    
        - Android-and-Arduino-based 智能灯控 (!android 的蓝牙库编程模型)
        - GFS
        - Google Code Jam    
        - Spanner
        - CFrame
    - 可选
        - MapReduce
        - BigTable
        - UNIX 编程艺术
        - golang
        - F1
        - Jeff Dean 的两个 pdf
        - 建议模仿 facebook，正式发布后一段时间以内的的线上环境再分为小规模线上环境和正式环境

## 实习期间的针对项目的学习计划
- 《Effective Java》每天一小时
- Findbugs 学习与使用
- 学习 Velocity
- 无理由退款查询页面
- 学习 WEBX
- 学习 iBatis/MyBatis
- iBatis 是怎么和 Spring 整合的，配置 XML 是哪个
- 理解 工作流和 bpm 的使用
- [LMAX 架构](http://www.jdon.com/42452)
- [The LMAX Architecture](http://martinfowler.com/articles/lmax.html)
- jenkins 与 git 集成和使用
- 关注 SOA、交易系统、架构（同步异步、Twitter、CQRS、监控）
- 学习领域驱动设计
    - [disruptor](http://coolshell.cn/?s=DISRUPTOR)
    - [Jdon 框架](http://www.jdon.com/jdonframework/)
    - [What SOA do you have (with extended EDA and CQRS material)](http://www.slideshare.net/jeppec/what-soa-eda-and-cqrs-version)
    - [CQRS](http://www.jdon.com/cqrs.html)
- joda time, guava, moco, mockito 学习
- 理解代码结构，如层次、模块
- 体验现有退款功能的 GUI
- 数据库是如何连接的
- 了解整个汇金系统的架构
- 理解退款功能是如何注册到 HSF 的 (finance-server-hsf.xml)
- 理解系统是如何被启动的 (applicationContext.xml)
- HSF
    - 理解 HSF 是如何做到多台节点并行提供相同服务的，例如，退款服务目前在日常环境中可由两台机器并行提供
    - [服务框架HSF分析之一容器启动](http://iwinit.iteye.com/blog/1745132)
    - [HSF源码阅读笔记（一）](http://www.cnblogs.com/dongqingswt/archive/2013/01/22/2872068.html)