cmake_minimum_required(VERSION 3.14)
project(videomaker)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 17)

# Enable Compiler warnings
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options()
endif()


################################# Dependencies ################################
include(FetchContent)

######## Google Test ########
# Fetch GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


##### matplot++ #########
FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus)
FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

################################# Targets #####################################

######## Libraries ########

# cppmpc Library
add_library(videomaker
    videomaker/VideoMaker.h
    videomaker/VideoMaker.cpp
)
target_include_directories(videomaker PUBLIC videomaker/)
target_link_libraries(videomaker matplot)
target_compile_options(videomaker PRIVATE -Wall -Wextra -Wpedantic -Werror)


######## Tests ########
enable_testing()

add_executable(videomaker_tests
    tests/VideoMakerTests.cpp)
target_link_libraries(videomaker_tests gtest_main videomaker matplot)
target_compile_options(videomaker_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)

include(GoogleTest)
gtest_discover_tests(videomaker_tests)
