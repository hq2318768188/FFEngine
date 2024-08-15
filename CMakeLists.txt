cmake_minimum_required(VERSION 3.12)

# 使用 vcpkg 工具链文件
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_GENERATOR_PLATFORM x64)

project(FFEngine_Project)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug) 

# 查找 vcpkg 安装的库
find_package(glad CONFIG REQUIRED) 
find_package(glfw3 CONFIG REQUIRED) 
find_package(assimp CONFIG REQUIRED)
message("Found glad: ${glad_VERSION}")
message("Found glfw: ${glfw3_VERSION}") 

add_subdirectory(ff)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CommonTools)
include(UseDoxygenDoc)

configure_target(dianosaurScene)
configure_target(triangle)
configure_target(cube)
configure_target(directionalLight)
configure_target(materials)
configure_target(modelLoading)
configure_target(animation)
configure_target(renderTarget)

add_doxygen_doc(
  BUILD_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/docs
  DOXY_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
  TARGET_NAME
    docs
  COMMENT
    "HTML documentation"
  )