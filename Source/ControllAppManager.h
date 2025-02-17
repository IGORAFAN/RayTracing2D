#pragma once
#include "Constants.h"

union SDL_Event;
struct FMainData;

class ControllAppManager
{
public:
	//
	ControllAppManager();
	//
	~ControllAppManager();
	//
	static void HandleInput(SDL_Event& InEvent, bool& InbRunTheApp, float2& OutDetectedControll);
	//
	static bool ApplyControll(float2 DetectedControll, FMainData& InMainData);

protected:
	static IFigure* CurrentlyControlledFigure;
};

