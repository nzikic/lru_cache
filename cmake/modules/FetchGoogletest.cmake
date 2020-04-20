include(FetchContent)

set (CMAKE_PROJECT_googletest-distribution_INCLUDE_BEFORE ${CMAKE_SOURCE_DIR}/cmake/modules/GoogleTest-policy.cmake)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.10.0
)

FetchContent_MakeAvailable(googletest)

# Create alias for targets.
if (NOT TARGET GTest::gtest)
    add_library(GTest::gtest ALIAS gtest)
endif()
if (NOT TARGET GTest::main)
    add_library(GTest::main ALIAS gtest_main)
endif()

if (NOT TARGET GMock::gmock)
    target_link_libraries(gmock PUBLIC GTest::gtest) # Note: cannot use INTERFACE here
    add_library(GMock::gmock ALIAS gmock)
endif()
if (NOT TARGET GMock::main)
    target_link_libraries(gmock_main PUBLIC GTest::gtest) # Note: cannot use INTERFACE here
    add_library(GMock::main ALIAS gmock_main)
endif()
