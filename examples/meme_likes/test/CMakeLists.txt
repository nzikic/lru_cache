add_executable(example_meme_likes_tests)

target_include_directories (example_meme_likes_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(example_meme_likes_tests
    PRIVATE
        lru_cache
        lib_example_meme_likes
        GMock::main
)

target_sources(example_meme_likes_tests
    PRIVATE
        provider_tests.cpp
        database_tests.cpp
)


add_test(Meme_likes example_meme_likes_tests)

install (TARGETS example_meme_likes_tests DESTINATION ${CMAKE_INSTALL_PREFIX}/examples/meme_likes/test/)
