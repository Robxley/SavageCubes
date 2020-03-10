
#pragma once


#include "imgui.h"

inline void SavageCubeEditor()
{
	
    static bool show_demo_window = false;
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {
        ImGui::Begin("Savage Cube Editor");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Checkbox("Demo Window", &show_demo_window);

        if (ImGui::IsAnyWindowHovered())
            ImGui::Text("A item is hovered");
        else
            ImGui::Text("A item isn't hovered !");

        ImGui::End();
    }

}

