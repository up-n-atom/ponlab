# creates the variable EXEC and sets it to dut_cli which is the executable name
set(EXEC dut_cli)
add_subdirectory(../dut_lib build)

# I../includes
set(CMAKE_BUILD_TYPE Release)
include_directories(include)
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../extern)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../extern/tclap-1.4.0-rc1/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../dut_lib/include)
include_directories(${SHARED_HEADERS_DIR})
set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")
add_definitions(-DLINUX_HOST)



# puts all .cpp files inside src to the SOURCES variable
#file(GLOB SOURCES src/*.cpp)
# Set the base path for the current module
set(MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
file(GLOB SOURCES "Application.cpp" "CommandLine.cpp" "Commands.cpp" "Config.cpp" "ConsoleImpl.cpp" "Context.cpp" "Function.cpp" "Main.cpp" "Functions.cpp" "Tools.cpp")

# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_executable(${EXEC} ${SOURCES})

target_link_libraries(${EXEC} dut_lib)
