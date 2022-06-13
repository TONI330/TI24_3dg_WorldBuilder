#include "World.h"
#include "StaticSettings.h"

World::World(GLFWwindow& window, FpCam& camera) : window(window), camera(camera)
{
    time = 0;
    for (auto worldObject : objects)
        worldObject->Init();
}

void World::AddWorldObject(WorldObject* object)
{
    objects.push_back(object);
}

void World::UpdateWorld()
{
	for (auto worldObject : objects)
	{
		worldObject->Update();
	}
	time++;
    camera.update(&window);
}

void World::DrawWorld()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera.getMatrix());
    tigl::shader->setModelMatrix(glm::scale(glm::mat4(1.0f), {0.2f, 0.2f, 0.2f}));

    //tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);
    glPointSize(10.0f);
    tigl::shader->enableTexture(true);

    for (auto worldObject : objects)
    {
        worldObject->Draw();
#if DEBUG_LEVEL == 0
        std::cout << worldObject->name << std::endl;
#endif
    }
}
