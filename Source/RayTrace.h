#pragma once

#include "Constants.h"
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <widemath.h>
#endif

#if defined(__linux__)
//
#endif

#if defined(__APPLE__)
//#include <widemath.h>
#endif

struct FMainData;
struct SDL_Surface;
struct FFigureRenderData;

class RayTrace
{
public:
	//
	static void GenerateRays(float2& InGenerateRaysFrom, FRay* InRayArray);
	
	//
	static void FillRaysOnSurface_ByRange(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, FRay* InRayArray, uint32_t StartFillFromRay, uint32_t EndFillToRay, uint32_t InColor);

	//
	static void FillRaysOnSurface_Async(FMainData& InMainData);
	//
	static void FillRaysOnSurface_Async_ParallelFor(FMainData& InMainData);
};