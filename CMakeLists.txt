cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(PROJECT_NAME LinkedHashSet)

project( ${PROJECT_NAME}
    LANGUAGES C CXX
)

execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(EXEC_NAME ${PROJECT_NAME}_${GIT_HASH})


add_executable( ${EXEC_NAME} 
    src/main.cpp
    
    libs/Helpers/HashUtils.cpp
    libs/Helpers/HashUtils.h

    libs/Types/LinkedHashSet.h
    libs/Types/LinkedHashSet.cpp
    libs/Types/Student.h
    libs/Types/Student.cpp
)

