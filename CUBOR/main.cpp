//alumno:Victor Manuel Toledo Gamarra
//Link de gitHub: https://github.com/ggmanuel1998/CuboRubik


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include <cmath>
#include <time.h>
#include <iostream>
#include "Rubik.h"
#include "AllSolver/solve.h"
#include "AllSolver/random.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 view;\n"
"void main()\n"
"{\n"
"   float camara = 8.0;\n"
"   gl_Position = view * vec4(aPos, camara);\n"
"   //gl_Position = view * vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"   TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"FragColor =  vec4(ourColor, 1.0f)*texture(texture1, TexCoord) ;\n"
"   //FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";



///////////////////////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
// ---------------------------------------------------------------------------------------------------------
///////////VARIABLES
bool condition = false;

//
float camera_pos_x = 0.0f, camera_pos_y = 0.0f, camera_pos_z = 0.0f;
glm::vec3 cameraPos = glm::vec3(camera_pos_x, camera_pos_y, camera_pos_z);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = 0.0f;
float pitch = 0;
float cam_x = 0, cam_y = 0;
float vel_camera = 1;
/// mouse

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

//
string condition_input = "N";
vector<string> reg_mov;
vector<string> solution_cubo;
// ---------------------------------------------------------------------------------------------------------
//objetos

Rubik vic ;
//Cubo Ra({-1,-1,-1}, {1,1,1});
Rubik* pt;
//*pt = &vic;
/*
Polygono estrella(vertices_estrella, Restrella, Gestrella, Bestrella);
Polygono rombo(vertices_rombo, Rrombo, Grombo, Brombo);
Polygono cuadrado(vertices_cuadrado, Rcuadrado, Gcuadrado, Bcuadrado);
Polygono triangulo_rojo(vertices_triangulo, Rtriangulo, Gtriangulo, Btriangulo);
*/
// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
   
 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
 ///
    float cameraSpeed = 0.99 ;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
       pitch -= (cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pitch += (cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
       yaw += (cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
         yaw -= (cameraSpeed);
    }
 ///movi camadas 
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && condition_input == "N") {
        reg_mov.push_back("R");
        //cout<<"INICIA ANIMACION R"<<endl;
        condition_input = "R";
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && condition_input == "N") {
        reg_mov.push_back("L");
        //cout<<"INICIA ANIMACION L"<<endl;
        condition_input = "L";
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && condition_input == "N") {
        reg_mov.push_back("U");
        //cout<<"INICIA ANIMACION U"<<endl;
        condition_input = "U";
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && condition_input == "N") {
        reg_mov.push_back("D");
        //cout<<"INICIA ANIMACION D"<<endl;
        condition_input = "D";
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && condition_input == "N") {
        reg_mov.push_back("F");
        //cout<<"INICIA ANIMACION F"<<endl;
        condition_input = "F";
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && condition_input == "N") {
        reg_mov.push_back("B");
        //cout<<"INICIA ANIMACION B"<<endl;
        condition_input = "B";
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && condition_input == "N") {

        condition_input = "A";
        string temp = to_cube_not(reg_mov);
        reg_mov.clear();

        solution_cubo = get_solution(temp);

        cout << endl;
    } 
    ///
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vic.rot_x(3);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vic.rot_x(-3);
    }
 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vic.rot_y(3);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vic.rot_y(-3);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        vic.rot_z(3);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && condition_input == "N") {
        condition_input = "M";
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        vic.masescalar();
    }
}

