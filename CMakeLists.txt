cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECT_NAME LinkedHashSet)

project(${PROJECT_NAME}
        LANGUAGES C CXX
        )

execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(EXEC_NAME ${PROJECT_NAME}_${GIT_HASH})

set(CMAKE_CXX_STANDARD 14)

add_executable(${EXEC_NAME}
        src/main.cpp

        libs/Helpers/HashUtils.cpp
        libs/Helpers/HashUtils.h

        libs/Types/LinkedHashSet.h
        libs/Types/LinkedHashSet.cpp
        libs/Types/Student.h
        libs/Types/Student.cpp

        libs/Types/Pair.cpp
        libs/Types/Pair.h

        libs/Helpers/LoadPrivateDataGTest.cpp
        libs/Helpers/LoadPrivateDataGTest.h
        )

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

        libs/Helpers/HashUtils.cpp
        libs/Helpers/HashUtils.h

        libs/Types/LinkedHashSet.h
        libs/Types/LinkedHashSet.cpp
        libs/Types/Student.h
        libs/Types/Student.cpp
        libs/Types/Pair.cpp
        libs/Types/Pair.h

        libs/Helpers/LoadPrivateDataGTest.cpp
        libs/Helpers/LoadPrivateDataGTest.h
        )

target_link_libraries(
        LHStest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(LHStest)

# -------------------------------------------------------------

