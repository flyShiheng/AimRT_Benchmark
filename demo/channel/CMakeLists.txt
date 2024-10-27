
set(SourceCode ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/module.cpp)

add_executable(${PROJECT_NAME}_node ${SourceCode})

target_link_libraries(${PROJECT_NAME}_node
  yaml-cpp::yaml-cpp
  aimrt::runtime::core
  aimrt_benchmark::proto_pb_gencode
)

install(FILES channel.yaml DESTINATION ./bin)
install(FILES start_node.sh DESTINATION ./bin)
install(TARGETS ${PROJECT_NAME}_node DESTINATION ./bin)