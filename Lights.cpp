#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "shader.h"
#include "Camera.h"
#include<iostream>
#include <stdlib.h>
#include <time.h>

//#include<ww>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char *path);
void constructFigure(float coords[], GLFWwindow* _window, int shader);
//medidas de la pantalla
int test = 0;
const unsigned int largo = 800;
const unsigned int ancho = 800;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = largo / 2.0f;
float lastY = ancho / 2.0f;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
//posicion de la luz
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int main()
{
    //inicialiamos glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creacion de nuestra ventanita
    GLFWwindow* window = glfwCreateWindow(largo, ancho, "drogas drogas", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "fallaste, date de baja paro" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "oye wey no se esta ejecutando xd" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    //Declarar nuestro programa shader y sus complicaciones
    Shader nuestroShader("luces1.vs", "luces1.fs");
    Shader otroShader("luces2.vs", "luces2.fs");

    //Con texturas
   /* float vertices[] = {
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    0
        0.5f, -0.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    1
       -0.0f, -0.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  2
       -0.0f,  0.5f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  3

        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    4
        0.5f, -0.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    5
       -0.0f, -0.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  6
       -0.0f,  0.5f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  7

        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    8
        0.5f, -0.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    9
       -0.0f, -0.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  10
       -0.0f,  0.5f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  11

        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    12
        0.5f, -0.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    13
       -0.0f, -0.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  14
       -0.0f,  0.5f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  15


    };*/

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f

    }; 

    unsigned int indices[]{
      0,3,2,
      0,2,1,

      4,7,6,
      4,6,5,

      8,11,10,
      8,10,9,

      12,15,14,
      12,14,13,

      16,18,17,
      19,21,20,

      22,25,24,
      22,24,23,

       26,28,27,
       29,31,30,

       32,35,34,
       32,34,33,

       35,38,37,
       35,37,36,

       39,41,40,
       42,44,43,
       //29,34,33,
       //8,11,10,
       //0,3,2,
       //0,2,1,
       //3,7,6,
       //4,5,6,

    };


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    //Unificamos
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Generamos un segundo vertex
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);   
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture("cosa1.png");
    unsigned int specularMap = loadTexture("contenedor.png");

    otroShader.use();
    otroShader.setInt("material.diffuse", 0);
    otroShader.setInt("material.specular", 1);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.5f, 0.7f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        nuestroShader.use();
        nuestroShader.setVec3("light.position",glm::vec3((float)glfwGetTime()/15,1.0f,0));        
        nuestroShader.setVec3("viewPos", cam.Position);

        //propiedades de la luz
        nuestroShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        nuestroShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        nuestroShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);



        /*
        //propiedades de la luz
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);//inflñuencia de luz difusa en nuestra luz
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);//influencia de la luz en el ambiente
        otroShader.setVec3("light.ambient", ambientColor);
        otroShader.setVec3("light.diffuse", diffuseColor);
        otroShader.setVec3("light.specular",1.0f,0.0f,1.0f);
        */

        //propiedades d elos materiales
        otroShader.setVec3("material.ambient", 0.4f,0.2f, 0.0f);
        otroShader.setVec3("material.diffuse", 0.4f,0.2f, 0.0f);
        otroShader.setVec3("material.specular", 0.5f,0.5f, 0.5f);
        otroShader.setFloat("material.shininess", 64.0f);


        //model view projection
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)ancho / (float)largo, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMartrix();
        nuestroShader.setMat4("projection", projection);
        nuestroShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        nuestroShader.setMat4("model", model);

        //linkeo de texturas
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        otroShader.use();
        otroShader.setMat4("projection", projection);
        otroShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(1.0f));
        otroShader.setMat4("model", model);

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void proccessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARDS, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents,0 );
    if(data)
    {
        GLenum format;
        if (nrComponents == 1) {
            format = GL_RED;
        }
        else if(nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if(nrComponents == 3)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else {
        std::cout << "fallo al cargr las texturas" << path << std::endl;
    }
    return textureID;
}