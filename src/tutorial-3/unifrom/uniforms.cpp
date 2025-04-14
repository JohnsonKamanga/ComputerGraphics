#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int draw();

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);

    if (window == NULL)
    {
        cout << "Failed to create glfw window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize glad" << endl;
        return -1;
    }

    // vertex shader file
    ifstream shaderFile("vertexShader.vsh");
    string shaderString = "";

    if (!shaderFile.is_open())
    {
        cout << "Could not open vertex shader" << endl;
        return -1;
    }

    while (!shaderFile.eof())
    {
        string input = "";
        getline(shaderFile, input);
        shaderString += input + "\n";
    }

    shaderFile.close();

    // store vertex shader string as a char pointer
    const char *vertexShaderSource = shaderString.c_str();

    // create a vertex shader object
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // attach shader code to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // compile shader
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // check if vertex shader compilation was successful
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Errorr::SHADER::VERTEX::COMPILATION_FAILED\n"
             << infoLog << endl;
        cout << "vertex shader: " << shaderString << endl;
    }

    shaderString = "";

    shaderFile.open("fragmentShader.fsh");

    if (!shaderFile.is_open())
    {
        cout << "Could not open fragment shader" << endl;
        return -1;
    }

    while (!shaderFile.eof())
    {
        string input = "";
        getline(shaderFile, input);
        shaderString += input + "\n";
    }

    shaderFile.close();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // convert shader file to char pointer
    const char *fragmentShaderSource = shaderString.c_str();

    // attach fragment shader source to to fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // compile fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // check if vertex shader compilation was successful
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "Errorr::SHADER::FRAGMENT::COMPILATION_FAILED\n"
             << infoLog << endl;
        cout << "fragment shader: " << shaderString << endl;
    }

    unsigned int shaderProgram = glCreateProgram();

    // link shader programs to shader program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Errorr::SHADER::PROGRAM::LINKING_FAILED\n"
             << infoLog << endl;
    }

    // delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        0.5f, 0.25f, 0.0f,
        0.0f, 1.0f, 0.0f,
        -0.25f, 0.25f, 0.0f};

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);

    // cout << "vbo value(before binding): " << VBO << endl;

    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // bind VBO to Gl_Array_Buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // cout << "vbo value(after binding): " << VBO << endl;

    // copy vertices data into vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // cout << "vbo value(after copying data to binded vbo): " << VBO << endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        float redValue = (sin(timeValue) / 3.0f) + 0.5f;
        float blueValue = (sin(timeValue) / 7.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");

        glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}