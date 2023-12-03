# this cmake build system build the opengl-based version of goose64 for desktop platforms

cmake_minimum_required(VERSION 3.12) 

project(goose64 VERSION 0.1.0 LANGUAGES C CXX)

# 1. LOADING CONFIG VARS
 
# extract some more variables from common.makefile (shared with the N64 build system)
# and gldefs.makefile (defines for the opengl build)
# Execute the script and capture its output
execute_process(COMMAND make -f ${CMAKE_SOURCE_DIR}/vars.makefile print-all-vars
                RESULT_VARIABLE print_make_vars_script_result
                OUTPUT_VARIABLE print_make_vars_script_output
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_STRIP_TRAILING_WHITESPACE)
# Check the script result
if(NOT print_make_vars_script_result EQUAL "0")
    message(FATAL_ERROR "Script failed with return code: ${print_make_vars_script_result}")
endif()
# convert the output to a list
string(REPLACE "\n" ";" print_make_vars_line_list "${print_make_vars_script_output}")
# Parse and set the MAKE_ENV variables
foreach(line IN LISTS print_make_vars_line_list)
    # message("Processing line: '${line}'")
    if(line MATCHES "^([^=]+) = (.*)$")
        set(MAKE_ENV_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}")
    endif()
endforeach()

# 2. PROJECT SOURCE FILES

# load crossplat source files 
string(JOIN " " CROSSPLAT_SOURCE_FILES_STRING ${MAKE_ENV_CROSSPLAT_CODEFILES} ${MAKE_ENV_CROSSPLAT_DATAFILES})
# split the string into a list
string(REPLACE " " ";" ALL_SOURCE_FILES "${CROSSPLAT_SOURCE_FILES_STRING}")

# IMGUI source files
set(IMGUI_SOURCE_FILES 
    imgui/examples/imgui_impl_opengl2.cpp
    imgui/examples/imgui_impl_glut.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_draw.cpp
    imgui/imgui.cpp)

file(GLOB GL_CPP_SOURCE_FILES_GLOBBED "gl/*.cpp") 

# GL CPP source files, including the IMGUI sources
set(GL_CPP_SOURCE_FILES 
    ${GL_CPP_SOURCE_FILES_GLOBBED}
    ${IMGUI_SOURCE_FILES})
# GL C source files
set(GL_C_SOURCE_FILES compat.c)

set(ALL_SOURCE_FILES ${ALL_SOURCE_FILES} ${GL_CPP_SOURCE_FILES} ${GL_C_SOURCE_FILES})

# Add files to the executable
add_executable(goose64 ${ALL_SOURCE_FILES})
# Set each source file to be treated as C++
set_source_files_properties(${ALL_SOURCE_FILES} PROPERTIES LANGUAGE CXX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/imgui")

# treat C++ files as C++20
set_property(TARGET goose64 PROPERTY CXX_STANDARD 20)

# 3. DEPENDENCIES

# find GLM (specified in gldefs.makefile)
include_directories("${MAKE_ENV_GLM_INC_PATH}") 

# link opengl
find_package(OpenGL REQUIRED)
target_link_libraries(goose64 ${OPENGL_LIBRARIES})

# link glut
find_package(GLUT REQUIRED)
# add glut include directories
# if not defined then it is not needed for this platform, e.g. macos
if(DEFINED GLUT_INCLUDE_DIRS)
    include_directories(${GLUT_INCLUDE_DIRS})
endif()
target_link_libraries(goose64 ${GLUT_LIBRARY})

# link glew
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories($(GLEW_INCLUDE_DIRS))
endif()
target_link_libraries(goose64 GLEW::GLEW)
