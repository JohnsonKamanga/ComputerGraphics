#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);


int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(800, 800, "Hello Rectangle", NULL, NULL);

    if(window == NULL){
        cout << "Failed to create glfw window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Fialed to initialize glad" << endl;
        return -1;
    }

    ifstream shaderFile("../vertexShader.vsh");
    string shaderString = "";

    if(!shaderFile.is_open()){
        cout << "Could not open file" << endl;
        return -1;
    }

    while(!shaderFile.eof()){
        string input = "";
        getline(shaderFile, input);
        shaderString += input + "\n";
    }

    shaderFile.close();

    const char * vertexShaderSource = shaderString.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    shaderString = "";

    shaderFile.open("../fragmentShader.fsh");

    if(!shaderFile.is_open()){
        cout << "Failed to open fragment shader" << endl;
        return -1;
    }

    while(!shaderFile.eof()){
        string input = "";
        getline(shaderFile, input);
        shaderString += input + "\n";
    }

    shaderFile.close();

    unsigned int fragementShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char * fragmentShaderSource = shaderString.c_str();

    glShaderSource(fragementShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragementShader);

    glGetShaderiv(fragementShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(fragementShader, 512, NULL, infoLog);
        cout << "ERROR::FRAGMENT::COMPIATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram ,vertexShader);
    glAttachShader(shaderProgram, fragementShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragementShader);

    float vertices[] = {
        0.5f, 0.5f, 0.0f,//top right
        0.5f, -0.5f, 0.0f,//bottom right
        -0.5f, -0.5f, 0.0f,//bottom left
        -0.5f, 0.5f, 0.0f//top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

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