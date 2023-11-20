//
// Created by huwj on 2023/11/20.
//
#include "glad.h"
#include <GLFW//glfw3.h>
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkCanvas.h"
#include "include/gpu/gl/GrGLAssembleInterface.h"
#include "include/core/SkColorSpace.h"
#include "Window.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
//    glViewport(0,0,width,height);
    printf("%d,%d\n", width, height);
}

Window::Window(int width, int height) : m_width(width), m_height(height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(m_width, m_height, "LearnOPenGL", nullptr, nullptr);
    if (m_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    initSkia();


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to innitialize GLAD" << std::endl;
        return;
    }

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glViewport(0, 0, m_width, m_height);
}

void Window::update() {
    while (!glfwWindowShouldClose(m_window)) {
        m_surface->getCanvas()->clear(SK_ColorBLUE);
        m_context->flush();
        processInput(m_window);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Window::initSkia() {
    auto interface = GrGLMakeNativeInterface();
    if (interface == nullptr) {
        //backup plan. see https://gist.github.com/ad8e/dd150b775ae6aa4d5cf1a092e4713add?permalink_comment_id=4680136#gistcomment-4680136
        interface = GrGLMakeAssembledInterface(
                nullptr,
                (GrGLGetProc) *[](void *, const char *p) -> void * { return (void *) glfwGetProcAddress(p); });
    }
    m_context = GrDirectContexts::MakeGL(interface).release();

    GrGLFramebufferInfo framebufferInfo;
    framebufferInfo.fFBOID = 0; // assume default framebuffer
    // We are always using OpenGL and we use RGBA8 internal format for both RGBA and BGRA configs in OpenGL.
    //(replace line below with this one to enable correct color spaces) framebufferInfo.fFormat = GL_SRGB8_ALPHA8;
    framebufferInfo.fFormat = GL_RGBA8;

    SkColorType colorType = kRGBA_8888_SkColorType;
    GrBackendRenderTarget backendRenderTarget = GrBackendRenderTargets::MakeGL(m_width, m_height,
                                                                               0, // sample count
                                                                               0, // stencil bits
                                                                               framebufferInfo);

    //(replace line below with this one to enable correct color spaces) sSurface = SkSurfaces::WrapBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();
    m_surface = SkSurfaces::WrapBackendRenderTarget(m_context, backendRenderTarget, kBottomLeft_GrSurfaceOrigin,
                                                    colorType, nullptr, nullptr).release();
}

void Window::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
