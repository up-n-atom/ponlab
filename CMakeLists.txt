# only for cmake --version >= 3.12
cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)

#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/external_toolchain.cmake)
#add_definitions(-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/external_toolchain.cmake)
# project name
project(dut)

# set the C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Do not fall back to C++98
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

message("${BoldWhite}Setting TOOLCHAIN for ${BoldGreen}'${TARGET_PLATFORM}'${BoldWhite} platform...${ColourReset}")

set(PLATFORM_TOOLCHAIN_PREFIX "${TOOLCHAIN_DIR}/bin/x86_64-openwrt-linux-")

set(CMAKE_C_COMPILER ${PLATFORM_TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${PLATFORM_TOOLCHAIN_PREFIX}g++)

# Target Environment
set(CMAKE_FIND_ROOT_PATH    ${PLATFORM_STAGING_DIR})
set(ENV{PKG_CONFIG_PATH}  "${PLATFORM_STAGING_DIR}/usr/lib/pkgconfig")

# Platform link directories
link_directories(${PLATFORM_STAGING_DIR}/usr/lib)

add_subdirectory(dut_cli)
