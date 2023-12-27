cmake_minimum_required(VERSION 3.12)
# 使用 vcpkg 工具链文件
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(FFEngine_Project)

set(CMAKE_CXX_STANDARD 17)

# 查找 vcpkg 安装的库
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
message("Found glad: ${glad_VERSION}")
message("Found glfw: ${glfw3_VERSION}")

add_subdirectory(ff)

# 使用 target_link_libraries() 链接库
function(configure_target TARGET_NAME)
    add_executable(${TARGET_NAME} "examples/${TARGET_NAME}.cpp")
    target_link_libraries(${TARGET_NAME} PRIVATE ff_lib glad::glad glfw assimp::assimp)
endfunction()

configure_target(dianosaurScene)
configure_target(triangle)
configure_target(cube)
configure_target(directionalLight)
configure_target(materials)
configure_target(modelLoading)
configure_target(animation)
configure_target(renderTarget)