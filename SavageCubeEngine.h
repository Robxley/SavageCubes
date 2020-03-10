#pragma once

#include "BH3D_SDLEngine.hpp"
#include "BH3D_SDLImGUI.hpp"
#include "BH3D_Camera.hpp"
#include "SavageCubeMatrix.h"
#include "SavageCubeEditor.h"

class SavageCubeEngine : public bh3d::SDLEngine
{
	bh3d::SDLImGUI m_sdlImGUI;

	SavageCubeMatrix m_floor;
	SavageCubeMatrix m_savageCubes = { glm::vec3{0.99f,0.99f,0.99f} };

	glm::ivec2 floor_size		= {	8,	32};
	glm::ivec2 savageCube_size	= { 8,	16 };

public:
	SavageCubeEngine(const bh3d::WindowInfo& winInfos) : 
		bh3d::SDLEngine(winInfos)
	{
		

	}

	void Init() override;
	void Display() override;

};