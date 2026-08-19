file(GLOB DUT_LIB_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
list(REMOVE_ITEM DUT_LIB_SOURCES "${CMAKE_CURRENT_LIST_DIR}/WinSockInit.cpp")
list(REMOVE_ITEM DUT_LIB_SOURCES "${CMAKE_CURRENT_LIST_DIR}/WindowsConsoleLogger.cpp")
list(REMOVE_ITEM DUT_LIB_SOURCES "${CMAKE_CURRENT_LIST_DIR}/ConnectionImpl.cpp")

add_library(dut_lib ${DUT_LIB_SOURCES})
message("dutlib current source dir = ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${SHARED_HEADERS_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../extern)
target_include_directories( dut_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")
add_definitions(-DLINUX_HOST)
