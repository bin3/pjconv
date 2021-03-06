cmake_minimum_required(VERSION 2.8)

project(cmk_proj CXX)

set(root_dir ${PROJECT_SOURCE_DIR})
set(src_dir ${root_dir}/src)
set(build_dir ${root_dir}/build)
set(bin_dir ${root_dir}/bin)
set(lib_dir ${root_dir}/lib)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
 -Wall
 -Wextra
 -Werror
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 )
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")

set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
#set(CMAKE_EXE_LINKER_FLAGS "-static")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(EXECUTABLE_OUTPUT_PATH ${bin_dir})
set(LIBRARY_OUTPUT_PATH ${lib_dir})

find_package(gtest REQUIRED)

include_directories(
    ${src_dir}
)
link_directories(
    ${lib_dir}
)

#self-defined functions

# Add a library
# An optional argument can specify the dependencies
# Example1: add_lib(foo "foo.cpp util.cpp")
# Example2: add_lib(foo "foo.cpp util.cpp" "bar glog")
function(add_lib name srcstr)
	message(STATUS "[add_lib] name=${name}, srcstr=${srcstr}, depstr=${depstr}")
  string(REGEX MATCHALL "[^ ;]+" srcs "${srcstr}")
	message(STATUS "srcs=${srcs}")
	add_library(${name} ${srcs})
  if (${ARGC} GREATER 2)
    string(REGEX MATCHALL "[^ ;]+" deps "${ARGV2}")
    list(LENGTH deps len)
    if (${len} GREATER 0)
      message(STATUS "${len} deps: ${deps}")
      target_link_libraries(${name} ${deps})
    endif()
  endif()
endfunction()	

# Add a binary
# An optional argument can specify the dependencies
# Example1: add_bin(foo_main)
# Example2: add_bin(foo_main "glog")
# Example3: add_bin(foo_main "protobuf glog")
function(add_bin name)
  message(STATUS "[add_bin] ARGC=${ARGC}, ARGV=${ARGV}")
  add_executable(${name} ${name})
  if (${ARGC} GREATER 1)
    string(REGEX MATCHALL "[^ ;]+" deps "${ARGV1}")
    list(LENGTH deps len)
    if (${len} GREATER 0)
      message(STATUS "${len} deps: ${deps}")
      target_link_libraries(${name} ${deps})
    endif()
  endif()
endfunction()	

# Add a unittest
# An optional argument can specify the dependencies
# Example1: add_test(foo_test)
# Example2: add_test(foo_test "glog")
# Example3: add_test(foo_test "protobuf glog")
function(add_test name)
  message(STATUS "[add_test] name=${name}, ARGC=${ARGC}, ARGV=${ARGV}")
  set(libs gtest gtest_main)
  if (${ARGC} GREATER 1)
    string(REGEX MATCHALL "[^ ]+" deps "${ARGV1}")
    list(LENGTH deps len)
    if (${len} GREATER 0)
      message(STATUS "${len} deps: ${deps}")
      list(APPEND libs ${deps})
    endif()
  endif()
  message(STATUS "[add_test] name=${name}, libs=${libs}")
  add_executable(${name} ${name})
  target_link_libraries(${name} ${libs})
endfunction()	

add_subdirectory(src)

