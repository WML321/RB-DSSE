cmake_minimum_required(VERSION 3.0)
project(RB-DSSE)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(Boost COMPONENTS REQUIRED random)
include_directories(${Boost_INCLUDE_DIRS})

if(Boost_FOUND)
	MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
	MESSAGE(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
	MESSAGE(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}")
endif()

if(OpenSSL_FOUND)
	MESSAGE(STATUS "OpenSSL_INCLUDE_DIR = ${OPENSSL_INCLUDE_DIR}")
	MESSAGE(STATUS "OpenSSL_CRYPTO = ${OPENSSL_CRYPTO_LIBRARY}")
	MESSAGE(STATUS "OpenSSL_SSL" = ${OPENSSL_SSL_LIBRARY})

add_library(MyLibrary KUPRF.cpp RB-DSSE.cpp)
target_include_directories(MyLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(RB-DSSE RB-DSSE.cpp)

target_link_libraries(RB-DSSE ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} MyLibrary)
target_link_libraries(RB-DSSE OpenSSL::Crypto)
target_include_directories(RB-DSSE PRIVATE ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})