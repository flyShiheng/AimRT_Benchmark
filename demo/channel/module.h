/**
 * @file module.h
*/

#pragma once

#include <string>
#include <atomic>
#include <future>

#include "aimrt_module_cpp_interface/module_base.h"
#include <test_data.pb.h>

class ChannelModule : public aimrt::ModuleBase {
 public:
  ChannelModule();
  ~ChannelModule() override;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "ChannelModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

  void MainLoop();

  void EventHandle(aimrt::channel::ContextRef ctx,
                   const std::shared_ptr<const aimrt_benchmark::proto::TestData>& data);

 private:
  aimrt::CoreRef core_;
  aimrt::executor::ExecutorRef executor_;

  std::atomic_bool run_flag_ = true;
  std::promise<void> stop_sig_;

  std::string topic_name_ = "test_topic";
  double channel_frq_ = 0.5;
  aimrt::channel::PublisherRef publisher_;
  aimrt::channel::SubscriberRef subscriber_;
};