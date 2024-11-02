# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in img−aos
add_executable(utest-img-soa
        one_test.cpp
        test_soa_1.cpp
        test_soa_2.cpp)
# Library dependencies
target_link_libraries (utest-img-soa
        PRIVATE
        img−soa
        GTest::gtest_main
        Microsoft.GSL::GSL)