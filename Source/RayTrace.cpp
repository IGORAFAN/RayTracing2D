
#include "RayTrace.h"
#include "ProfileMertickCollector.h"
#include "RenderManager.h"

#if defined(_WIN32) || defined(_WIN64)
#include <corecrt_math_defines.h>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <tbb/parallel_for.h>
#endif

#if defined(__linux__)
//
#endif

#if defined(__APPLE__)
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <tbb/parallel_for.h>
//#include <corecrt_math_defines.h>
#endif


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
		if (FRenderManager* FRenderManager = FRenderManager::GetInstance())
		{
			FRenderManager->DrawOneRayOnSurface_ByPixel(InSurface, InObjectsArray, Ray, InColor);
		}
	}
}

void RayTrace::FillRaysOnSurface_Async(FMainData& InMainData)
{
	{
		PROFILE_METRICS_COLLECTOR("FillRaysOnSurface_Async_UseThreadPool");

		int RayStep = RAYS_NUMBERS / THREADS_NUMBERS;
		int StartFillFromRay = 0;
		int EndFillToRay = RayStep - 1;
		for (int i = 0; i < THREADS_NUMBERS; ++i)
		{
			//PROFILE_METRICS_COLLECTOR("FillRaysOnSurface_Async_Iteration");
			InMainData.ThreadPoolManager->AddTask(
				[StartFillFromRay, EndFillToRay](void* InData)
				{
					//LogDebugTrace("Start from " + std::to_string(StartFillFromRay) + " to " + std::to_string(EndFillToRay));
					FMainData* Data = static_cast<FMainData*>(InData);
					RayTrace::FillRaysOnSurface_ByRange(Data->Surface, Data->ObjectsToRender, Data->RaysArray, StartFillFromRay, EndFillToRay, COLOR_YELLOW);
				},
				&InMainData
			);
			StartFillFromRay = StartFillFromRay + RayStep;
			EndFillToRay = StartFillFromRay + RayStep - 1;
		}
	}

	{
		PROFILE_METRICS_COLLECTOR("WaitUntilAllTasksFinished")
		InMainData.ThreadPoolManager->WaitUntilAllTasksFinished();
	}
}

void RayTrace::FillRaysOnSurface_Async_ParallelFor(FMainData& InMainData)
{
	PROFILE_METRICS_COLLECTOR("FillRaysOnSurface_Async_ParallelFor");

	tbb::parallel_for(
		tbb::blocked_range<size_t>(0, RAYS_NUMBERS),
		[&InMainData](const tbb::blocked_range<size_t>& InRange)
		{
			PROFILE_METRICS_COLLECTOR("InParallel_for");

			for (auto i = InRange.begin(); i < InRange.end(); ++i)
			{
				if (FRenderManager* FRenderManager = FRenderManager::GetInstance())
				{
					FRenderManager->DrawOneRayOnSurface_ByPixel(InMainData.Surface, InMainData.ObjectsToRender, InMainData.RaysArray[i], COLOR_YELLOW);
				}
			}
		});
}
