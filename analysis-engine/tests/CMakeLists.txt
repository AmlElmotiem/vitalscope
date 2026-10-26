include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
# Windows: keep our runtime library choice instead of GoogleTest's default.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(vitalscope_tests
    test_stats.cpp
    test_anomaly.cpp
    test_news2.cpp
    test_analysis.cpp
)
target_link_libraries(vitalscope_tests PRIVATE vitalscope_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(vitalscope_tests)
