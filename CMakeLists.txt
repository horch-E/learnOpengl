cmake_minimum_required(VERSION 3.26)
project(learnOpengl)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_H /opt/homebrew/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /opt/homebrew/Cellar/glfw/3.3.8/include)
set(GLAD_H 3rd/glad/include/glad)
set(KH_H 3rd/glad/include)
set(SKIA_H 3rd/skia)
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H} ${SKIA_H})

# 添加目标链接
set(GLEW_LINK  /opt/homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib)
set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.3.8/lib/libglfw.3.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

add_library(mylib STATIC IMPORTED)
set_target_properties(mylib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bin/libskia.a)

# 执行编译命令
set(SOURCE_FILES "3rd/glad/src/glad.c" "main.cpp")
add_executable(learnOpengl ${SOURCE_FILES})
target_link_libraries(learnOpengl PRIVATE mylib $<LINK_LIBRARY:FRAMEWORK,AppKit>)
