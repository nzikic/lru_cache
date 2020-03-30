cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (lru_cache CXX)

#
# lru_cache - header-only library
#

add_library (lru_cache INTERFACE)
target_include_directories (lru_cache INTERFACE include/)

# c++17 has to be enabled
set(CXX_STANDARD_REQUIRED ON)
target_compile_features(lru_cache INTERFACE cxx_std_17)

#
# build unit tests (default off)
#
option(LRU_CACHE_BUILD_UNIT_TESTS "Build unit tests" ON)

if (LRU_CACHE_BUILD_UNIT_TESTS)
    enable_testing()

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
    find_package(GTest REQUIRED)
    find_package(GMock REQUIRED)

    add_subdirectory(test)

endif(LRU_CACHE_BUILD_UNIT_TESTS)

#
# examples
#
option(LRU_CACHE_BUILD_EXAMPLES "Build examples" ON)

if (LRU_CACHE_BUILD_EXAMPLES)
    add_subdirectory(examples/)
endif(LRU_CACHE_BUILD_EXAMPLES)


#installation
install (DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
