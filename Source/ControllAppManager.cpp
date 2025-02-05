#include "ControllAppManager.h"
#include <SDL_events.h>
#include "Constants.h"
#include "RenderManager.h"

IFigure* ControllAppManager::CurrentlyControlledFigure = nullptr;

ControllAppManager::ControllAppManager()
{

}

ControllAppManager::~ControllAppManager()
{

}

void ControllAppManager::HandleInput(SDL_Event& InEvent, bool& InbRunTheApp, float2& OutDetectedControll)
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

bool ControllAppManager::ApplyControll(float2 InNewDetectedControllPosition, FMainData& InMainData)
{
	if ((int)InMainData.LastControllPosition.X != (int)InNewDetectedControllPosition.X ||
		(int)InMainData.LastControllPosition.Y != (int)InNewDetectedControllPosition.Y)
	{
		for (FFigureRenderData* CurrentFigureRD : InMainData.ObjectsToRender)
		{
			if (CurrentFigureRD && CurrentFigureRD->Figure)
			{
				if (CurrentlyControlledFigure != CurrentFigureRD->Figure &&
					CurrentFigureRD->Figure->IsIn(InNewDetectedControllPosition))
				{
					CurrentlyControlledFigure = CurrentFigureRD->Figure;

					return false;
				}
				else if (CurrentlyControlledFigure == CurrentFigureRD->Figure)
				{
					CurrentFigureRD->Figure->Move(InNewDetectedControllPosition);
					//CurrentFigureRD->bShouldUpdateOnScreenAtTheNextFrame = true;

					InMainData.LastControllPosition.X = InNewDetectedControllPosition.X;
					InMainData.LastControllPosition.Y = InNewDetectedControllPosition.Y;

					if (CurrentFigureRD->Figure->bDoesItEmitLight)
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