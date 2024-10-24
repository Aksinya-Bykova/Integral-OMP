cmake_minimum_required(VERSION 3.22)
project(omp_custom)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

OPTION(USE_OpenMP "Use OpenMP" ON)
IF (USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF (OPENMP_FOUND)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    ENDIF ()
ENDIF ()

include(FetchContent)

# json lib
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

include_directories(include research)

#set(CACHE_LINE_SIZE 64)
#
#if (UNIX)
#    execute_process(
#            COMMAND getconf LEVEL1_DCACHE_LINESIZE
#            OUTPUT_VARIABLE CACHE_LINE_SIZE
#            OUTPUT_STRIP_TRAILING_WHITESPACE
#            ERROR_VARIABLE GETCONF_ERROR
#    )
#endif ()

add_executable(omp_custom main.cpp hit.cpp)

#target_compile_definitions(omp_custom PRIVATE CACHE_LINE_SIZE=${CACHE_LINE_SIZE})
