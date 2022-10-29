cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECT_NAME LinkedHashSet)

project(${PROJECT_NAME}
        LANGUAGES C CXX
        )

#execute_process(
#        COMMAND git log -1 --format=%h
#        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
#        OUTPUT_VARIABLE GIT_HASH
#        OUTPUT_STRIP_TRAILING_WHITESPACE
#)

#set(EXEC_NAME ${PROJECT_NAME}_${GIT_HASH})
set(EXEC_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

add_executable(${EXEC_NAME}
        src/main.cpp

        libs/Types/LinkedHashSet.h
        libs/Types/LinkedHashSet.hpp

        libs/Types/Student.h
        libs/Types/Student.cpp

        libs/Types/Path.h
        libs/Types/Path.cpp

        libs/Types/Hasher.h
        libs/Types/Hasher.hpp)

target_compile_options(${EXEC_NAME} PUBLIC -O0)

# Google test implementation ----------------------------------

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(LHStest
        src/tests.cpp

        libs/Types/LinkedHashSet.h
        libs/Types/LinkedHashSet.hpp

        libs/Types/Student.h
        libs/Types/Student.cpp

        libs/Types/Path.h
        libs/Types/Path.cpp

        libs/Types/Hasher.h
        libs/Types/Hasher.hpp)

target_link_libraries(
        LHStest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(LHStest)

# -------------------------------------------------------------

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

target_compile_options(${EXEC_NAME} PRIVATE -fsanitize=address)
target_link_options(${EXEC_NAME} PRIVATE -fsanitize=address)
