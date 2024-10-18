cmake_minimum_required(VERSION 3.29)
project(roguelike)

set(CMAKE_CXX_STANDARD 23)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories(${INCLUDE_DIR})

function(collect_files DIR EXT OUT)
    file(GLOB OUT_FILES "${DIR}/*.${EXT}")
    file(GLOB SUB_DIRS "${DIR}/*")
    foreach(SUB_DIR ${SUB_DIRS})
        if(IS_DIRECTORY ${SUB_DIR})
            collect_files(${SUB_DIR} ${EXT} SUB_FILES)
            list(APPEND OUT_FILES ${SUB_FILES})
        endif()
    endforeach()
    set(${OUT} ${OUT_FILES} PARENT_SCOPE)

    # DEBUG
    list(LENGTH OUT_FILES FILES_LIST_LENGTH)
    message(DEBUG "Collected ${FILES_LIST_LENGTH} files of .${EXT} extension.")
    message(DEBUG "Files: ${OUT_FILES}")
endfunction()

collect_files(${SRC_DIR} "cpp" CPP_SOURCES)
collect_files(${INCLUDE_DIR} "h" H_SOURCES)

add_executable(roguelike ${CPP_SOURCES}
                         ${INCLUDE_SOURCES})
