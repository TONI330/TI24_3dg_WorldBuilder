#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpCam.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "World.h"
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#define ATLAS_WIDTH 32
#define ATLAS_HEIGHT 32

GLFWwindow* window;

FpCam* camera;
World* world;

double lastTime = 0;

glm::mat4 cubeModel;
int textureMap[20][20][10];

unsigned int textureID;

void fillMap()
{
    for (int x = 0; x < 20; x++)
        for (int z = 0; z < 20; z++)
            for (int y = 0; y < 10; y++)
            {
                if (y == 5) textureMap[x][z][y] = 200;
                if (y > 5) textureMap[x][z][y] = -1;
                if (y < 5) textureMap[x][z][y] = 308;
            }
}

void drawTriangle()
{
    tigl::begin(GL_TRIANGLES);
    tigl::addVertex(Vertex::PC(glm::vec3(-2, -1, -4), glm::vec4(1, 0, 0, 1)));
    tigl::addVertex(Vertex::PC(glm::vec3(2, -1, -4), glm::vec4(0, 1, 0, 1)));
    tigl::addVertex(Vertex::PC(glm::vec3(0, 1, -4), glm::vec4(0, 0, 1, 1)));


    tigl::addVertex(Vertex::PC(glm::vec3(-10, -1, -10), glm::vec4(1, 1, 1, 1)));
    tigl::addVertex(Vertex::PC(glm::vec3(-10, -1, 10), glm::vec4(1, 1, 1, 1)));
    tigl::addVertex(Vertex::PC(glm::vec3(10, -1, 10), glm::vec4(1, 1, 1, 1)));

    tigl::addVertex(Vertex::PC(glm::vec3(-10, -1, -10), glm::vec4(1, 1, 1, 1)));
    tigl::addVertex(Vertex::PC(glm::vec3(10, -1, -10), glm::vec4(1, 1, 1, 1)));
    tigl::addVertex(Vertex::PC(glm::vec3(10, -1, 10), glm::vec4(1, 1, 1, 1)));

    tigl::end();
}

