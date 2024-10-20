# AimRT_Benchmark

  一个AimRT的测试框架，支持一下特性
  1. 支持手动配置任意通信节点，每个节点任意发布者/订阅者端点，每个发布者端点以任意频率和任意数据大小发送数据
  2. 提供AimRT docker环境配置，一键配置AimRT开发环境
  3. 支持构建随机拓扑
  4. 支持输出拓扑图
  5. 支持AimRT所支持的后端插件
  6. 支持保存节点所接收的可分析数据


  docker build --progress=plain -t ubuntu_test .
  docker run -it ubuntu_test
  docker run -it -v path_in_host:path_in_docker ubuntu_test

  bash build.sh
