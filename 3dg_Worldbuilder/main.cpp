#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpCam.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "World.h"
#include "Object3d.h"
#include "imgui.h"
#include "lib/imgui-1.60/imgui_impl_glfw_gl3.h"
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

    // Initialize GUI
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();


    tigl::init(); // Initialize shader
    init(); // Initialize world

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(window))
	{
        ImGui_ImplGlfwGL3_NewFrame();
        world->UpdateWorld();
        world->DrawWorld();

        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
        
	}
    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
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
    world->AddWorldObject(new Object3d("models/car/honda_jazz.obj", "honda jazz"));
 //   camera = new FpCam(window);

 //   cubeModel = glm::mat4(1.0f);


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