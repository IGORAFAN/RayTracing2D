#pragma once

#include <widemath.h>
#include <vector>

struct FMainData;
struct SDL_Surface;
struct FFigureRenderData;
struct float2;
struct FRay;

class RayTrace
{
public:
	//
	static void GenerateRays(float2& InGenerateRaysFrom, FRay* InRayArray);
	
	//
	static void FillRaysOnSurface_ByRange(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, FRay* InRayArray, uint32_t StartFillFromRay, uint32_t EndFillToRay, uint32_t InColor);

	//
	static void FillRaysOnSurface_Async_UseThreadPool(FMainData& InMainData);
	
	//
	static void FillRaysOnSurface_Async_ParallelFor(FMainData& InMainData);
};