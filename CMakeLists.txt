#
# a CMakeLists.txt for users on midway
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(pamn)

set(CMAKE_INSTALL_PREFIX "../bin")

# setting ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)
include(${ROOT_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.C)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_definitions(${ROOT_CXX_FLAGS})
add_executable(pamn ${sources} ${headers})
target_link_libraries(pamn ${ROOT_LIBRARIES})

# compiler options
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -O1")

# install the binary
install(TARGETS pamn DESTINATION ${CMAKE_INSTALL_PREFIX})
