// SavageCube.cpp : définit le point d'entrée de l'application.
//

#include "SavageCubeEngine.h"



int main()
{

	bh3d::WindowInfo winInfos;
	winInfos.width = 1280;
	winInfos.height = 720;
	winInfos.title = "SavageCube";
	winInfos.fullscreen = false;

	SavageCubeEngine engine(winInfos);
	engine.Init();
	engine.Run();


	return 0;
}
