#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Camera.h"

#include <iostream>
#include <vector>
using namespace std;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
unsigned int loadCubeMap(vector<string> faces);
void BuildCube(float _ancho, float _alto, float largo, Shader _objShader, unsigned int _VBO, unsigned int _VAO, unsigned int _EBO);
void DrawCube(Shader _objShader, unsigned int _VAO, glm::vec3 pos, float angles, int axis, unsigned int _diffuseMap, unsigned int _specularMap);
//medidas de la pantalla
const unsigned int largo = 800;
const unsigned int ancho = 800;

//configuraciones para nuestra camara
Camara cam(glm::vec3(5.0f, 0.0f, -30.0f));
float lastX = ancho / 2.0f;
float lastY = largo / 2.0f;
bool firstMouse = true;


//timing
float deltaTime = 0.0f; //marca el tiempo transcurrido entre el frame mas actual y el anterior
float lastFrame = 0.0f;
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
        std::cout << "fallaste hijo de toda tu putisima madre" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //le indicamos a glfw que reciba datos del nuestro mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "oye wey no se esta ejecutando xd" << std::endl;
        return -1;
    }
    //comprobacion del buffer de profundidad en el eje z
    glEnable(GL_DEPTH_TEST);
    //Declarar nuestro programa shader y sus complicaciones
    Shader nuestroShader("luces1.vs", "luces1.fs"); //nuestro cubo que va a recibir la luz
    Shader otroShader("luces2.vs", "luces2.fs"); //el origen de la luz
    Shader skyboxShader("skybox.vs", "skybox.fs"); //el origen de la luz

    

   /* //Declaramos nuestros vertices con sus atributos
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

        
    };*/
    /*float verticesTri[] = {
        0.0f,0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.0f,1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.5f,  1.0f,
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
    float skybox []= {
        -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

    /*float cubito[] = {
        //Frontal
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,  //Derecha superior    0
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  //Derecha inferior    1
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  //Izquierda inferior  2
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  //Izquierda superior  3

        //Trasera
         0.5f,  0.5f, -1.0f,  1.0f, 1.0f,  //Derecha superior    4
         0.5f, -0.5f, -1.0f,  1.0f, 0.0f,  //Derecha inferior    5
        -0.5f, -0.5f, -1.0f,   0.0f, 0.0f,  //Izquierda inferior  6
        -0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  //Izquierda superior  7

        
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

    /*unsigned int indices[]{
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

    };*/

    unsigned int indices[]{
      
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        14, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };

    

    /*float _ancho = 5, _alto = 5, _largo = 5;
    float vertex[] = {
        ////--X--/////////--Y--///////--Z--
         0.0f,          0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.0f + _ancho, 0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f,          0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
         0.0f,          0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

         ////--X--/////////--Y--/////////--Z--
         0.0f,          0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.0f + _ancho, 0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f,          0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
         0.0f,          0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

         //--X--/////--Y--/////////--Z--
           0.0f,  0.0f + _alto, 0.0f + _largo, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           0.0f,  0.0f + _alto, 0.0f,         -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
           0.0f,  0.0f,         0.0f,         -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f,  0.0f,         0.0f,         -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f,  0.0f,         0.0f + _largo, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
           0.0f,  0.0f + _alto, 0.0f + _largo, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

           ///--X--///////////--Y--/////////--Z--
           0.0f + _ancho,  0.5f + _alto, 0.5f + _largo,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           0.0f + _ancho,  0.5f + _alto, 0.0f,          1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
           0.0f + _ancho,  0.0f,         0.0f,          1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho,  0.0f,         0.0f,          1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho,  0.0f,         0.5f + _largo,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
           0.0f + _ancho,  0.5f + _alto, 0.5f + _largo,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

           ///--X--///////--Y--//////--Z--
           0.0f,          0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho, 0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
           0.0f + _ancho, 0.0f,  0.5f + _largo,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
           0.0f + _ancho, 0.0f,  0.5f + _largo,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
           0.0f,          0.0f,  0.5f + _largo,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
           0.0f,          0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

           ///--X--//////////--Y--/////////--Z--
           0.0f,          0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho, 0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
           0.0f + _ancho, 0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
           0.0f + _ancho, 0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
           0.0f,          0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
           0.0f,          0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    */
    glm::vec3 posicionesCubo[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    unsigned int skybxVBO, skybxVAO;
    glGenVertexArrays(1, &skybxVAO);
    glGenBuffers(1, &skybxVBO);
    glBindVertexArray(skybxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skybxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), &skybox, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<string> skyboxFaces
    {
        ("skybox/right.jpg"),
        ("skybox/left.jpg"),
        ("skybox/top.jpg"),
        ("skybox/bottom.jpg"),
        ("skybox/front.jpg"),
        ("skybox/back.jpg")

    };
    unsigned int VBO, VAO, EBO;    
    unsigned int VBO2, VAO2;
    unsigned int VBO3, VAO3;
    unsigned int VBO4, VAO4;
    unsigned int VBO5, VAO5;
    unsigned int VBO6, VAO6;
    unsigned int VBO7, VAO7;
    unsigned int VBO8, VAO8;
    unsigned int VBO9, VAO9;
    unsigned int VBO10, VAO10;
    unsigned int VBO11, VAO11;
    unsigned int VBO12, VAO12;
    unsigned int VBO13, VAO13;
    unsigned int VBO14, VAO14;
    unsigned int VBO15, VAO15;
    unsigned int VBO16, VAO16;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);    

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glGenVertexArrays(1, &VAO6);
    glGenBuffers(1, &VBO6);
    glGenVertexArrays(1, &VAO7);
    glGenBuffers(1, &VBO7);
    glGenVertexArrays(1, &VAO8);
    glGenBuffers(1, &VBO8);
    glGenVertexArrays(1, &VAO9);
    glGenBuffers(1, &VBO9);
    glGenVertexArrays(1, &VAO10);
    glGenBuffers(1, &VBO10);
    glGenVertexArrays(1, &VAO11);
    glGenBuffers(1, &VBO11);
    glGenVertexArrays(1, &VAO12);
    glGenBuffers(1, &VBO12);
    glGenVertexArrays(1, &VAO13);
    glGenBuffers(1, &VBO13);
    glGenVertexArrays(1, &VAO14);
    glGenBuffers(1, &VBO14);
    glGenVertexArrays(1, &VAO15);
    glGenBuffers(1, &VBO15);
    glGenVertexArrays(1, &VAO16);
    glGenBuffers(1, &VBO16);
    
    
    unsigned int diffuseMap = loadTexture("ghost.jpg");    
    unsigned int chairrMap = loadTexture("woodwall.jpg");
    unsigned int specularMap = loadTexture("wood2.jpg");
    unsigned int brickMap = loadTexture("brick.jpg");
    unsigned int marbleMap = loadTexture("marble.jpg");
    unsigned int marfilMap = loadTexture("marfil.jpg");
    unsigned int barMap = loadTexture("descarga.jpg");
    unsigned int roofMap = loadTexture("tejas.jpg");

    ////Piso
    BuildCube(75.0f, 0.5f, 55.0f, nuestroShader, VBO, VAO, EBO);

    ////Paredes
    BuildCube(35.0f, 35.0f, 1.0f, nuestroShader, VBO2, VAO2, EBO);
    BuildCube(55.0f, 35.0f, 1.0f, nuestroShader, VBO9, VAO9, EBO);

        //Entrada
        BuildCube(35.0f, 25.0f, 1.0f, nuestroShader, VBO3, VAO3, EBO);        
        BuildCube(15.0f,  10.0f, 1.0f, nuestroShader, VBO4, VAO4, EBO);
    
    BuildCube(10.0f, 25.0f, 1.0f, nuestroShader, VBO15, VAO15, EBO);
    ////Altar
        BuildCube(22.0f, 0.5f, 10.0f, nuestroShader, VBO5, VAO5, EBO);
        BuildCube(5.0f, 1.5f, 3.0f, nuestroShader, VBO6, VAO6, EBO);
        BuildCube(4.0f, 2.0f, 1.5f, nuestroShader, VBO7, VAO7, EBO);

        //Sillas
        BuildCube(7.5f, 2.0f, 0.5f, nuestroShader, VBO8, VAO8, EBO);

        //Silla2
        BuildCube(2.5f, 2.0f, 0.5f, nuestroShader, VBO10, VAO10, EBO);

        //Mesas
        BuildCube(3.5f, 0.2f, 3.5f, nuestroShader, VBO11, VAO11, EBO);
        BuildCube(0.5f, 3.0f, 0.5f, nuestroShader, VBO12, VAO12, EBO);

    ///Barandal
        BuildCube(8.0f, 3.0f, 0.5f, nuestroShader, VBO13, VAO13, EBO);
        BuildCube(7.0f, 3.0f, 0.5f, nuestroShader, VBO14, VAO14, EBO);

     //Techo
        BuildCube(1.0f, 35.0f, 56.0f, nuestroShader, VBO16, VAO16, EBO);
    unsigned int cubemapTexture = loadCubeMap(skyboxFaces);
    skyboxShader.use();
    skyboxShader.setInt("skybox",0);
   /* unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    

    //Unificamos
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //atributos de posicion 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //atributos de colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //attribs de testuras :v
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /*
    ///////Generacion de texturas clasica////////////////////////////
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
    unsigned char* data = stbi_load(("lavaTexture.jpg"), &width, &height, &nrChannels, 0);
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
    nuestroShader.setInt("textura2", 1);
    */

    /*//generamos un segundo vertex 
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //TRingle

    unsigned int VAOT,VBOT, EBOT;
    glGenVertexArrays(1, &VAOT);    
    glGenBuffers(1, &EBOT);
    glGenBuffers(1, &VBOT);
    glBindVertexArray(VAOT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOT);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOT);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTri),verticesTri,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //atributos de posicion 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //atributos de colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //attribs de testuras :v
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    nuestroShader.use();
    nuestroShader.setInt("material.diffuse", 1);
    nuestroShader.setInt("material.specular", 0);*/

