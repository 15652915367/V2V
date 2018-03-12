cmake_minimum_required(VERSION 2.6)

project(v2v)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "release")
endif()

set(
    CXX_FLAGS
    -DMUDUO_STD_STRING
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -W
    -Wall
#    -march=native
#    -rdynamic

	-static
)
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")




set(CMAKE_CXX_COMPILER "/home/idiot/Downloads/linaro/gcc-linaro-arm-linux-gnueabihf-4.9-2014.07_linux/bin/arm-linux-gnueabihf-g++")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(STATIC_LIBRARIES_INPUT_PATH ${PROJECT_SOURCE_DIR}/src/lib)

#find_package(Boost REQUIRED)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/include/v2v_asn1c/)

# boost, on my own, for cross compile
INCLUDE_DIRECTORIES(/home/idiot/Documents/boost_1_60_0/)
link_directories("/home/idiot/Documents/boost_1_60_0/") 
# zlib 
INCLUDE_DIRECTORIES(/home/idiot/Documents/zlib-1.2.11)
link_directories("/home/idiot/Documents/zlib-1.2.11")
# curl
INCLUDE_DIRECTORIES(/usr/local/arm/curl/include)
link_directories("/usr/local/arm/curl/lib")
# openssl
INCLUDE_DIRECTORIES(/usr/local/arm/openssl/include)
link_directories("/usr/local/arm/openssl/lib")



string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(src)
