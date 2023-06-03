cmake_minimum_required (VERSION 3.8)

project(clw-04)

SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wextra-semi -O1 -g -fsanitize=address -fno-omit-frame-pointer")


add_executable(test smoke_test.cpp)
add_executable(test1 tests/comp_include_once.cpp)
add_executable(test2 tests/comp_include_twice.cpp)
add_executable(test3 tests/comp_using_namespace_std.cpp)
add_executable(test4 tests/ncomp_ctor_string_explicit.cpp)
add_executable(test5 tests/ncomp_operator_bool_implicit.cpp)
add_executable(test6 tests/run_bignum_assignment_operator_return_type.cpp)
add_executable(test7 tests/run_bignum_binary_operators_with_conversion.cpp)
add_executable(test8 tests/run_polynomial.cpp)
add_executable(test9 tests/run_bignum_zero_multiplication.cpp)
add_executable(test10 tests/run_horner.cpp)
add_executable(test11 tests/run_long_bignum_bool.cpp)
add_executable(test12 tests/run_long_bignums.cpp)











