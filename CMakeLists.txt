cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)


file (GLOB_RECURSE Foo_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE Foo_HEADERS CONFIGURE_DEPENDS "src/*.h")

set (Foo_INCLUDE_DIRS "")
foreach (_headerFile ${Foo_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Foo_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES Foo_INCLUDE_DIRS)

add_executable(main ${Foo_SOURCES} main.cpp)
add_library(main_lib ${Foo_SOURCES})
target_include_directories(main PRIVATE ${Foo_INCLUDE_DIRS})


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)

enable_testing()

add_executable(
  spaceDividerTest
  tests/spaceDividerTest.cpp
)

target_link_libraries(spaceDividerTest main_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(spaceDividerTest)