cmake_minimum_required(VERSION 3.20)
project(ff_lib)

file(GLOB_RECURSE FF_FILES ./ *)

foreach(file IN LISTS FF_FILES)
    if(IS_DIRECTORY "${file}")
        continue()
    endif()

    file(RELATIVE_PATH file_rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${file}")
    get_filename_component(file_path "${file_rel_path}" PATH)
    string(REPLACE "/" "\\" file_group_path "${file_path}")
    source_group("${file_group_path}" FILES "${file}")
endforeach()

add_library(${PROJECT_NAME} ${FF_FILES})