// ---------------------------------------------------------------------------------------------------------

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    glEnable(GL_DEPTH_TEST);
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    /*
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);
    */

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("C:/Users/user/Desktop/GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CUBOR/carasfi.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, texture);
    


    // render loop
    // ---------------------------------------------------------------------------------------------------
    cout << "Presione W,A,S,D,Q para mover el cubo(NO CAMARA VIRTUAL)" << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione las flechas para mover la camara virtual , que tambien puede ser con el cursor del mouse " << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione 1 para la cara FRONT" << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione 2 para la cara BACK" << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione 3 para la cara LEFT" << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione 4 para la cara RIGHT" << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione 5 para la cara UP" << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione 6 para la cara DOWN" << endl;
    cout << "//---------------------------------------------------" << endl;
    cout << "Presione 8 para aplicar el solver del rubik (metodo kociemba o de dos pasos)" << endl;
    cout << "//---------------------------------------------------" << endl;
  /*
    // -----------
     ///cara verde 
    vic.cambiarcolor_cara(2, 0, 0, 1, 0);
    vic.cambiarcolor_cara(11, 0, 0, 1, 0);
    vic.cambiarcolor_cara(20, 0, 0, 1, 0);

    vic.cambiarcolor_cara(5, 0, 0, 1, 0);
    vic.cambiarcolor_cara(14, 0, 0, 1, 0);
    vic.cambiarcolor_cara(23, 0, 0, 1, 0);

    vic.cambiarcolor_cara(8, 0, 0, 1, 0);
    vic.cambiarcolor_cara(17, 0, 0, 1, 0);
    vic.cambiarcolor_cara(26, 0, 0, 1, 0);

    ///cara roja
    vic.cambiarcolor_cara(2, 2, 1, 0, 0);
    vic.cambiarcolor_cara(1, 2, 1, 0, 0);
    vic.cambiarcolor_cara(0, 2, 1, 0, 0);

    vic.cambiarcolor_cara(5, 2, 1, 0, 0);
    vic.cambiarcolor_cara(4, 2, 1, 0, 0);
    vic.cambiarcolor_cara(3, 2, 1, 0, 0);

    vic.cambiarcolor_cara(8, 2, 1, 0, 0);
    vic.cambiarcolor_cara(7, 2, 1, 0, 0);
    vic.cambiarcolor_cara(6, 2, 1, 0, 0);

    ///cara azul
    vic.cambiarcolor_cara(2, 4, 0, 0, 1);
    vic.cambiarcolor_cara(1, 4, 0, 0, 1);
    vic.cambiarcolor_cara(0, 4, 0, 0, 1);

    vic.cambiarcolor_cara(11, 4, 0, 0, 1);
    vic.cambiarcolor_cara(10, 4, 0, 0, 1);
    vic.cambiarcolor_cara(9, 4, 0, 0, 1);

    vic.cambiarcolor_cara(20, 4, 0, 0, 1);
    vic.cambiarcolor_cara(19, 4, 0, 0, 1);
    vic.cambiarcolor_cara(18, 4, 0, 0, 1);

    ///cara amarillo
    vic.cambiarcolor_cara(0, 1, 0, 1, 0);//
    vic.cambiarcolor_cara(9, 1, 1, 1, 0);
    vic.cambiarcolor_cara(18, 1, 1, 1, 0);

    vic.cambiarcolor_cara(3, 1, 0, 1, 0);//
    vic.cambiarcolor_cara(12, 1, 1, 1, 0);
    vic.cambiarcolor_cara(21, 1, 1, 1, 0);

    vic.cambiarcolor_cara(6, 1, 0, 1, 0);//
    vic.cambiarcolor_cara(15, 1, 1, 1, 0);
    vic.cambiarcolor_cara(24, 1, 1, 1, 0);

    ///cara naranja
    vic.cambiarcolor_cara(20, 3, 1, 0.5, 0);
    vic.cambiarcolor_cara(21, 3, 1, 0.5, 0);
    vic.cambiarcolor_cara(22, 3, 1, 0.5, 0);

    vic.cambiarcolor_cara(23, 3, 1, 0.5, 0);
    vic.cambiarcolor_cara(24, 3, 1, 0.5, 0);
    vic.cambiarcolor_cara(25, 3, 1, 0.5, 0);

    vic.cambiarcolor_cara(26, 3, 1, 0.5, 0);
    vic.cambiarcolor_cara(18, 3, 1, 0.5, 0);
    vic.cambiarcolor_cara(19, 3, 1, 0.5, 0);

    ///cara blanco
    vic.cambiarcolor_cara(6, 5, 1, 1, 1);
    vic.cambiarcolor_cara(7, 5, 1, 1, 1);
    vic.cambiarcolor_cara(8, 5, 1, 1, 1);

    vic.cambiarcolor_cara(15, 5, 1, 1, 1);
    vic.cambiarcolor_cara(16, 5, 1, 1, 1);
    vic.cambiarcolor_cara(17, 5, 1, 1, 1);

    vic.cambiarcolor_cara(24, 5, 1, 1, 1);
    vic.cambiarcolor_cara(25, 5, 1, 1, 1);
    vic.cambiarcolor_cara(26, 5, 1, 1, 1);
    */
    //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 view = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));



    while (!glfwWindowShouldClose(window)) {
        
          float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        glfwSetKeyCallback(window, processInput);
        // -----
        // render
        glClearColor(0, 0, 0, 5.0f);//fondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //profundidad

        /*
        triangulo_rojo.escalao(1.002, 1.002, 0);
        cuadrado.cambiarcolor(Restrella, Gestrella, Bestrella);
        cuadrado.pro_rotation_z(5);

        movimiento_cuadrado(rombo, -0.9, -0.9, 0.9, 0.9);
        lucifer_estrella(estrella, 1.3,1.3,-1.3,-1.3);
        color(&Restrella, &Gestrella, &Bestrella);
        */
        ///dibujar

        /*
        estrella.draw(window);
        rombo.draw(window);
        cuadrado.draw(window);
        triangulo_rojo.draw(window);
        */
       /// 
        glm::mat4 R = glm::yawPitchRoll(glm::radians(yaw), glm::radians(pitch), 0.0f);

        cameraFront = glm::vec3(R * glm::vec4(0, 0, -1, 0));//B
        cameraUp = glm::vec3(R * glm::vec4(0, 1, 0, 0));//A
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        /// 
        if (condition_input == "R") {
            condition_input = vic.animation_R();
        }
        if (condition_input == "U") {
            condition_input = vic.animation_U();
        }
        if (condition_input == "D") {
            condition_input = vic.animation_D();
        }
        if (condition_input == "F") {
            condition_input = vic.animation_F();
        }
        if (condition_input == "B") {
            condition_input = vic.animation_B();
        }
        if (condition_input == "L") {
            condition_input = vic.animation_L();
        }
        if (condition_input == "A") {
            condition_input = vic.aplicar_solucion(solution_cubo);
        }
        if (condition_input == "M") {
            condition_input = vic.bucle();
        }
        ///
       // Ra.print_min_max_axis();
       // Ra.draw(window);

        vic.draw(window);

       // Ra.cambiarcolor_cara(1, 1, 0, 0);
       // Ra.draw(window);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    // optional: de-allocate all resources once they've outlived their purpose:
    //------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}