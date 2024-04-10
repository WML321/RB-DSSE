# CMakeList.txt: RB-DSSE 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
#find_package(OpenSSL REQUIRED)
#include_directories(D:/visual studio/openssl/OpenSSL-Win64/includ)
#link_directories(D:/visual studio/openssl/OpenSSL-Win64/lib)
#link_libraries(openssl libcrypto)
include_directories("D:\\visual studio\\openssl\\OpenSSL-Win64\\include")
link_directories("D:\\visual studio\\openssl\\OpenSSL-Win64\\lib")
link_libraries(openssl libcrypto)

#指定boost根目录
set(BOOST_ROOT "D:\\visual studio\\boost_1_84_0")

#如果已经找到

#MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
#MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
#MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
#MESSAGE( STATUS "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}.")
include_directories(${BOOST_ROOT})
link_directories(${BOOST_ROOT}/lib)
add_executable (RB-DSSE "RB-DSSE.cpp" "RB-DSSE.h" "HEAD.h" "Utils.h" "Client.h" "Server.h")
target_link_libraries (RB-DSSE ${Boost_LIBRARIES})
target_link_libraries(RB-DSSE openssl)


# 将源代码添加到此项目的可执行文件。




if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RB-DSSE PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
