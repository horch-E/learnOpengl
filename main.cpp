#include <glad/glad.h>
#include <GLFW//glfw3.h>

#define SK_GANESH
#define SK_GL
#include <iostream>
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

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
//    glViewport(0,0,width,height);
    printf("%d,%d\n",width,height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, const char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOPenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    auto interface = GrGLMakeNativeInterface();
    if (interface == nullptr) {
        //backup plan. see https://gist.github.com/ad8e/dd150b775ae6aa4d5cf1a092e4713add?permalink_comment_id=4680136#gistcomment-4680136
        interface = GrGLMakeAssembledInterface(
                nullptr,
                (GrGLGetProc) *[](void *, const char *p) -> void * { return (void *) glfwGetProcAddress(p); });
    }
    auto sContext = GrDirectContexts::MakeGL(interface).release();

    GrGLFramebufferInfo framebufferInfo;
    framebufferInfo.fFBOID = 0; // assume default framebuffer
    // We are always using OpenGL and we use RGBA8 internal format for both RGBA and BGRA configs in OpenGL.
    //(replace line below with this one to enable correct color spaces) framebufferInfo.fFormat = GL_SRGB8_ALPHA8;
    framebufferInfo.fFormat = GL_RGBA8;

    SkColorType colorType = kRGBA_8888_SkColorType;
    GrBackendRenderTarget backendRenderTarget = GrBackendRenderTargets::MakeGL(SCR_WIDTH, SCR_HEIGHT,
                                                                               0, // sample count
                                                                               0, // stencil bits
                                                                               framebufferInfo);

    //(replace line below with this one to enable correct color spaces) sSurface = SkSurfaces::WrapBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();
    auto sSurface = SkSurfaces::WrapBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin,
                                                        colorType, nullptr, nullptr).release();
    if (sSurface == nullptr)
        std::cout << "sSurface fail" << std::endl;


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to innitialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        sSurface->getCanvas()->clear(SK_ColorBLUE);
        sContext->flush();
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}