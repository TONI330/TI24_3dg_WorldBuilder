#include "EditGUI.h"
#include "../lib/imgui-1.87/imgui.h"
#include "../lib/imgui-1.87/imgui_impl_glfw.h"
#include "../lib/imgui-1.87/imgui_impl_opengl3.h"
#include "Object3d.h"
#include "ObjectLight.h"
#include <sstream>
#include "StaticSettings.h"

EditGUI::EditGUI(World& world, GLFWwindow& window) : world(world), window(window)
{
    // Initialize GUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(&window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();
}
int selectedId = 0;
void EditGUI::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImVec4 active = ACTIVE_BUTTON_COLOR;
    static ImVec4 inactive = INACTIVE_BUTTON_COLOR;
    
    ImGui::Text("3d Objects:");

    std::vector<Object3d*> objects3d = world.GetWorldObjects<Object3d>();
    for (int i = 0; i < objects3d.size(); i++)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, selectedId == i ? active : inactive);
        if (ImGui::Button(objects3d[i]->name.c_str(), BUTTON_SIZE))
        {
            ImGui::Button(objects3d[i]->name.c_str(), BUTTON_SIZE);
            selectedId = world.FindObjectById(objects3d[i]->id)->id;
        }
        ImGui::PopStyleColor();
    }

    ImGui::Text("Lights:");
    std::vector<ObjectLight*> lightObjects = world.GetWorldObjects<ObjectLight>();
    for (int i = 0; i < lightObjects.size(); i++)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, selectedId == i ? active : inactive);
        std::string lightName = "Light ";
        lightName += std::to_string(lightObjects[i]->lightId);
        if (ImGui::Button(lightName.c_str(), BUTTON_SIZE))
        {
            ImGui::Button(lightName.c_str(), BUTTON_SIZE);
            selectedId = world.FindObjectById(lightObjects[i]->id)->id;
        }
        ImGui::PopStyleColor();
    }
    
    ImGui::Text("Change position:");
    ImGui::DragFloat("X position", &world.FindObjectById(selectedId)->GetTransform()->position.x, SLIDER_SENSITIVITY);
    ImGui::DragFloat("Y position", &world.FindObjectById(selectedId)->GetTransform()->position.y, SLIDER_SENSITIVITY);
    ImGui::DragFloat("Z position", &world.FindObjectById(selectedId)->GetTransform()->position.z, SLIDER_SENSITIVITY);

    ImGui::Text("Change size:");
    ImGui::DragFloat("X scale", &world.FindObjectById(selectedId)->GetTransform()->scale.x, SLIDER_SENSITIVITY / SLIDER_SCALE_MODIFIER);
    ImGui::DragFloat("Y scale", &world.FindObjectById(selectedId)->GetTransform()->scale.y, SLIDER_SENSITIVITY / SLIDER_SCALE_MODIFIER);
    ImGui::DragFloat("Z scale", &world.FindObjectById(selectedId)->GetTransform()->scale.z, SLIDER_SENSITIVITY / SLIDER_SCALE_MODIFIER);

    ImGui::Text("Change rotation:");
    ImGui::DragFloat("X rotation", &world.FindObjectById(selectedId)->GetTransform()->rotation.x, SLIDER_SENSITIVITY / SLIDER_ROTATION_MODIFIER);
    ImGui::DragFloat("Y rotation", &world.FindObjectById(selectedId)->GetTransform()->rotation.y, SLIDER_SENSITIVITY / SLIDER_ROTATION_MODIFIER);
    ImGui::DragFloat("Z rotation", &world.FindObjectById(selectedId)->GetTransform()->rotation.z, SLIDER_SENSITIVITY / SLIDER_ROTATION_MODIFIER);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

EditGUI::~EditGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}
