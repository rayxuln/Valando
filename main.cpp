//
// Created by Raiix on 2019/12/3.
//

//注意头文件引用的先后顺序
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

std::string GetStringFromFile(std::string path)
{
    using namespace std;
#ifdef __APPLE__
    path = std::string("../Resources/") + path;
#endif
#ifdef _WIN32
    path = std::string("resource/") + path;
#endif
    ifstream in(path);
    return string(
            (istreambuf_iterator<char>(in)),
            istreambuf_iterator<char>()
            );
}

unsigned int LoadImage(std::string path)
{
    stbi_set_flip_vertically_on_load(true);

    int w, h, channels;
    //使用stb_image库来加载图片文件数据
    //w, h 为图片的宽和高
    //channels为颜色分量，比如rgba就为4
    unsigned char *data = stbi_load(path.c_str(), &w, &h, &channels, 0);
    if(!data)
    {
        std::cout<<"Load text("<<path<<") fail."<<std::endl;
        return 0;
    }

    unsigned int tex;
    glGenTextures(1, &tex);//申请一个texture的内存空间，tex储存贴图的id
    glBindTexture(GL_TEXTURE_2D, tex);//绑定tex，用以执行后续操作

    //设置环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置采样方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //判断颜色通道数
    auto colorFormat = GL_RGBA;
    if(channels == 3) colorFormat = GL_RGB;
    else if(channels == 4) colorFormat = GL_RGBA;

    //将生数据data加载到tex中
    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, w, h, 0, colorFormat, GL_UNSIGNED_BYTE, data);
    //生成mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    //是否内存
    stbi_image_free(data);

    return tex;
}

//窗口大小改变事件回调函数的定义
void _WindowResizeCallBack(GLFWwindow *win, int w, int h){
    glViewport(0, 0, w, h);
}

int main()
{
    glfwInit();

    //设置窗口标识
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//glfw大版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//glfw小版本
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//opengl库
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//在mac平台下需要这样设置
#endif

    auto windowHandler = glfwCreateWindow(1280, 800, "Valando - Raiix", nullptr, nullptr);//创建1280*800大小的窗口，同时设定了窗口标题
    if(!windowHandler){
        std::cout<<"create window fail."<<std::endl;
        glfwTerminate();
        return 0;
    }
    //设定当前线程的绘制在刚刚创建的窗口上进行
    glfwMakeContextCurrent(windowHandler);

    //加载opengl库函数
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"gladLoadGLLoader fail."<<std::endl;
        return 0;
    }

    //设置viewport大小
    glViewport(0, 0, 1280, 800);

    //设置窗口大小改变事件回调，当窗口的大小被用户改变时回调用_WindowResizeCallBack函数
    glfwSetFramebufferSizeCallback(windowHandler, _WindowResizeCallBack);

    //Add some vao vbo
    //VAO
    unsigned int vao;
    glGenVertexArrays(1, &vao);//生成一个vao
    glBindVertexArray(vao);//绑定vao

    //VBO
    float vs[] = {//定义四边形的四个点的3d坐标
         //position          //color             //st
         0.5f,  0.5f, 0.0f,  0.1f, 0.5f, 0.3f,  1.0f,  1.0f,  // top right
         0.5f, -0.5f, 0.0f,  0.3f, 0.1f, 0.6f,  1.0f,  0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.9f, 1.0f, 0.4f,  0.0f,  0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  0.4f, 0.6f, 1.0f,  0.0f,  1.0f   // top left
    };
    unsigned int indices[] = {//定义两个三角形所使用的点，索引vs中的顶点
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    unsigned int vbo;//vbo储存所有顶点信息
    glGenBuffers(1, &vbo);//生成一个buffer，并将id储存到vbo中
    glBindBuffer(GL_ARRAY_BUFFER, vbo);//将array buffer绑定到vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);//自动将vs中的顶点数据传输到array buffer所绑定的buffer即vbo中

    //EBO
    unsigned int ebo;//ebo用来储存索引vbo中的信息
    glGenBuffers(1, &ebo);//生成一个buffer，并将id储存到ebo中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);//将element array buffer绑定到ebo中
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//自动将indices中的索引数据传输到element array buffer所绑定的buffer即ebo中

    //linking vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);//取消绑定vao

    //加载着色器
    using std::string;
    //读取顶点着色器源代码
    string verString = GetStringFromFile("shader/tex.vsh");

    unsigned int ver = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器
    auto verCString = verString.c_str();
    glShaderSource(ver, 1, &(verCString), nullptr);//向shader添加源代码
    glCompileShader(ver);//编译shader

    int success;
    char infoLog[512];
    glGetShaderiv(ver, GL_COMPILE_STATUS, &success);//获取是否编译成功
    if(!success)
    {
        glGetShaderInfoLog(ver, 512, nullptr, infoLog);//获取编译信息
        std::cout<<infoLog<<std::endl;
    }

    //fragment shader
    //获取片元着色器源代码
    string fragString = GetStringFromFile("shader/tex.fsh");

    unsigned frag = glCreateShader(GL_FRAGMENT_SHADER);//创建片元着色器
    auto fragCString = fragString.c_str();
    glShaderSource(frag, 1, &(fragCString), nullptr);//向shader添加源代码
    glCompileShader(frag);//编译shader

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);//获取是否编译成功
    if(!success)
    {
        glGetShaderInfoLog(frag, 512, nullptr, infoLog);//获取编译信息
        std::cout<<infoLog<<std::endl;
    }

    //shader program
    unsigned int shaderProgram = glCreateProgram();//创建一个program
    glAttachShader(shaderProgram, ver);//添加顶点着色器
    glAttachShader(shaderProgram, frag);//添加片元着色器
    glLinkProgram(shaderProgram);//执行链接操作

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);//获取是否链接成功
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);//获取链接消息
        std::cout<<infoLog<<std::endl;
    }

    glUseProgram(shaderProgram);//使用program

    glDeleteShader(ver);//释放顶点着色器
    glDeleteShader(frag);//释放片元着色器

    //load texture
    auto tex0 = LoadImage("image/tex.png");
    auto tex1 = LoadImage("image/tex2.png");

    //Bind texture
    glUniform1i(glGetUniformLocation(shaderProgram, "mainTex"), 0);
    glActiveTexture((GL_TEXTURE0));
	glBindTexture(GL_TEXTURE_2D, tex0);

	glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
	glActiveTexture((GL_TEXTURE1));
	glBindTexture(GL_TEXTURE_2D, tex1);

    while(!glfwWindowShouldClose(windowHandler))
    {
        //清屏操作
        glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);//使用之前编译好的shader program
        glBindVertexArray(vao);//绑定vao，之后的绘制函数都将在绑定的vao上进行
        //glDrawArrays(GL_TRIANGLES, 0, 3);//绘制vbo
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//绘制ebo

        glfwSwapBuffers(windowHandler);
        glfwPollEvents();
    }

    return 0;
}