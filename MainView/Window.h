//
// Created by huwj on 2023/11/20.
//

#ifndef LEARNOPENGL_WINDOW_H
#define LEARNOPENGL_WINDOW_H

#include <iostream>

class GLFWwindow;

class GrDirectContext;

class SkSurface;

class Window {
public:
    Window(int width, int height);

    void update();

private:
    void initSkia();

    static void processInput(GLFWwindow *window);

    int m_width{};
    int m_height{};
    GLFWwindow *m_window;
    GrDirectContext *m_context;
    SkSurface *m_surface;
};


#endif //LEARNOPENGL_WINDOW_H
