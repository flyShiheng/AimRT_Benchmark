
set(SourceCode ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(${PROJECT_NAME}_node ${SourceCode})

target_link_libraries(${PROJECT_NAME}_node
  aimrt::runtime::core
  # AimRT
  )