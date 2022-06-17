#include "ObjectLight.h"
#include "tigl.h"

ObjectLight::ObjectLight(int& lightsAmount, const int lightId) : lightId(lightId)
{
	lightsAmount++;
	tigl::shader->setLightCount(lightsAmount);
	tigl::shader->setLightDirectional(lightId, false);
}

ObjectLight::ObjectLight(const int lightId)
{
	tigl::shader->setLightCount(lightId);
	tigl::shader->setLightDirectional(lightId, false);
}

void ObjectLight::Update()
{
	tigl::shader->setLightPosition(lightId, this->GetTransform()->position);
}

void ObjectLight::Draw()
{
	tigl::shader->setLightDiffuse(lightId, glm::vec3(0.8f, 0.8f, 0.8f));
}
