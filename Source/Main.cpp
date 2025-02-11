//#include SDL_MAIN_HANDLED

#include "Constants.h"
#include "RenderManager.h"
#include "ThreadPoolManager.h"
#include "Helper.h"
#include "ControllAppManager.h"
#include <SDL_init.h>

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	
	FMainData MainData;
	MainData.Window = SDL_CreateWindow("RayTracing", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);
	MainData.Surface = SDL_GetWindowSurface(MainData.Window);
	
	FRect EraseRect = { 0, 0, WIDTH, HEIGHT };
	FFigureRenderData EraseRectRD(&EraseRect, true);
	MainData.EraseRectRD = &EraseRectRD;

	FCircle ControlledCircle = { WIDTH / 3, HEIGHT / 2, 20};
	ControlledCircle.bTransparent = true;
	ControlledCircle.bDoesItEmitLight = true;
	
	FFigureRenderData ControlledCircleRD(&ControlledCircle, false);
	MainData.ObjectsToRender.push_back(&ControlledCircleRD);
	
	FCircle Circle = { 500, 225, 100 };
	FFigureRenderData CircleRD(&Circle, false);
	MainData.ObjectsToRender.push_back(&CircleRD);

	FRect Rect = { 300, 400, 100, 40 };
	FFigureRenderData RectRD(&Rect, false);
	MainData.ObjectsToRender.push_back(&RectRD);

	FRay Rays[RAYS_NUMBERS];
	MainData.RaysArray = &Rays[0];

	FThreadPoolManager ThreadPoolManager(THREADS_NUMBERS);
	MainData.ThreadPoolManager = &ThreadPoolManager;


	// Render the first frame
	float2 DetectedControll = { ControlledCircle.X, ControlledCircle.Y };
	MainData.GenerateRaysFromPosition = DetectedControll;

	unsigned long long int FrameCounter = 0;
	++FrameCounter;

	FRenderManager* RenderManager = FRenderManager::GetInstance();
	if (RenderManager) RenderManager->MakeOneFrame(MainData);

	FControllAppManager* ControllAppManager = FControllAppManager::GetInstance();

	SDL_Event Event;
	bool bRunTheApp = true;
	while (bRunTheApp)
	{
		if (ControllAppManager && RenderManager)
		{
			ControllAppManager->HandleInput(Event, bRunTheApp, DetectedControll);

			//ControllAppManager->ApplyControll(DetectedControll, MainData);
			if (ControllAppManager->ApplyControll(DetectedControll, MainData))
			{
				DebugTrace("Frame: " + std::to_string(++FrameCounter));

				RenderManager->MakeOneFrame(MainData);
			}
		}
	}

	SDL_DestroyWindow(MainData.Window);
	SDL_Quit();

	return 0;
}