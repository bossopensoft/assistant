# 博思客户端 QT 版本

## 版本记录：

* 2020-09-13，yanbin：创建该工程，并添加初始文件。

## 1. 目录结构说明：

```
├── external	- 博思客户端程序所依赖的外部模块源码
│   └── QtWebApp - QtWebApp 工程，用于http/https协议收发实现；
├── lib - 工程依赖的外部库目录
└── src - 工程源码路径
    ├── controllers - http/https中所有子路径下服务类模块源码
    ├── main.cpp - 工程主函数入口文件
    ├── utility - 公共类
    └── view-models - ui模块以及ui中控制逻辑模块源码
```

## 2. 编译运行方法：
```  
9月18日11点13分提交的版本：  
在WIN10平台下需要安装openssl后在qt pro文件中加一句代码INCLUDEPATH += C:/OpenSSL-Win32/include，然后就可以正常使用。  
参考：https://blog.csdn.net/qq_44980390/article/details/103353070?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param  
在win10系统下，qt可以使用http及https通信。  
在uos、ubuntu下，qt可以使用http通信，https通信还未通。  
```  
