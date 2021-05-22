#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void constructFigure(float coords[], GLFWwindow* _window, int shader);

//medidas de la pantalla

const unsigned int largo = 800;
const unsigned int ancho = 800;
const char* vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main()
{
    //inicializamos glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creacion de nuestra ventanita hermosa bella de chula
    GLFWwindow* window = glfwCreateWindow(largo, ancho, "DROGAS DROGAS DURAS", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "fallaste, mejor llora XD" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "oye wey no se esta ejecutando el alegre" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error correeeee, drogas drogas shader";
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error correeeee, drogas drogas fragment";
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "errror en el programa, VALIO BERTHA" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    float HEXA[]{
         -0.5f, 0.9f, 0.0f,
         0.5f, 0.9f, 0.0f,
         0.9f, 0.0f, 0.0f,
         0.5f, -0.9f, 0.0f,
        -0.5f, -0.9f, 0.0f,
        -0.9f, 0.0f, 0.0f,


    };
    float triangles[]{
           0.0f, 0.0f, 0.0f,
           0.2f, 0.2f, 0.0f,
           0.0f, -0.2f, 0.0f,

           0.3f, 0.0f, 0.0f,
           0.2f, -0.2f, 0.0f,
           0.5f, -0.2f, 0.0f,

           -0.2f, 0.0f, 0.0f,
           -0.7f, 0.2f, 0.0f,
           -1.0f, -0.2f, 0.0f
    };

    float Cast[]{
        //Base
           -0.9f, 0.0, 0.0f,
           0.9f,  0.0f, 0.0f,
           -0.9f, -0.9f, 0.0f,
           0.9f, 0.0f, 0.0f,
           0.9f, -0.9f, 0.0f,
           -0.9f, -0.9f, 0.0f,

    //torre1
            -0.9f, -0.3f, 0.0f,
           -0.5f, -0.3f, 0.0f,
           -0.9f, 0.6f, 0.0f,
           -0.5f, -0.3f, 0.0f,
           -0.5f, 0.6f, 0.0f,
           -0.9f, 0.6f, 0.0f,

           -1.0f, 0.6f, 0.0f,
           -0.4f, 0.6f, 0.0f,
           -1.0f, 0.9f, 0.0f,
           -0.4f, 0.6f, 0.0f,
           -0.4f, 0.9f, 0.0f,
           -1.0f, 0.9f, 0.0f,

           -0.3f, 0.9f, 0.0f,
           -0.1f, 0.9f, 0.0f,
           -0.2f, 1.0f, 0.0f,

           -0.1f, 0.9f, 0.0f,
           0.1f, 0.9f, 0.0f,
           0.0f, 1.0f, 0.0f,

           0.1f, 0.9f, 0.0f,
           0.3f, 0.9f, 0.0f,
           0.2f, 1.0f, 0.0f,
    //torre2
           -0.2f, -0.3f, 0.0f,
           0.2f, -0.3f, 0.0f,
           -0.2f, 0.6f, 0.0f,
           0.2f, -0.3f, 0.0f,
           0.2f, 0.6f, 0.0f,
           -0.2f, 0.6f, 0.0f,

           -0.3f, 0.6f, 0.0f,
           0.3f, 0.6f, 0.0f,
           -0.3f, 0.9f, 0.0f,
           0.3f, 0.6f, 0.0f,
           0.3f, 0.9f, 0.0f,
           -0.3f, 0.9f, 0.0f,

           -0.3f, 0.9f, 0.0f,
           -0.1f, 0.9f, 0.0f,
           -0.2f, 1.0f, 0.0f,

           -0.1f, 0.9f, 0.0f,
           0.1f, 0.9f, 0.0f,
           0.0f, 1.0f, 0.0f,

           0.1f, 0.9f, 0.0f,
           0.3f, 0.9f, 0.0f,
           0.2f, 1.0f, 0.0f,
      //torre 3
            0.9f, -0.3f, 0.0f,
            0.5f, -0.3f, 0.0f,
            0.9f, 0.6f, 0.0f,
           0.5f, -0.3f, 0.0f,
           0.5f, 0.6f, 0.0f,
           0.9f, 0.6f, 0.0f,

           1.0f, 0.6f, 0.0f,
           0.4f, 0.6f, 0.0f,
           1.0f, 0.9f, 0.0f,
           0.4f, 0.6f, 0.0f,
           0.4f, 0.9f, 0.0f,
           1.0f, 0.9f, 0.0f,


           
    };
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cast), Cast, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //generacion del ciclo de dibujado
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        //color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 120);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
void constructFigure(float coords[], GLFWwindow* _window, int shader)
{
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //generacion del ciclo de dibujado
    while (!glfwWindowShouldClose(_window))
    {
        processInput(_window);
        //color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VAO);
    glDeleteProgram(shader);
    glfwTerminate();
}

    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

