# MiniOB 介绍
我的数据库内核开发之旅，基于 Miniob 框架实现一个单机数据库。


## 快速上手

# 前提
    cmake 版本 >= 3.13
    gcc/clang gcc 11 以上，clang 14以上，编译器需要支持c++20新标准
    flex (2.5+), bison (3.7+) 用于生成词法语法分析代码
    

# 编译
    环境初始化
    bash build.sh init
    
    编译debug
    bash build.sh
    
    编译release
    bash build.sh release


# 运行
    以直接执行命令的方式启动服务端程序
    ./bin/observer -f ../etc/observer.ini -P cli



refer: https://oceanbase.github.io/miniob/