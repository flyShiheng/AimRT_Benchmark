#!/bin/bash

update_source_x86_64() {
  # https://mirrors.tuna.tsinghua.edu.cn/help/ubuntu/
  echo "deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ jammy main restricted universe multiverse" > /etc/apt/sources.list
  echo "deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ jammy-updates main restricted universe multiverse" >> /etc/apt/sources.list
  echo "deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ jammy-backports main restricted universe multiverse" >> /etc/apt/sources.list
  echo "deb http://security.ubuntu.com/ubuntu/ jammy-security main restricted universe multiverse" >> /etc/apt/sources.list
}

update_source_arm64() {
  # https://mirrors.tuna.tsinghua.edu.cn/help/ubuntu-ports/
  echo "deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ jammy main restricted universe multiverse" > /etc/apt/sources.list
  echo "deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ jammy-updates main restricted universe multiverse" >> /etc/apt/sources.list
  echo "deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ jammy-backports main restricted universe multiverse" >> /etc/apt/sources.list
  echo "deb http://ports.ubuntu.com/ubuntu-ports/ jammy-security main restricted universe multiverse" >> /etc/apt/sources.list
}

update_source() {
  mv /etc/apt/sources.list /etc/apt/sources.list.bak
  if [ "$(uname -m)" == "x86_64" ]; then
    echo "update source for x86_64 ..."
    update_source_x86_64
  else
    echo "update source for arm64 ..."
    update_source_arm64
  fi
  apt update
  apt upgrade -y
}

install_lib() {
  echo "Install lib ..."
  apt update
  apt install -y perl patch less 
  apt install -y gcc g++ python3-dev python3-pip vim git
  apt install -y curl wget openssl libcurl4-openssl-dev libssl-dev
  apt install -y libboost-dev libsqlite-dev libasio-dev
  apt install -y libngtcp2-dev libnghttp3-dev libsystemd-dev libjansson-dev libevent-dev
  apt install -y emqx 
  apt install -y libc6-dev libacl1-dev
  apt install -y --reinstall ca-certificates
}

install_cmake() {
  # https://github.com/Kitware/CMake/releases/tag/v3.30.0
  echo "Install cmake ..."
  apt install -y --reinstall ca-certificates
  wget https://github.com/Kitware/CMake/archive/refs/tags/v3.30.0.tar.gz > wget_log
  tar -zxvf v3.30.0.tar.gz
  cd CMake-3.30.0
  mkdir build
  cd build
  ../configure
  make -j16
  make install
  if [ -f /usr/bin/cmake ]; then
    mv /usr/bin/cmake /usr/bin/cmake.back
  fi
  ln -s /usr/local/bin/cmake /usr/bin/cmake
  cmake --version
  cd ../..
}

install_ros2() {
  # https://docs.ros.org/en/humble/index.html
  # https://mirrors.tuna.tsinghua.edu.cn/help/ros2/
  echo "Install ros2 ..."

  apt install -y gnupg2
  curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key  -o /usr/share/keyrings/ros-archive-keyring.gpg
  echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] https://mirrors.tuna.tsinghua.edu.cn/ros2/ubuntu jammy main" | tee /etc/apt/sources.list.d/ros2.list > /dev/null

  apt update
  apt upgrade -y
  apt install -y ros-humble-ros-base
  apt install -y ros-dev-tools
  source /opt/ros/humble/setup.bash

  echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
}

update_source
install_lib
install_ros2
install_cmake