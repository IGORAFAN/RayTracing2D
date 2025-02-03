#include "ProcessAppManager.h"
#include <SDL_events.h>
#include "Helper.h"
#include "ThreadPoolManager.h"
#include "ProfileMertickCollector.h"

IFigure* ProcessAppManager::CurrentlyControlledFigure = nullptr;

void ProcessAppManager::HandleInput(SDL_Event& InEvent, bool& InbRunTheApp, float2& OutDetectedControll)
{
	while (SDL_PollEvent(&InEvent))
	{
		if (InEvent.type == SDL_EVENT_QUIT)
		{
			InbRunTheApp = false;
		}
		if (InEvent.type == SDL_EVENT_MOUSE_MOTION && InEvent.motion.state != 0)
		{
			OutDetectedControll.X = InEvent.motion.x;
			OutDetectedControll.Y = InEvent.motion.y;
		}
		if (InEvent.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			CurrentlyControlledFigure = nullptr;
			//DebugTrace("SDL_EVENT_MOUSE_BUTTON_UP completed");
		}
	}
}

void ProcessAppManager::MakeOneFrame(FMainData& InRenderData)
{
	PROFILE_METRICS_COLLECTOR("ProcessAppManager_MakeOneFrame");

	// Erase the screen
	if (InRenderData.EraseRect) SDL_FillSurfaceRect(InRenderData.Surface, &InRenderData.EraseRect->Rect, COLOR_BLACK);
	// Generate new rays
	RayTrace::GenerateRays(InRenderData.GenerateRaysFromPosition, InRenderData.RaysArray);
	// Fill the rays on the screen
	RayTrace::FillRaysOnSurface(InRenderData.Window, InRenderData.Surface, InRenderData.ObjectsToRender, InRenderData.RaysArray, COLOR_GRAY);
	// Fill the rectangles
	RayTrace::FillRectanglesOnSurface(InRenderData.Window, InRenderData.Surface, InRenderData.ObjectsToRender, COLOR_WHITE);
	// Fill the circles
	RayTrace::FillCirclesOnSurface(InRenderData.Window, InRenderData.Surface, InRenderData.ObjectsToRender, COLOR_WHITE);
	// Update the frame
	SDL_UpdateWindowSurface(InRenderData.Window);
	// Hard Vsync
	//SDL_Delay(16.6);
}

bool ProcessAppManager::ApplyControll(float2 InNewDetectedControllPosition, FMainData& InMainData)
{
	if ((int)InMainData.LastControllPosition.X != (int)InNewDetectedControllPosition.X ||
		(int)InMainData.LastControllPosition.Y != (int)InNewDetectedControllPosition.Y)
	{
		for (IFigure* CurrentFigure : InMainData.ObjectsToRender)
		{
			if (CurrentFigure)
			{
				if (CurrentlyControlledFigure != CurrentFigure && CurrentFigure->IsIn(InNewDetectedControllPosition))
				{
					CurrentlyControlledFigure = CurrentFigure;

					return false;
				}
				else if (CurrentlyControlledFigure == CurrentFigure)
				{
					CurrentFigure->Move(InNewDetectedControllPosition);

					InMainData.LastControllPosition.X = InNewDetectedControllPosition.X;
					InMainData.LastControllPosition.Y = InNewDetectedControllPosition.Y;

					if (CurrentFigure->bDoesItEmitLight)
					{
						InMainData.GenerateRaysFromPosition.X = InNewDetectedControllPosition.X;
						InMainData.GenerateRaysFromPosition.Y = InNewDetectedControllPosition.Y;
					}

					return true;
				}
			}
		}
	}
	return false;
}