void drawCube(int textureIndex, glm::vec3 pos, int size = 1)
{
    const float textureWidth = 1.0f / ATLAS_WIDTH;
    const float textureHeigth = 1.0f / ATLAS_HEIGHT;

    const float tx0 = (textureIndex % ATLAS_WIDTH) * textureWidth;
    const float tx1 = tx0 + textureWidth;
    const float ty0 = (textureIndex / ATLAS_HEIGHT) * textureHeigth;
    const float ty1 = ty0 + textureHeigth;

    const glm::vec3 v0 = pos;
    const glm::vec3 v1 = glm::vec3(pos.x + size, pos.y, pos.z);
    const glm::vec3 v2 = glm::vec3(pos.x + size, pos.y + size, pos.z);
    const glm::vec3 v3 = glm::vec3(pos.x, pos.y + size, pos.z);

    const glm::vec3 v4 = glm::vec3(pos.x, pos.y, pos.z + size);
    const glm::vec3 v5 = glm::vec3(pos.x + size, pos.y, pos.z + size);
    const glm::vec3 v6 = glm::vec3(pos.x + size, pos.y + size, pos.z + size);
    const glm::vec3 v7 = glm::vec3(pos.x, pos.y + size, pos.z + size);

    std::vector<Vertex> cube{

        // Front face   
        Vertex::PTC(v0, glm::vec2(tx1,ty1), glm::vec4(1, 0, 0, 1)), //0
        Vertex::PTC(v1, glm::vec2(tx0,ty1), glm::vec4(1, 0, 0, 1)), //1
        Vertex::PTC(v2, glm::vec2(tx0,ty0), glm::vec4(1, 0, 0, 1)), //2
        Vertex::PTC(v3, glm::vec2(tx1,ty0), glm::vec4(1, 0, 0, 1)), //3

        // Back face
        Vertex::PTC(v4, glm::vec2(tx1,ty1), glm::vec4(0, 1, 0, 1)), //4
        Vertex::PTC(v5, glm::vec2(tx0,ty1), glm::vec4(0, 1, 0, 1)), //5
        Vertex::PTC(v6, glm::vec2(tx0,ty0), glm::vec4(0, 1, 0, 1)), //6
        Vertex::PTC(v7, glm::vec2(tx1,ty0), glm::vec4(0, 1, 0, 1)), //7

        // Right face
        Vertex::PTC(v1, glm::vec2(tx1,ty1), glm::vec4(0, 0, 1, 1)),
        Vertex::PTC(v5, glm::vec2(tx0,ty1), glm::vec4(0, 0, 1, 1)),
        Vertex::PTC(v6, glm::vec2(tx0,ty0), glm::vec4(0, 0, 1, 1)),
        Vertex::PTC(v2, glm::vec2(tx1,ty0), glm::vec4(0, 0, 1, 1)),

        // Left Face
        Vertex::PTC(v0, glm::vec2(tx1,ty1), glm::vec4(0, 1, 1, 1)),
        Vertex::PTC(v4, glm::vec2(tx0,ty1), glm::vec4(0, 1, 1, 1)),
        Vertex::PTC(v7, glm::vec2(tx0,ty0), glm::vec4(0, 1, 1, 1)),
        Vertex::PTC(v3, glm::vec2(tx1,ty0), glm::vec4(0, 1, 1, 1)),

        // Top Face
        Vertex::PTC(v3, glm::vec2(tx0,ty0), glm::vec4(1, 1, 0, 1)),
        Vertex::PTC(v2, glm::vec2(tx1,ty0), glm::vec4(1, 1, 0, 1)),
        Vertex::PTC(v6, glm::vec2(tx1,ty1), glm::vec4(1, 1, 0, 1)),
        Vertex::PTC(v7, glm::vec2(tx0,ty1), glm::vec4(1, 1, 0, 1)),

        // Down Face
        Vertex::PTC(v0, glm::vec2(tx0,ty0), glm::vec4(1, 1, 0, 1)),
        Vertex::PTC(v1, glm::vec2(tx1,ty0), glm::vec4(1, 1, 0, 1)),
        Vertex::PTC(v5, glm::vec2(tx1,ty1), glm::vec4(1, 1, 0, 1)),
        Vertex::PTC(v4, glm::vec2(tx0,ty1), glm::vec4(1, 1, 0, 1)),
    };

    tigl::shader->setModelMatrix(cubeModel);
    tigl::drawVertices(GL_QUADS, cube);

}


void init();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1000, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init(); // Initialize shader
    init(); // Initialize world

	while (!glfwWindowShouldClose(window))
	{
        world->UpdateWorld();
        world->DrawWorld();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
    return 0;
}

void init()
{

 //   int value[10];
 //   glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    camera = new FpCam(window);
    world = new World(*window, *camera);
 //   camera = new FpCam(window);

 //   cubeModel = glm::mat4(1.0f);

 //   unsigned int textureId;
 //   glGenTextures(1, &textureId);
 //   glBindTexture(GL_TEXTURE_2D, textureId);

 //   int width, height, bpp;
	//unsigned char* imgData = stbi_load("textures/minecraft_atlas.png", &width, &height, &bpp, 4);

 //   glTexImage2D(GL_TEXTURE_2D,
 //       0,		//level
 //       GL_RGBA,		//internal format
 //       512,		//width
 //       512,		//height
 //       0,		//border
 //       GL_RGBA,		//data format
 //       GL_UNSIGNED_BYTE,	//data type
 //       imgData);		//data
 //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

 //   glBindTexture(GL_TEXTURE_2D, textureId);
 //   fillMap();
    

}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());

    //tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);
    tigl::shader->enableTexture(true);

    //drawTriangle();
    drawCube(5, glm::vec3(2, -1, 0));
    drawCube(5, glm::vec3(2, 0, 0));
    drawCube(5, glm::vec3(3, -1, 0));
    drawCube(5, glm::vec3(3, 0, 0));
    drawCube(5, glm::vec3(4, -1, 0));
    drawCube(5, glm::vec3(2, -1, 1));

    for (int x = 0; x < 20; x++)
        for (int z = 0; z < 20; z++)
            for (int y = 0; y < 10; y++)
            {
                if (textureMap[x][z][y] == -1) continue;

                glm::mat4 block = glm::mat4(1.0f);
                drawCube(textureMap[x][z][y], glm::vec3(x, y - 7, z));
            }

}