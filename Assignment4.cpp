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

// Assignment 4
const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader *> shaderList;

glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, -10.0f);

// float yaw = 0.0f, pitch = 0.0f;
Mesh *light;
static const char *lightVShader = "Shaders/lightShader.vert";
static const char *lightFShader = "Shaders/lightShader.frag";

// Vertex Shader
static const char *vShader = "Shaders/shader.vert";

// Fragment Shader
static const char *fShader = "Shaders/shader.frag";

void CreateOBJ()
{
    // Object about sea-----------------------------------------------------------------
    // obj1
    Mesh *obj1 = new Mesh();
    bool loaded1 = true; //fix bug
    // obj2
    Mesh *obj2 = new Mesh();
    bool loaded2 = obj2->CreateMeshFromOBJ("Models/carop.obj");
    // obj3
    Mesh *obj3 = new Mesh();
    bool loaded3 = obj3->CreateMeshFromOBJ("Models/treehouse.obj");
    // obj4
    Mesh *obj4 = new Mesh();
    bool loaded4 = obj4->CreateMeshFromOBJ("Models/plane.obj");
    // obj5
    Mesh *obj5 = new Mesh();
    bool loaded5 = obj5->CreateMeshFromOBJ("Models/house.obj");
    // obj6
    Mesh *obj6 = new Mesh();
    bool loaded6 = obj6->CreateMeshFromOBJ("Models/home.obj");
    // obj7
    Mesh *obj7 = new Mesh();
    bool loaded7 = obj7->CreateMeshFromOBJ("Models/scaniverse-20210530-070602.obj");
    // obj8
    Mesh *obj8 = new Mesh();
    bool loaded8 = obj8->CreateMeshFromOBJ("Models/Scaniverse.obj");

    // Loading
    //  Object ----------------------------------------------------------------
    if (loaded1)
    {
        meshList.push_back(obj1);
        std::cout << "load model 1" << std::endl;
    }else
    {
        std::cout << "Failed to load model 1" << std::endl;
    }

    if (loaded2)
    {
        meshList.push_back(obj2);
        std::cout << "load model 2" << std::endl;
    }
    else
    {
        std::cout << "Failed to load model 2" << std::endl;
    }

    if (loaded3)
    {
        meshList.push_back(obj3);
        std::cout << "load model 3" << std::endl;
    }
    else
    {
        std::cout << "Failed to load model 3" << std::endl;
    }

    if (loaded4)
    {
        meshList.push_back(obj4);
        std::cout << "load model 4" << std::endl;
    }
    else
    {
        std::cout << "Failed to load model 4" << std::endl;
    }

    if (loaded5)
    {
        meshList.push_back(obj5);
        std::cout << "load model 5" << std::endl;
    }
    else
    {
        std::cout << "Failed to load model 5" << std::endl;
    }

    if (loaded6)
    {
        meshList.push_back(obj6);
        std::cout << "load model 6" << std::endl;
    }
    else
    {
        std::cout << "Failed to load model 6" << std::endl;
    }

    if (loaded7)
    {
        meshList.push_back(obj7);
        std::cout << "load model 7" << std::endl;
    }
    else
    {
        std::cout << "Failed to load model 7" << std::endl;
    }

    if (loaded8)
    {
        meshList.push_back(obj8);
        std::cout << "load model 8" << std::endl;
    }
    else
    {
        std::cout << "Failed to load model 8" << std::endl;
    }

    // light cube แหล่งกำเนิดแสง----------------------------------
    light = new Mesh();
    bool loaded = light->CreateMeshFromOBJ("Models/cube.obj");
    if (!loaded)
    {
        std::cout << "Failed to load model" << std::endl;
        delete (light);
    }
}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader1);

    //ตน ที่ 1
    Shader *shader2 = new Shader();
    shader2->CreateFromFiles(lightVShader, lightFShader);
    shaderList.push_back(shader2);
}

