//
// Created by Raiix on 2019/12/3.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//窗口大小改变事件回调函数的定义
void _WindowResizeCallBack(GLFWwindow *win, int w, int h){
    glViewport(0, 0, w, h);
}

int main()
{
    //注意头文件引用的先后顺序
    glfwInit();

    //设置窗口标识
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//glfw大版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//glfw小版本
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//opengl库
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//在mac平台下需要这样设置
#endif

    auto windowHandler = glfwCreateWindow(1280, 800, "GLFW created window", nullptr, nullptr);//创建1280*800大小的窗口，同时设定了窗口标题
    if(!windowHandler){
        glfwTerminate();
        return 0;
    }
    //设定当前线程的绘制在刚刚创建的窗口上进行
    glfwMakeContextCurrent(windowHandler);

    //加载opengl库函数
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        return 0;
    }

    //设置viewport大小
    glViewport(0, 0, 1280, 800);

    //设置窗口大小改变事件回调，当窗口的大小被用户改变时回调用_WindowResizeCallBack函数
    glfwSetFramebufferSizeCallback(windowHandler, _WindowResizeCallBack);

    while(!glfwWindowShouldClose(windowHandler))
    {
        //清屏操作
        glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(windowHandler);
        glfwPollEvents();
    }

    return 0;
}