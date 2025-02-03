#pragma once

#include <widemath.h>
#include "Constants.h"
#include <vector>

struct SDL_Window;
struct SDL_Surface;

class RayTrace
{
public:
	//
	static void GenerateRays(float2& InGenerateRaysFrom, FRay* InRayArray);
	//
	static void FillCirclesOnSurface(SDL_Window* InWindow, SDL_Surface* InSurface, const std::vector<IFigure*>& InObjectsArray, uint32_t InColor);
	//
	static void FillRectanglesOnSurface(SDL_Window* InWindow, SDL_Surface* InSurface, const std::vector<IFigure*>& InObjectsArray, uint32_t InColor);
	//
	static void FillRaysOnSurface(SDL_Window* Window, SDL_Surface* InSurface, const std::vector<IFigure*>& InObjectsArray, FRay* InRayArray, uint32_t InColor);
};