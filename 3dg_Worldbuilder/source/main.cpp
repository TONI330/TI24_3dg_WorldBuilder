#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpCam.h"
#include <iostream>
#include "stb_image.h"
#include "World.h"
#include "Object3d.h"
#include "StaticSettings.h"
#include "EditGUI.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

FpCam* camera;
World* world;
EditGUI* gui;

double lastTime = 0;
bool editMode = true;

int main(void);
void Init();
void ToggleEditMode();
void CenterCursor();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init(); // Initialize shader
    Init(); // Initialize world

	while (!glfwWindowShouldClose(window))
	{
        world->UpdateWorld();
        world->DrawWorld();

        if (editMode)
        {
            gui->Update();
        }
        else
        {
            camera->update(window);
        }
        
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}
    // Cleanup 
    delete(camera, world, gui);
	glfwTerminate();
    return 0;
}

void Init()
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
            ToggleEditMode();
    });

    camera = new FpCam(window);
    world = new World(*window, *camera);

    //add map
    auto map = new Object3d(MAPFILE, "map");
    map->Scale(MAPSIZE);
    world->AddWorldObject(map);
    gui = new EditGUI(*world, *window);
}

double cursorX, cursorY;
void ToggleEditMode()
{
    editMode = !editMode;
    if (!editMode)
    {
        // Prepare camera
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, cursorX, cursorY);
    }
    else
    {
        // Prepare GUI
        glfwGetCursorPos(window, &cursorX, &cursorY);
        CenterCursor();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void CenterCursor()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width / 2, height / 2);
#if DEBUG_LEVEL <= DEBUG_LEVEL_INFO
    std::cout << "Centering window on: " << width / 2 << ", " << height / 2;
#endif
}