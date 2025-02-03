//#include SDL_MAIN_HANDLED

#include <SDL_init.h>
#include "RayTrace.h"
#include "Constants.h"
#include "ProcessAppManager.h"
#include "ThreadPoolManager.h"

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	
	FMainData MainData;
	MainData.Window = SDL_CreateWindow("RayTracing", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);
	MainData.Surface = SDL_GetWindowSurface(MainData.Window);
	
	FRect EraseRect = { 0, 0, WIDTH, HEIGHT };
	MainData.EraseRect = &EraseRect;

	FCircle ControlledCircle = { WIDTH / 3, HEIGHT / 2, 20};
	ControlledCircle.bTransparent = true;
	ControlledCircle.bDoesItEmitLight = true;
	MainData.ObjectsToRender.push_back(&ControlledCircle);
	
	FCircle Circle = { 500, 225, 100 };
	MainData.ObjectsToRender.push_back(&Circle);

	FRect Rect = { 300, 400, 100, 40 };
	MainData.ObjectsToRender.push_back(&Rect);

	FRay Rays[RAYS_NUMBERS];
	MainData.RaysArray = &Rays[0];

	FThreadPoolManager ThreadPoolManager(THREADS_NUMBERS);
	MainData.ThreadPoolManager = &ThreadPoolManager;

	// Render the first frame
	float2 DetectedControll = { ControlledCircle.X, ControlledCircle.Y };
	MainData.GenerateRaysFromPosition = DetectedControll;
	ProcessAppManager::MakeOneFrame(MainData);

	SDL_Event Event;
	bool bRunTheApp = true;
	while (bRunTheApp)
	{
		ProcessAppManager::HandleInput(Event, bRunTheApp, DetectedControll);

		if (ProcessAppManager::ApplyControll(DetectedControll, MainData))
		{
			ProcessAppManager::MakeOneFrame(MainData);
		}
	}

	SDL_DestroyWindow(MainData.Window);
	SDL_Quit();

	return 0;
}