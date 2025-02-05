
#include "RayTrace.h"
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <corecrt_math_defines.h>
#include "ProfileMertickCollector.h"
#include "RenderManager.h"

void RayTrace::GenerateRays(float2& InGenerateRaysFrom, FRay* InRayArray)
{
	//PROFILE_METRICS_COLLECTOR("RayTrace_GenerateRays");

	for (int i = 0; i <= RAYS_NUMBERS; ++i)
	{
		double Angle = ((double)i / RAYS_NUMBERS) * 2 * M_PI;
		FRay Ray = { InGenerateRaysFrom.X, InGenerateRaysFrom.Y, Angle };
		InRayArray[i] = Ray;
	}
}

void RayTrace::FillRaysOnSurface_ByRange(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, FRay* InRayArray, uint32_t StartFillFromRay, uint32_t EndFillToRay, uint32_t InColor)
{
	PROFILE_METRICS_COLLECTOR("RayTrace_FillRaysOnSurface");

	for (uint32_t i = StartFillFromRay; i <= EndFillToRay; ++i)
	{
		FRay& Ray = InRayArray[i];
		RenderManager::DrawOneRayOnSurface_ByLine(InSurface, InObjectsArray, Ray, InColor);
	}
}

void RayTrace::FillRaysOnSurface_Async(FMainData& InMainData)
{
	int RayStep = RAYS_NUMBERS / THREADS_NUMBERS;
	int StartFillFromRay = 0;
	int EndFillToRay = RayStep - 1;
	for (int i = 0; i < THREADS_NUMBERS; ++i)
	{
		InMainData.ThreadPoolManager->AddTask(
			[StartFillFromRay, EndFillToRay](void* InData)
			{
				//DebugTrace("Start from " + std::to_string(StartFillFromRay) + " to " + std::to_string(EndFillToRay));
				FMainData* Data = static_cast<FMainData*>(InData);
				RayTrace::FillRaysOnSurface_ByRange(Data->Surface, Data->ObjectsToRender, Data->RaysArray, StartFillFromRay, EndFillToRay, COLOR_YELLOW);
			},
			&InMainData
		);
		StartFillFromRay = StartFillFromRay + RayStep;
		EndFillToRay = StartFillFromRay + RayStep - 1;
	}

	InMainData.ThreadPoolManager->WaitUntilAllTasksFinished();
}
