#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"
#include "Libs/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(6.0f, 5.0f, 0.0f);

float yaw = 0.0f, pitch = 0.0f;

//Vertex Shader
static const char *vShader = "Shaders/shader.vert";

//Fragment Shader
static const char *fShader = "Shaders/shader.frag";

void CreateTriangle()
{
    GLfloat vertices[] =
        {
            //pos                       //TexCoord
            -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,      0.5f, 0.0f,
            1.0f, -1.0f, 0.0f,       1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,       0.5f, 1.0f
            };

    unsigned int indices[] =
        {
            0,  3,  1,
            1,  3,  2,
            2,  3,  0,
            0,  1,  2,

        };

    // square
    GLfloat svertices[] =
        {
            1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
        };
    unsigned int sindices[] =
        {
            0, 1, 2,
            1, 2, 3,
            2, 3, 7,
            2, 7, 6,
            2, 6, 0,
            0, 4, 6,
            0, 4, 1,
            1, 4, 5,
            1, 5, 3,
            3, 5, 7,
            5, 7, 6,
            5, 6, 4

        };

    Mesh *obj = new Mesh();
    obj->CreateMesh(vertices, indices, 20, 12);

    //Mesh *obj2 = new Mesh();
    //obj2->CreateMesh(svertices, sindices, 38, 36);
}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void CreateOBJ(){
    Mesh *obj1 = new Mesh();
    bool loaded = obj1->CreateMeshFromOBJ("Models/car2.obj");
    if(loaded){
        for(int i = 0; i< 10; i++){
            meshList.push_back(obj1);
        }
    }
    else{
        std::cout<<"Failed to load model"<<std::endl;
    }

    Mesh *obj2 = new Mesh();
    loaded = obj2->CreateMeshFromOBJ("Models/car2.obj");
    if (loaded)
    {
        for (int i = 0; i < 10; i++)
        {
            meshList.push_back(obj2);
        }
    }
    else
    {
        std::cout << "Failed to load model" << std::endl;
    }
}


int main()
{
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    CreateOBJ();
    //CreateTriangle();
    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;
    //glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth(), 0.1f, 100.0f);

    glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);

    unsigned int texture, texture2;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("Textures/uvmap.png", &width, &height, &nrChannels, 0);
    
    if (data)
    {
        //BIND IMAGE WITH TEXTURES
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Loop until window closed
    while (!mainWindow.getShouldClose())
    {

        //Get + Handle user input events
        glfwPollEvents();

        //Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //draw here
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetUniformLocation("view");

        glm::mat4 view(1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraPos = glm::vec3(1.0f, 1.0f, 1.0f);    //ตำแหน่งกล้อง
        glm::vec3 cameraTarget = glm::vec3(1.0f, 1.0f, 0.0f); //จุดที่สนใจ

        glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, -cameraRight));

        glm::mat4 cameraPosMat(1.0f);
        cameraPosMat[0][3] = -cameraPos.x;
        cameraPosMat[1][3] = -cameraPos.y;
        cameraPosMat[2][3] = -cameraPos.z;

        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);
        //view = cameraRotateMat * glm::transpose(cameraPosMat);
        view = glm::lookAt(cameraPos, cameraPos + cameraDirection, up);

        glm::vec3 pyramidPositions[] =
            {
                glm::vec3(0.0f, 0.0f, -2.5f),
                glm::vec3(2.0f, 5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.2f, -2.5f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f, 3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f, 2.0f, -2.5f),
                glm::vec3(1.5f, 0.2f, -1.5f),
                glm::vec3(-1.3f, 1.0f, -1.5f)};

        //Object
            glm::mat4 model(1.0f);

            //model = glm::translate(model, pyramidPositions[0]);
            //model = glm::rotate(model, glm::radians(2.0f * 0), glm::vec3(1.0f, 0.3f, 0.5f));
            //model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            // light
            glUniform3fv(shaderList[0].GetUniformLocation("lightPos"), 1, (GLfloat *)&lightPos);
            glUniform3fv(shaderList[0].GetUniformLocation("lightColour"), 1, (GLfloat *)&lightColour);
            glUniform3fv(shaderList[0].GetUniformLocation("viewPos"), 1, (GLfloat *)&cameraPos);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            meshList[0]->RenderMesh();

            //meshList[1]->RenderMesh();

            //---------------------//
            
            glm::mat4 model2(1.0f);
            model2 = glm::translate(model, glm::vec3(1.0f, -0.5f, -0.8f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            // light
            glUniform3fv(shaderList[0].GetUniformLocation("lightPos"), 1, (GLfloat *)&lightPos);
            glUniform3fv(shaderList[0].GetUniformLocation("lightColour"), 1, (GLfloat *)&lightColour);
            glUniform3fv(shaderList[0].GetUniformLocation("viewPos"), 1, (GLfloat *)&cameraPos);
            meshList[1]->RenderMesh();

            model2 = glm::translate(model2, glm::vec3(1.0f, -0.5f, -0.8f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            // light
            glUniform3fv(shaderList[0].GetUniformLocation("lightPos"), 1, (GLfloat *)&lightPos);
            glUniform3fv(shaderList[0].GetUniformLocation("lightColour"), 1, (GLfloat *)&lightColour);
            glUniform3fv(shaderList[0].GetUniformLocation("viewPos"), 1, (GLfloat *)&cameraPos);
            meshList[1]->RenderMesh();

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
            glUseProgram(0);

            // end draw

            mainWindow.swapBuffers();
    }

    return 0;
}
//63050095 Kanticha Traisri