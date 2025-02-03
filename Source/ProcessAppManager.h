#pragma once
#include <vector>
#include <SDL_video.h>
#include <SDL_surface.h>
#include <SDL_rect.h>
#include "RayTrace.h"
#include "ThreadPoolManager.h"

struct FMainData
{
	SDL_Window* Window = nullptr;
	SDL_Surface* Surface = nullptr;

	FThreadPoolManager* ThreadPoolManager = nullptr;

	FRect* EraseRect = nullptr;
	FRay* RaysArray = nullptr;

	float2 LastControllPosition = { 0, 0 };
	float2 GenerateRaysFromPosition = { 0,0 };

	std::vector<IFigure*> ObjectsToRender;
};

union SDL_Event;
struct IFigure;

class ProcessAppManager
{

public:
	//
	static void HandleInput(SDL_Event& InEvent, bool& InbRunTheApp, float2& OutDetectedControll);
	//
	static bool ApplyControll(float2 DetectedControll, FMainData& InMainData);
	//
	static void MakeOneFrame(FMainData& InRenderData);

protected:
	static IFigure* CurrentlyControlledFigure;
};