/*

unsigned int indices[]{

   0, 1, 2,
   3, 4, 5,
   6, 7, 8,
   9, 10, 11,
   12, 13, 14,
   14, 16, 17,
   18, 19, 20,
   21, 22, 23,
   24, 25, 26,

};*/

   /* 
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

    //atributos de posicion 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //atributos de colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //attribs de testuras :v
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    nuestroShader.use();
    nuestroShader.setInt("material.diffuse", 1);
    nuestroShader.setInt("material.specular", 0);*/
    
    //loop de dibujado
    while (!glfwWindowShouldClose(window))
    {
        
        // carcular el tiempo entre los frames para poder implementar le movimiento de la camara
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //entradas
        processInput(window);
        
        //Renderizado
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*//activar las caracteristicas de luz en nuestro objeto
        nuestroShader.use();
        nuestroShader.setVec3("light.position", cam.Position);
        nuestroShader.setVec3("light.direction", cam.Front);
        nuestroShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        nuestroShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        nuestroShader.setVec3("viewPos", cam.Position);

        //propiedades de la luz
        nuestroShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        nuestroShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        nuestroShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        nuestroShader.setFloat("light.constant", 1.0f);
        nuestroShader.setFloat("light.linear", 0.09f);
        nuestroShader.setFloat("light.quadratic", 0.032f);

        //propiedades de los materiales
        nuestroShader.setFloat("material.shininess", 32.0f); */
     
       //linkeamos nuestras textr¿uras
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        //render del cubo
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, posicionesCubo[i]);
        //float angle = 20.0f * i;
        //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        nuestroShader.setMat4("model", model);


        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
        glm::vec3 pos(0, 0, 0);
        ////Piso
        DrawCube(nuestroShader, VAO, glm::vec3(0.0f, -7.5f, -10), 0, 0, diffuseMap, chairrMap);

        ////Paredes
        DrawCube(nuestroShader, VAO2, glm::vec3(0.0f, -7.5f, 45), 0, 0, diffuseMap, brickMap);
            //Entrada
            DrawCube(nuestroShader, VAO3, glm::vec3(0.0f, 3.0f, -10.0f), 0, 0, diffuseMap, brickMap);
            DrawCube(nuestroShader, VAO4, glm::vec3(20.0f, -7.0f, -10.0f), 0, 0, diffuseMap, brickMap);
            DrawCube(nuestroShader, VAO4, glm::vec3(0.0f, -7.0f, -10.0f), 0, 0, diffuseMap, brickMap);

            //Costados
            DrawCube(nuestroShader, VAO9, glm::vec3(0.0f, -7.5f, 45.0f), 90, 1, diffuseMap, brickMap);
            DrawCube(nuestroShader, VAO2, glm::vec3(35.0f, -7.5f, 35.0f), 90, 1, diffuseMap, brickMap);

            DrawCube(nuestroShader, VAO15, glm::vec3(35.0f, 2.50f, 45.0f), 90, 1, diffuseMap, brickMap);
            DrawCube(nuestroShader, VAO15, glm::vec3(35.0f, 2.50f, 0.0f), 90, 1, diffuseMap, brickMap);
        ////Altar
            DrawCube(nuestroShader, VAO5, glm::vec3(7.0f, -7.0f, 30.0f), 0, 0, diffuseMap, marbleMap);
            DrawCube(nuestroShader, VAO6, glm::vec3(15.0f, -4.5f, 32.5f), 0, 0, diffuseMap, marbleMap);
            DrawCube(nuestroShader, VAO7, glm::vec3(15.5f, -6.5f, 33.25f), 0, 0, diffuseMap, marbleMap);
