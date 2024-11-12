# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in common

#find_package(GTest REQUIRED)

add_executable(utest-common
        binario_test.cpp
        progargs_test.cpp
)
# Library dependencies
target_link_libraries (utest-common
        PRIVATE
        common
        GTest::gtest_main
        gmock
        GTest::gmock_main
        Microsoft.GSL::GSL)