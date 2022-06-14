#include "EditGUI.h"
#include "lib/imgui-1.87/imgui.h"
#include "lib/imgui-1.87/imgui_impl_glfw.h"
#include "lib/imgui-1.87/imgui_impl_opengl3.h"
#include "Object3d.h"


EditGUI::EditGUI(World& world, GLFWwindow& window) : world(world), window(window)
{
    // Initialize GUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(&window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    //ImGuiIO& io = ImGui::GetIO(); (void)io;

    //io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;  // Enable Keyboard Controls
    //io.KeyMap[ImGuiKey_Tab] = 258;
    //io.AddInputCharacter(ImGuiKey_Tab);

}
int index = 0;
void EditGUI::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImVec4 active = { 0, 0, 0, 0.5f };
    static ImVec4 inactive = { 0, 0, 0, 0.3f };
    
    std::vector<Object3d*> objects3d = world.GetWorldObjects<Object3d>();
    for (int i = 0; i < world.objects.size(); i++)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, index == i ? active : inactive);
        if (ImGui::Button(objects3d[i]->name.c_str(), ImVec2(230 - 15, 41)))
        {
            ImGui::Button(objects3d[i]->name.c_str(), ImVec2(230 - 15, 41));
            index = i;
        }
        ImGui::PopStyleColor();
    }
    
    ImGui::DragFloat("X position", &world.objects[index]->GetTransform()->position.x, 1.0f);
    ImGui::DragFloat("Y position", &world.objects[index]->GetTransform()->position.y, 1.0f);
    ImGui::DragFloat("Z position", &world.objects[index]->GetTransform()->position.z, 1.0f);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

EditGUI::~EditGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}