#pragma region Props
            //Sillas           
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, -3.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, -5.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 4.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 2.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 9.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 9.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 11.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 9.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 15.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 15.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 17.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 15.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 22.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 22.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -7.0f, 24.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(3.0f, -5.0f, 22.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, -3.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, -5.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 4.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 2.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 9.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 9.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 11.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 9.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 15.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 15.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 17.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 15.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 22.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 22.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -7.0f, 24.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO8, glm::vec3(25.0f, -5.0f, 22.0f), 90, 0, diffuseMap, specularMap);

            //Terraza
                   //Mesa
            DrawCube(nuestroShader, VAO11, glm::vec3(44.5f, -4.0f, -3.0f), 0, 0, diffuseMap, marfilMap);
            DrawCube(nuestroShader, VAO12, glm::vec3(46.0f, -7.0f, -1.5f), 0, 0, diffuseMap, specularMap);

            //Sillas 
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, -3.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, -5.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, 0.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, 0.0f), 90, 0, diffuseMap, specularMap);

            //Mesa
            DrawCube(nuestroShader, VAO11, glm::vec3(44.5f, -4.0f, 15.0f), 0, 0, diffuseMap, marfilMap);
            DrawCube(nuestroShader, VAO12, glm::vec3(46.0f, -7.0f, 16.5f), 0, 0, diffuseMap, specularMap);

            //Sillas 
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, 13.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, 13.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, 15.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, 13.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, 20.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, 20.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -7.0f, 18.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(45.0f, -5.0f, 18.0f), 90, 0, diffuseMap, specularMap);


            //Mesa
            DrawCube(nuestroShader, VAO11, glm::vec3(64.5f, -4.0f, -3.0f), 0, 0, diffuseMap, marfilMap);
            DrawCube(nuestroShader, VAO12, glm::vec3(66.0f, -7.0f, -1.5f), 0, 0, diffuseMap, specularMap);

            //Sillas 
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, -5.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, -3.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, -5.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, 2.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, 0.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, 0.0f), 90, 0, diffuseMap, specularMap);

            //Mesa
            DrawCube(nuestroShader, VAO11, glm::vec3(64.5f, -4.0f, 15.0f), 0, 0, diffuseMap, marfilMap);
            DrawCube(nuestroShader, VAO12, glm::vec3(66.0f, -7.0f, 16.5f), 0, 0, diffuseMap, specularMap);

            //Sillas 
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, 13.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, 13.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, 15.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, 13.0f), 90, 0, diffuseMap, specularMap);

            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, 20.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, 20.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -7.0f, 18.0f), 0, 0, diffuseMap, specularMap);
            DrawCube(nuestroShader, VAO10, glm::vec3(65.0f, -5.0f, 18.0f), 90, 0, diffuseMap, specularMap);

            //Barandar
            DrawCube(nuestroShader, VAO13, glm::vec3(35.0f, -7.0f, -10.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(43.0f, -7.0f, -10.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(51.0f, -7.0f, -10.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(59.0f, -7.0f, -10.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(67.0f, -7.0f, -10.0f), 0, 0, diffuseMap, barMap);

            DrawCube(nuestroShader, VAO13, glm::vec3(35.0f, -7.0f, 45.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(43.0f, -7.0f, 45.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(51.0f, -7.0f, 45.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(59.0f, -7.0f, 45.0f), 0, 0, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(67.0f, -7.0f, 45.0f), 0, 0, diffuseMap, barMap);

            DrawCube(nuestroShader, VAO13, glm::vec3(74.5f, -7.0f, 45.0f), 90, 1, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(74.5f, -7.0f, 37.0f), 90, 1, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(74.5f, -7.0f, 29.0f), 90, 1, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(74.5f, -7.0f, 21.0f), 90, 1, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(74.5f, -7.0f, 13.0f), 90, 1, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO13, glm::vec3(74.5f, -7.0f, 5.0f), 90, 1, diffuseMap, barMap);
            DrawCube(nuestroShader, VAO14, glm::vec3(74.5f, -7.0f, -3.0f), 90, 1, diffuseMap, barMap);

#pragma endregion

           
                //Techo
                DrawCube(nuestroShader, VAO16, glm::vec3(-5.0f, 23.0f, -10.0f), 315, 2, diffuseMap, roofMap);
                DrawCube(nuestroShader, VAO16, glm::vec3(40.0f, 23.0f, -10.0f), 45, 2, diffuseMap, roofMap);
                //DrawCube(nuestroShader, VAO14, glm::vec3(74.5f, -7.0f, -3.0f), 90, 1, diffuseMap, barMap);
        





        //mvp 
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)ancho / (float)largo, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();
        nuestroShader.setMat4("projection", projection);
        nuestroShader.setMat4("view", view);
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glBindVertexArray(VAO);
        //configuraciones del modelo
        glm::mat4 model = glm::mat4(1.0f);


        //model = glm::translate(model, glm::vec3(1.2f,0.0f,0.0f));
        nuestroShader.setMat4("model", model);
        model = glm::translate(model, pos);
        //linkeamos nuestras texturas
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        /*for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, posicionesCubo[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            nuestroShader.setMat4("model", model);


            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        */
        ////SKYBOX//////
        
       /* glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(cam.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        glBindVertexArray(skybxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);*/
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    //glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    ////SKYBOX//////
    glDeleteVertexArrays(1, &skybxVAO);
    glDeleteVertexArrays(1, &skybxVAO);

    glfwTerminate();
    return 0;
}

void BuildCube(float _ancho, float _alto, float _largo, Shader _objShader , unsigned int _VBO, unsigned int _VAO, unsigned int _EBO) {

    
    float vertex[] = {
       ////--X--/////////--Y--///////--Z--
        0.0f,          0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.0f + _ancho, 0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.0f + _ancho, 0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.0f + _ancho, 0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.0f,          0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        0.0f,          0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        ////--X--/////////--Y--/////////--Z--
        0.0f,          0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.0f + _ancho, 0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.0f + _ancho, 0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.0f + _ancho, 0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.0f,          0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        0.0f,          0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

      //--X--/////--Y--/////////--Z--
        0.0f,  0.0f + _alto, 0.0f + _largo, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.0f,  0.0f + _alto, 0.0f,         -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.0f,  0.0f,         0.0f,         -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,         0.0f,         -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,         0.0f + _largo, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.0f,  0.0f + _alto, 0.0f + _largo, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        ///--X--///////////--Y--/////////--Z--
        0.0f + _ancho,  0.0f + _alto, 0.0f + _largo,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.0f + _ancho,  0.0f + _alto, 0.0f,          1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.0f + _ancho,  0.0f,         0.0f,          1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.0f + _ancho,  0.0f,         0.0f,          1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.0f + _ancho,  0.0f,         0.0f + _largo,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.0f + _ancho,  0.0f + _alto, 0.0f + _largo,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        ///--X--///////--Y--//////--Z--
        0.0f,          0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.0f + _ancho, 0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.0f + _ancho, 0.0f,  0.0f + _largo,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.0f + _ancho, 0.0f,  0.0f + _largo,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.0f,          0.0f,  0.0f + _largo,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        0.0f,          0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        ///--X--//////////--Y--/////////--Z--
        0.0f,          0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.0f + _ancho, 0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.0f + _ancho, 0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.0f + _ancho, 0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.0f,          0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        0.0f,          0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    unsigned int indices[]{

      0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        14, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
      
    };
    
    //Unificamos
    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //atributos de posicion 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //atributos de colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //attribs de testuras :v
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    _objShader.use();
    _objShader.setInt("material.diffuse", 1);
    _objShader.setInt("material.specular", 0);

}

void DrawCube(Shader _objShader, unsigned int _VAO, glm::vec3 pos,float angles, int axis, unsigned int _diffuseMap, unsigned int _specularMap) {

    //activar las caracteristicas de luz en nuestro objeto
    _objShader.use();
    _objShader.setVec3("light.position", cam.Position);
    _objShader.setVec3("light.direction", cam.Front);
    _objShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    _objShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    _objShader.setVec3("viewPos", cam.Position);

    //propiedades de la luz
    _objShader.setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
    _objShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    _objShader.setVec3("light.specular", 3.0f, 3.0f, 3.0f);
    _objShader.setFloat("light.constant", 1.0f);
    _objShader.setFloat("light.linear", 0.045f);
    _objShader.setFloat("light.quadratic", 0.0075f);

    //propiedades de los materiales
    _objShader.setFloat("material.shininess", 64.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _specularMap);
    glBindVertexArray(_VAO);
    //configuraciones del modelo
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    switch (axis)
    {
    case 0:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(1.0f, 0.0f, 0.0f));
        break;
    case 1:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 2:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    case 3:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(1.0f, 1.0f, 1.0f));
        break;   
    }
    
    
    _objShader.setMat4("model", model);

    //linkeamos nuestras texturas
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}
void processInput(GLFWwindow* window)
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



// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset);
}


// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.ProcessMouseScroll(yoffset);
}

//Funcion que carga texturas
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "fallamos al cargar las texturas del siguiente origen: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}
unsigned int loadCubeMap(vector<string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            cout << "Fallo en cargar las texturas del skybox " << faces[i] << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_LINEAR);

    return textureID;

}

