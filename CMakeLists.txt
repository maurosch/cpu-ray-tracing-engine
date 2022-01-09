cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-O3")

add_executable(
  main
  src/main.cpp
  src/hittables/bvh.cpp
  src/hittables/triangle_mesh.cpp
  src/engine.cpp
)

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
  testing
  tests/main.cpp
)

target_link_libraries(testing gtest_main)

include(GoogleTest)
gtest_discover_tests(testing)