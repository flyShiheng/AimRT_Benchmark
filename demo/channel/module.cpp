#include "aimrt_module_protobuf_interface/channel/protobuf_channel.h"
#include "aimrt_module_protobuf_interface/util/protobuf_tools.h"

#include "yaml-cpp/yaml.h"

#include "module.h"

ChannelModule::ChannelModule() {}
ChannelModule::~ChannelModule() {}

bool ChannelModule::Initialize(aimrt::CoreRef core) {
  core_ = core;

  try {
    // Read cfg
    auto file_path = core_.GetConfigurator().GetConfigFilePath();
    if (!file_path.empty()) {
      YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
      topic_name_ = cfg_node["topic_name"].as<std::string>();
      channel_frq_ = cfg_node["channel_frq"].as<double>();
    }

    // Get executor handle
    executor_ = core_.GetExecutorManager().GetExecutor("work_thread_pool");
    AIMRT_CHECK_ERROR_THROW(executor_ && executor_.SupportTimerSchedule(),
                            "Get executor 'work_thread_pool' failed.");

    // Register publish type
    publisher_ = core_.GetChannelHandle().GetPublisher(topic_name_);
    AIMRT_CHECK_ERROR_THROW(publisher_, "Get publisher for topic '{}' failed.", topic_name_);

    bool ret = aimrt::channel::RegisterPublishType<aimrt_benchmark::proto::TestData>(publisher_);
    AIMRT_CHECK_ERROR_THROW(ret, "Register publish type failed.");

    subscriber_ = core_.GetChannelHandle().GetSubscriber(topic_name_);
    AIMRT_CHECK_ERROR_THROW(subscriber_, "Get subscriber for topic '{}' failed.", topic_name_);

    ret = aimrt::channel::Subscribe<aimrt_benchmark::proto::TestData>(
        subscriber_,
        std::bind(&ChannelModule::EventHandle, this, std::placeholders::_1, std::placeholders::_2));
    AIMRT_CHECK_ERROR_THROW(ret, "Subscribe failed.");
  } catch (const std::exception& e) {
    AIMRT_ERROR("Init failed, {}", e.what());
    return false;
  }

  AIMRT_INFO("Init succeeded.");
  return true;
}

bool ChannelModule::Start() {
  try {
    executor_.Execute(std::bind(&ChannelModule::MainLoop, this));
  } catch (const std::exception& e) {
    AIMRT_ERROR("Start failed, {}", e.what());
    return false;
  }

  AIMRT_INFO("Start succeeded.");
  return true;
}

void ChannelModule::Shutdown() {
  try {
    run_flag_ = false;
    stop_sig_.get_future().wait();
  } catch (const std::exception& e) {
    AIMRT_ERROR("Shutdown failed, {}", e.what());
    return;
  }

  AIMRT_INFO("Shutdown succeeded.");
}

// Main loop
void ChannelModule::MainLoop() {
  try {
    AIMRT_INFO("Start MainLoop.");

    aimrt::channel::PublisherProxy<aimrt_benchmark::proto::TestData> publisher_proxy(publisher_);

    uint32_t count = 0;
    while (run_flag_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<uint32_t>(1000 / channel_frq_)));

      count++;
      AIMRT_INFO("Loop count : {} -------------------------", count);

      // publish event
      aimrt_benchmark::proto::TestData msg;
      msg.set_message("count: " + std::to_string(count));
      msg.set_index(count);

      AIMRT_INFO("Publish new pb event, data: {}", aimrt::Pb2CompactJson(msg));
      publisher_proxy.Publish(msg);
    }

    AIMRT_INFO("Exit MainLoop.");
  } catch (const std::exception& e) {
    AIMRT_ERROR("Exit MainLoop with exception, {}", e.what());
  }

  stop_sig_.set_value();
}

void ChannelModule::EventHandle(aimrt::channel::ContextRef ctx,
                  const std::shared_ptr<const aimrt_benchmark::proto::TestData>& data) {
  AIMRT_INFO("Receive new pb event, ctx: {}, data: {}", ctx.ToString(), aimrt::Pb2CompactJson(*data));
}