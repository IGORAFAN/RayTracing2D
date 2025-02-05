#pragma once

#include <widemath.h>
#include "Constants.h"
#include <vector>

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
};