int main()
{
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    // CreateTriangle();
    CreateShaders();
    CreateOBJ();

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
    // All Objects's scale equal
    // glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);

    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, -cameraRight));
    // texture0
    int width, height, nrChannels;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load("Textures/uvmap.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // bind image with texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // 1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    unsigned char *data1 = stbi_load("Textures/Mirado_B_01_Df.png", &width, &height, &nrChannels, 0);
    if (data1)
    {
        // bind image with texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);

    // 2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    unsigned char *data2 = stbi_load("Textures/snow01.png", &width, &height, &nrChannels, 0);
    if (data2)
    {
        // bind image with texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    // 3
    unsigned int texture3;
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    unsigned char *data3 = stbi_load("Textures/rainbow.png", &width, &height, &nrChannels, 0);
    if (data3)
    {
        // bind image with texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data3);

    // Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        // Get + Handle user input events
        glfwPollEvents();

        cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        cameraUp = glm::normalize(glm::cross(cameraDirection, -cameraRight));

        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraDirection * 0.01f;
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraDirection * 0.01f;
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= cameraRight * 0.01f;
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += cameraRight * 0.01f;

        // Clear window --สีของหน้าจอ--
        glClearColor(0.0, 0.0, 0.0, 0.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw here
        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetUniformLocation("model");
        uniformView = shaderList[0]->GetUniformLocation("view");
        uniformProjection = shaderList[0]->GetUniformLocation("projection");

        // Position for object
        glm::vec3 objPositions[] = {
            glm::vec3(-6.0f, -2.0f, -3.5f), // ตำแหน่งรถ 1
            glm::vec3(0.0f, -2.0f, -5.5f),   // ตำแหน่งรถ 2
            glm::vec3(6.0f, -2.0f, -5.5f),   // ตำแหน่งรถ 3
            glm::vec3(16.0f, -2.0f, -5.5f),  // ตำแหน่งรถ 4
            glm::vec3(-6.0f, -2.0f, -3.5f),
            glm::vec3(-12.0f, -2.0f, -3.5f),
            glm::vec3(-18.0f, -2.0f, -3.5f),
            glm::vec3(-24.0f, -2.0f, -3.5f)
        };

        glm::mat4 view(1.0f);

        glm::mat4 cameraPosMat(1.0f);
        cameraPosMat[0][3] = cameraPos.x;
        cameraPosMat[1][3] = cameraPos.y;
        cameraPosMat[2][3] = cameraPos.z;

        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(-cameraRight.z, -cameraUp.z, -cameraDirection.z, 0.0f);

        view = glm::lookAt(cameraPos, cameraPos + cameraDirection, up);

        for (int i = 0; i < 8; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, objPositions[i]);
            // model = glm::rotate(model, glm::radians(2.0f), glm::vec3(50.0f, 0.0f, 10.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            // light
            glUniform3fv(shaderList[0]->GetUniformLocation("lightColour"), 1, (GLfloat *)&lightColour);
            glUniform3fv(shaderList[0]->GetUniformLocation("lightPos"), 1, (GLfloat *)&lightPos);

            glUniform3fv(shaderList[0]->GetUniformLocation("viewPos"), 1, (GLfloat *)&cameraPos);

            if (i == 0 || i==4) // texture รถคนที่1

            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);
                meshList[0]->RenderMesh();
            }

            if (i == 1) // texture รถคนที่1

            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture1);
                meshList[0]->RenderMesh();
            }

            if (i == 3 || i==5 || i==7) // texture รถคนที่3

            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture2);
                meshList[0]->RenderMesh();
            }

            if (i == 2 || i==6) // texture รถคนที่4

            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture3);
                meshList[0]->RenderMesh();
            }

            meshList[i]->RenderMesh();
        }
        // light cube
        shaderList[1]->UseShader();
        uniformModel = shaderList[1]->GetUniformLocation("model");
        uniformView = shaderList[1]->GetUniformLocation("view");
        uniformProjection = shaderList[1]->GetUniformLocation("projection");

        glm::mat4 model(1.0f);

        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // light
        glUniform3fv(shaderList[1]->GetUniformLocation("lightColour"), 1, (GLfloat *)&lightColour);
        light->RenderMesh();

        glUseProgram(0);
        // end draw

        mainWindow.swapBuffers();
    }

    return 0;
}
