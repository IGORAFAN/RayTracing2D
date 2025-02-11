#pragma once
#include "Constants.h"

union SDL_Event;
struct FMainData;

class FControllAppManager
{
public:
	static FControllAppManager* GetInstance();

	//
	static void HandleInput(SDL_Event& InEvent, bool& InbRunTheApp, float2& OutDetectedControll);
	//
	static bool ApplyControll(float2 DetectedControll, FMainData& InMainData);

private:
	FControllAppManager();
	FControllAppManager(const FControllAppManager&) = delete;
	FControllAppManager(const FControllAppManager&&) = delete;
	
	~FControllAppManager();

	FControllAppManager& operator=(const FControllAppManager&) = delete;
	FControllAppManager& operator=(const FControllAppManager&&) = delete;

protected:
	static IFigure* CurrentlyControlledFigure;
};

