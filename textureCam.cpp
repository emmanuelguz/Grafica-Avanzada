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
void constructFigure(float coords[], GLFWwindow* _window, int shader);
//medidas de la pantalla
int test = 0;
const unsigned int largo = 800;
const unsigned int ancho = 800;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = largo/2.0f;
float lastY = ancho/2.0f;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
    Shader nuestroShader("texture.vs", "texture.fs");
    

    

    //Declaramos nuestros vertices con sus atributos
    float vertices[] = {        
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

        
    };
    float cubito[] = {
        //Frontal
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,  //Derecha superior    1
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  //Derecha inferior    2
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  //Izquierda inferior  3
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  //Izquierda superior  4

        //Trasera
         0.5f,  0.5f, -1.0f,  1.0f, 1.0f,  //Derecha superior    5
         0.5f, -0.5f, -1.0f,  1.0f, 0.0f,  //Derecha inferior    6
        -0.5f, -0.5f, -1.0f,   0.0f, 0.0f,  //Izquierda inferior  7
        -0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  8

        
    };
  
    float nameVertex[] = {
        //E
         -1.5f,  3.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    0
         -1.5f,  3.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    1
        -3.0f,   3.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  2
        -3.0f,   3.5f, 0.0f,  0.0f, 1.0f, //Izquierda superior  3
        -2.0f,   2.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    4
         -2.0,  2.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    5
        -3.0f,   2.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior 6
        -3.0f,   2.5f, 0.0f,  0.0f, 1.0f, //Izquierda superior  7
        -1.5f,   1.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    8
         -1.5f,  1.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    9
        -3.0f,   1.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  10
        -3.0f,   1.5f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  11
        -3.0f,   3.5f, 0.0f,  1.0f, 1.0f,  //Derecha superior    12
         -3.0f,  1.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    13
        -3.5f,   1.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  14
        -3.5f,   3.5f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  15


        //A
        -0.0f,   3.5f, 0.0f,  1.0f, 1.0f,  //Superior    16         
        -0.7f,   1.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  17
         -0.3f,   1.0, 0.0f,  0.0f, 1.0f,  //Derecha inferior  18

         -0.0f,   3.5f, 0.0f,  1.0f, 1.0f,  //Superior    19         
        0.3f,   1.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  20
         0.7f,   1.0, 0.0f,  0.0f, 1.0f,  //Derecha inferior  21

          0.3f,  2.0f, 0.0f,  1.0f, 1.0f,  //Derecha superior    22
         0.3f,  1.5f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    23
        -0.3f,   1.5f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  24
        -0.3f,   2.0f, 0.0f,  0.0f, 1.0f, //Izquierda superior  25

        //G
         1.5f,   3.5f, 0.0f,  1.0f, 1.0f,  //Superior    26         
         1.8f,   3.2f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  27
         1.0f,   2.0, 0.0f,  0.0f, 1.0f,  //Derecha inferior  28

         1.8f,   1.0f, 0.0f,  1.0f, 1.0f,  //Superior    29
         2.0f,   1.3f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  30
         1.0f,   2.0, 0.0f,  0.0f, 1.0f,  //Derecha inferior  31

         2.1f,  2.0f, 0.0f,  1.0f, 1.0f,  //Derecha superior    32
         2.1f,  1.0f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    33
         1.8f,   1.0f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  34
         1.8f,   2.0f, 0.0f,  0.0f, 1.0f, //Izquierda superior  35

         
         2.1f,  1.8f, 0.0f,  1.0f, 0.0f,  //Derecha inferior    36
         1.5f,   1.8f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  37
         1.5f,   2.0f, 0.0f,  0.0f, 1.0f, //Izquierda superior  38

         //C
         3.5f,   3.5f, 0.0f,  1.0f, 1.0f,  //Superior    39
         3.8f,   3.2f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  40
         3.0f,   2.0, 0.0f,  0.0f, 1.0f,  //Derecha inferior  41

         3.8f,   1.0f, 0.0f,  1.0f, 1.0f,  //Superior    42
         4.0f,   1.3f, 0.0f,  0.0f, 0.0f,  //Izquierda inferior  43
         3.0f,   2.0, 0.0f,  0.0f, 1.0f,  //Derecha inferior  44

         
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

    

    /*
        weird
        unsigned int indices[]{
       0,3,2,
       //0,2,1,
       0,5,6,
       /*5,1,6
        4,7,6,
        4,6,5


};
    */

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*//atributos de color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/
    //atributos de nuestras texturas
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //Creamos y cargamos las texturas
    //-------------------------------
    unsigned int textura1, textura2;
    //Configurar nuestra primera textura
    glGenTextures(1, &textura1);
    glBindTexture(GL_TEXTURE_2D, textura1);
    //Configurar como queremos que actue nuestra mascara de textura en nuestro objeto
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y
    //Configuramos el tipo de filtrado que va a manejar nuestra textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //Cargar nuestra textura
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(("dvd.png"), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "fallaste al cargar la primera textura" << std::endl;
    }
    stbi_image_free(data);

    //Textura 2
    //Configurar nuestra segunda textura
    glGenTextures(1, &textura2);
    glBindTexture(GL_TEXTURE_2D, textura2);
    //Configurar como queremos que actue nuestra mascara de textura en nuestro objeto
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y
    //Configuramos el tipo de filtrado que va a manejar nuestra textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //Cargar nuestra textura
    data = stbi_load(("cosa1.png"), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "fallaste al cargar la segunda textura" << std::endl;
    }
    stbi_image_free(data);

    //Empezamos a usar nuestra libreria shader
    nuestroShader.use();
    
    glUniform1i(glGetUniformLocation(nuestroShader.ID, "textura1"), 0);
    
    nuestroShader.setInt("textura1", 0);
    //nuestroShader.setInt("textura2", 1);

    
    //loop de dibujado
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

     
        
        //Renderizado
        glClearColor(0.05f,  0.06f, .08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //unificar nuestras texturas y activarlas
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textura1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textura2);

        nuestroShader.use();
        
        glm::mat4 name = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model2 = glm::mat4(1.0f);
        glm::mat4 model3 = glm::mat4(1.0f);
        glm::mat4 model4 = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        float move = 0 + (float)glfwGetTime();
        
        
        projection = glm::perspective(glm::radians(cam.Zoom),(float)largo/(float)ancho,0.1f,100.0f);
        
        view = cam.GetViewMartrix();
       // name = glm::scale(name, glm::vec3(5.3f - sin((move / 2.5))   , 5.3f - (move / 2.5), -0.0f));
        //name = glm::rotate(name, move * 2, glm::vec3(1.0f, 0.0f, 1.0f));
        
        //-2.1f  1.6f

        
        
        float auxX = 0 + (float)glfwGetTime();
        float auxY = 0 + (float)glfwGetTime();
        


        if (auxX + 0.5f >= 1.6f) {
            
            auxX -= (float)glfwGetTime();
            name = glm::translate(name, glm::vec3(auxX, 0, 0.0f));
        }
        else if (auxX <= -2.1)
        {
            auxX += (float)glfwGetTime();
            name = glm::translate(name, glm::vec3(auxX, 0, 0.0f));
        }
        unsigned int modelLoc = glGetUniformLocation(nuestroShader.ID, "model");
    
        unsigned int viewlLoc = glGetUniformLocation(nuestroShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(name));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
        glUniformMatrix4fv(viewlLoc, 1, GL_FALSE, &view[0][0]);
        nuestroShader.setMat4("projection", projection);
        
        glBindVertexArray(VAO);
        /*float move = 0 +(float)glfwGetTime();       
        
        if (move < 10.3f) {

            model = glm::translate(model, glm::vec3(-5.3f +(move/2), 0.0f, 0.0f));
            model = glm::rotate(model, move * 2, glm::vec3(0.0f, 1.0f, 0.0f));
            model3 = glm::translate(model3, glm::vec3(-0.3f, -5.3f + (move / 2), -0.0f));
            model3 = glm::scale(model3, glm::vec3(5.3f - (move / 2.5), 5.3f - (move / 2.5), -0.0f));


            model4 = glm::translate(model4, glm::vec3(5.3f -(move/2), 0.0f, 0.0f));
            model4 = glm::rotate(model4, move * -2, glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else if (move > 10.3f && move < 15.0f)
        {
           model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f));
           model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
           model3 = glm::translate(model3, glm::vec3(-0.3f, -0.0f, -0.0f));           
           model3 = glm::scale(model3, glm::vec3(1+(move-10.3), 1 + (move-10.3), -0.0f));
           model4 = glm::translate(model4, glm::vec3(0.2f, 0.0f, 0.0f));
           model4 = glm::rotate(model4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        

        nuestroShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        nuestroShader.setMat4("model", model4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

         
            
        glm::mat4 arr[] = {
        model,
        model3,
        
        model4
        };
        
        
        /*for (int i = 0; i < 3; i++) {
            nuestroShader.setMat4("model", arr[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
           
        }*/
        nuestroShader.setMat4("model", name);
        glDrawElements(GL_TRIANGLES, 70, GL_UNSIGNED_INT, 0);

       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void proccessInput(GLFWwindow* window)
{
    
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

    cam.ProcessMouseMovement(xoffset,yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.ProcessMouseScroll(yoffset);
}