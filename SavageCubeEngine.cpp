#include "SavageCubeEngine.h"
#include "BH3D_DemoImGUI.hpp"

void SavageCubeEngine::Init()
{
	bh3d::SDLEngine::Init();
	m_sdlImGUI.InitContext(*this);

	m_cameraEngine.LookAt();

	m_floor.Init(floor_size.y, floor_size.x);
	m_savageCubes.Init(savageCube_size.y, savageCube_size.x);
}

void SavageCubeEngine::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_sdlImGUI.Frame();
	{
		ImGui::Begin("Camera debug");
		bh3d::SDLImGUI::CameraManager(m_cameraEngine);
		ImGui::End();
	}

	SavageCubeEditor();

	if(!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemHovered())
		m_cameraEngine.LookAround(m_mouse);

	this->m_floor.Draw(m_cameraEngine.ProjViewTransform());
	this->m_savageCubes.DrawAnimation(m_cameraEngine.ProjViewTransform());

	//Update the camera with the mouse deplacement/events


	m_sdlImGUI.Render();


	glUseProgram(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}