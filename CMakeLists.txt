cmake_minimum_required(VERSION 3.10)
project(first-opengl-program)

# find_package(glfw3 3.4 REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# ===============source files=================
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h)

# ===============source dirs==================
file(GLOB_RECURSE ALL_ITEMS LIST_DIRECTORIES true "src/*")

set(SUBDIRECTORIES "")

foreach(ITEM ${ALL_ITEMS})
    if(IS_DIRECTORY "${ITEM}")
        list(APPEND SUBDIRECTORIES "${ITEM}")
    endif()
endforeach()

# message("Found subdirectories: ${SUBDIRECTORIES}")


add_executable(Hello ${ALL_SOURCE_FILES})
target_link_libraries(Hello
    PUBLIC
        GLEW
        glfw3
        OpenGL
    PRIVATE
        glm::glm-header-only
)

target_include_directories(Hello 
    PUBLIC 
        src 
        ${SUBDIRECTORIES}
)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# find_package(glfw3 3.4 REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(OpenGL 4.5 REQUIRED)

# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# include_directories(/usr/local/include)
# set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# OpenGL::GL
