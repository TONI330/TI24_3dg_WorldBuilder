#include "World.h"
#include "StaticSettings.h"
#include "Object3d.h"
#include "EditGUI.h"
#include "WorldFactory.h"
#include "ObjectLight.h"

World::World(GLFWwindow& window, FpCam& camera) : window(window), camera(camera)
{
    time = 0;
    
    //add sun
    auto sun = new Object3d("models/sun.obj", "sun");
    sun->Scale(200);
    this->AddWorldObject(sun);

    //load world from file
    auto loadedObjects =  WorldFactory::LoadWorldObjects();
    this->objects.insert(objects.end(), loadedObjects.begin(), loadedObjects.end());

    for (auto worldObject : objects)
        worldObject->Init();

    tigl::shader->enableColor(false);
    tigl::shader->enableTexture(true);
    tigl::shader->enableLighting(true);
    lightCount++;
    tigl::shader->setLightCount(lightCount);

    tigl::shader->setLightAmbient(0, glm::vec3(0.1f, 0.1f, 0.15f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.8f, 0.8f, 0.8f));
    tigl::shader->setLightSpecular(0, glm::vec3(0, 0, 0));
    tigl::shader->setShinyness(32.0f);

    tigl::shader->enableFog(true);
    tigl::shader->setFogColor(glm::vec3(1, 1, 1));
    tigl::shader->setFogLinear(10, 10000);

    AddWorldObject(new ObjectLight(lightCount, lightCount +1));
    AddWorldObject(new ObjectLight(lightCount, lightCount +1));
}



void World::AddWorldObject(WorldObject* object)
{
    objects.push_back(object);
    object->id = objects.size();
}

float sunAngle;
void World::UpdateWorld()
{
    time++;
	for (auto worldObject : objects)
	{
		worldObject->Update();
	}

    //orbiting of sun
    auto sunTransform = this->GetWorldObjects<Object3d>()[0]->GetTransform();
    sunAngle = START_TIME + ((float)time / 60);   
    sunTransform->position.x = (SUN_DIST * -glm::cos(glm::radians(sunAngle)));
    sunTransform->position.y = (SUN_DIST * glm::sin(glm::radians(sunAngle)));
}

void World::DrawWorld()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, (float)SUN_DIST * 2);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera.getMatrix());

    glEnable(GL_DEPTH_TEST);
    glPointSize(10.0f);

    
    auto sunTransform = this->GetWorldObjects<Object3d>()[0]->GetTransform()->position;
    tigl::shader->setLightPosition(0, sunTransform);

#if DEBUG_LEVEL == DEBUG_LEVEL_VERBOSE
    std::cout << "angle: " << sunAngle << std::endl;
#endif
    
    for (auto worldObject : objects)
    {
        worldObject->Draw();
        tigl::shader->setLightDiffuse(0, glm::vec3(1.0f, 1.0f, 1.0f));
#if DEBUG_LEVEL == DEBUG_LEVEL_VERBOSE
        std::cout << worldObject->name << std::endl;
#endif
    }
}

WorldObject* World::FindObjectById(const int& id)
{
    for (auto object : objects)
    {
        if(object->id == id)
            return object;
    }
}

World::~World()
{
    for (auto worldObject : objects)
    {
        delete(worldObject);
    }
    objects.clear();
}
