#pragma once
#include <vector>
#include <SDL_video.h>
#include <SDL_surface.h>
#include <SDL_rect.h>
#include "RayTrace.h"
#include "ThreadPoolManager.h"

struct FFigureRenderData
{
	FFigureRenderData() {}
	FFigureRenderData(IFigure* InFigure, bool InAlwaysShouldUpdateOnScreen = true)
		: Figure(InFigure), bAlwaysShouldUpdateOnScreen(InAlwaysShouldUpdateOnScreen)
	{
	}

	IFigure* Figure = nullptr;
	bool bAlwaysShouldUpdateOnScreen = true;
	bool bShouldUpdateOnScreenAtTheNextFrame = true;
};

struct FMainData
{
	SDL_Window* Window = nullptr;
	SDL_Surface* Surface = nullptr;

	FThreadPoolManager* ThreadPoolManager = nullptr;

	FFigureRenderData* EraseRectRD;
	FRay* RaysArray = nullptr;

	float2 LastControllPosition = { 0, 0 };
	float2 GenerateRaysFromPosition = { 0,0 };

	// figure and should update the figure on a new frame
	std::vector<FFigureRenderData*> ObjectsToRender;
};

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	//
	static void DrawOneCircleOnSurface(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor);
	//
	static void DrawOneCircleOnSurface_Bresenham(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor);
	//
	static void DrawMultiCirclesOnSurface(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, uint32_t InColor);


	//
	static void DrawOneRectangeOnSurface(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor);
	//
	static void DrawMultiRectanglesOnSurface(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, uint32_t InColor);


	//
	static void DrawLineOnSurface(SDL_Surface* InSurface, float2& InStart, float2& InEnd, int InThickness, uint32_t InColor);


	//
	static void DrawOnePixelOnSurface(SDL_Surface* InSurface, const FRect& InPixelPosition, uint32_t InColor);
	//
	static void DrawMultiPixelsOnSurface(SDL_Surface* InSurface, const std::vector<FRect>& Pixels, uint32_t InColor);


	//
	static void DrawOneRayOnSurface_ByPixel(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor);
	//
	static void DrawOneRayOnSurface_ByPixelBuffer(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor);
	//
	static void DrawOneRayOnSurface_ByLine(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor);


	//
	static void MakeOneFrame(FMainData& InRenderData);

};

