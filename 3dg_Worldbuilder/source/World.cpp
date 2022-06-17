#include "World.h"
#include "StaticSettings.h"
#include "Object3d.h"
#include "EditGUI.h"
#include "WorldFactory.h"
#include "ObjectLight.h"

World::World(GLFWwindow& window, FpCam& camera) : window(window), camera(camera)
{
    time = 0;

    //load world from file
    

    //add sun
    auto sun = new Object3d("models/sun.obj", "sun");
    sun->Scale(200);
    this->AddWorldObject(sun);

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
    AddWorldObject(new ObjectLight(lightCount));
    AddWorldObject(new ObjectLight(lightCount));
}



void World::AddWorldObject(WorldObject* object)
{
    objects.push_back(object);
    object->id = objects.size();
}

float sunAngle;
void World::UpdateWorld()
{
	for (auto worldObject : objects)
	{
		worldObject->Update();
	}

    auto sunTransform = this->GetWorldObjects<Object3d>()[0]->GetTransform();
    //TODO orbit sun
    sunAngle = 90 + ((float)time / 60);
    //sunAngle = 270;
    sunTransform->position.x = (8000 * -glm::cos(glm::radians(sunAngle)));
    sunTransform->position.y = (8000 * glm::sin(glm::radians(sunAngle)));
	time++;
}

void World::DrawWorld()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 16000.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera.getMatrix());
    //tigl::shader->setModelMatrix(glm::translate(glm::mat4(1.0f), { 0.0f, 10.0f, 10.0f }));

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
