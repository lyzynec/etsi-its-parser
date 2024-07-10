cmake_minimum_required(VERSION 3.25)

project(etsi_its_parser)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS TRUE)

include(CMakeParseArguments)

function(check_git_submodule)
    set(single_args PATH)
    set(multi_args REQUIRED_FILES)
    cmake_parse_arguments(args "" "${single_args}" "${multi_args}" ${ARGN})

    set(missing_files "")
    foreach(required_file IN LISTS args_REQUIRED_FILES)
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${args_PATH}/${required_file})
            list(APPEND missing_files ${required_file})
        endif()
    endforeach()

    if(missing_files)
        string(REPLACE ";" ", " missing_files "${missing_files}")
        message(FATAL_ERROR
                "Git submodule at ${args_PATH} is incomplete (missing: ${missing_files})! Please fetch git submodules and try again.")
    endif()
endfunction()

add_executable(etsi_its_parser main.cpp
        headers.h
        indicator.cpp
        indicator.h
)

check_git_submodule(PATH vanetza REQUIRED_FILES CMakeLists.txt)
set(VANETZA_INSTALL ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/vanetza)

mark_as_advanced(BUILD_BENCHMARK BUILD_CERTIFY BUILD_SOCKTAP BUILD_TESTS BUILD_USING_CONAN)
mark_as_advanced(VANETZA_ASN1_WITH_ASN1C VANETZA_EXPORT_PACKAGE VANETZA_NET_WITH_POSIX)

target_link_libraries(etsi_its_parser PUBLIC Vanetza::vanetza)

#check_git_submodule(PATH rapidjson REQUIRED_FILES CMakeLists.txt)
#add_subdirectory(${PROJECT_SOURCE_DIR}/rapidjson)

#check_git_submodule(PATH PcapPlusPlus REQUIRED_FILES CMakeLists.txt)
#add_subdirectory(${PROJECT_SOURCE_DIR}/PcapPlusPlus)
