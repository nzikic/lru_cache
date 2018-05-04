cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (lru_cache VERSION 1.0)

# c++17 has to be enabled
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

# header-only library
add_library (lru_cache INTERFACE)
target_include_directories (lru_cache INTERFACE include/)

# sources (only to be IDE friendly)
target_sources (lru_cache INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/lru_cache/lru_cache.hpp)

#installation
install (DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
