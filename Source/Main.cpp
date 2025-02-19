// #include SDL_MAIN_HANDLED
#include "Constants.h"
#include "RenderManager.h"
#include "ThreadPoolManager.h"
#include "Helper.h"
#include "ControllAppManager.h"
#include "LogManager.h"

#if defined(_WIN32) || defined(_WIN64)
#include <SDL_init.h>
#include <SDL_hints.h>
#endif

#if defined(__linux__)
//
#endif

#if defined(__APPLE__)
#include <SDL3/SDL.h>
#endif

//int main(int argc, char *argv[])
//{
//	if (not SDL_Init(SDL_INIT_VIDEO))
//	{
//		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
//		return 1;
//	}
//
//	SDL_Window *window = SDL_CreateWindow("SDL3 Renderer Test", 800, 600, SDL_WINDOW_RESIZABLE);
//	if (not window)
//	{
//		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
//		SDL_Quit();
//		return 1;
//	}
//
//	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
//	if (not renderer)
//	{
//		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//		SDL_DestroyWindow(window);
//		SDL_Quit();
//		return 1;
//	}
//
//	bool running = true;
//	SDL_Event event;
//
//	while (running)
//	{
//		while (SDL_PollEvent(&event))
//		{
//			if (event.type == SDL_EVENT_QUIT)
//			{
//				running = false;
//			}
//		}
//
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Чёрный цвет
//		SDL_RenderClear(renderer);
//
//		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Зелёный
//		const SDL_FRect rect = {200, 150, 400, 300};
//		SDL_RenderFillRect(renderer, &rect); // Рисуем зелёный прямоугольник
//
//		SDL_RenderPresent(renderer); // Обновление экрана
//	}
//
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//
//	return 0;
//}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	FMainData MainData;
	MainData.Window = SDL_CreateWindow("RayTracing", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_FOCUS);
	MainData.Renderer = SDL_CreateRenderer(MainData.Window, SDL_SOFTWARE_RENDERER);
	MainData.Surface = SDL_GetWindowSurface(MainData.Window);

	SDL_SetRenderDrawColor(MainData.Renderer, 0, 0, 0, 255);

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal"); //или "opengl"

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

	FControllAppManager* ControllAppManager = FControllAppManager::GetInstance();
	FRenderManager* RenderManager = FRenderManager::GetInstance();

	// Render the first frame
	float2 DetectedControll = { ControlledCircle.X, ControlledCircle.Y };
	MainData.GenerateRaysFromPosition = DetectedControll;
	unsigned long long int FrameCounter = 0;
	++FrameCounter;
	RenderManager->MakeOneFrame(MainData);


	SDL_Event Event;
	bool bRunTheApp = true;
	while (bRunTheApp)
	{
		if (ControllAppManager && RenderManager)
		{
			ControllAppManager->HandleInput(Event, bRunTheApp, DetectedControll);

			//ControllAppManager::ApplyControll(DetectedControll, MainData);
			if (ControllAppManager->ApplyControll(DetectedControll, MainData))
			{
				LogManager::LogDebugTrace("\nFrame: " + std::to_string(++FrameCounter));

				RenderManager->MakeOneFrame(MainData);
			}
		}
	}

	SDL_DestroyRenderer(MainData.Renderer);
	SDL_DestroyWindow(MainData.Window);
	SDL_Quit();

	return 0;
 }