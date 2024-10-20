# Copyright (c) 2023, AgiBot Inc.
# All rights reserved.

include(FetchContent)

message(STATUS "get AimRT ...")

set(AimRT_DOWNLOAD_URL
    "https://github.com/AimRT/AimRT/archive/refs/tags/v0.8.3.tar.gz"
    CACHE STRING "")

FetchContent_Declare(
  AimRT
  URL ${AimRT_DOWNLOAD_URL}
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  OVERRIDE_FIND_PACKAGE)

FetchContent_GetProperties(AimRT)
if(NOT AimRT_POPULATED)
  FetchContent_MakeAvailable(AimRT)
endif()
