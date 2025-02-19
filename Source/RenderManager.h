#pragma once

#include <vector>
#include "RayTrace.h"
#include "ThreadPoolManager.h"

#if defined(_WIN32) || defined(_WIN64)
#include <SDL_video.h>
#include <SDL_surface.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#endif

#if defined(__linux__)
//
#endif

#if defined(__APPLE__)
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#endif

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
	SDL_Renderer* Renderer = nullptr;
	SDL_Surface* Surface = nullptr;

	SDL_Texture* RenderTexture = nullptr;

	FThreadPoolManager* ThreadPoolManager = nullptr;

	FFigureRenderData* EraseRectRD;
	FRay* RaysArray = nullptr;

	float2 LastControllPosition = { 0, 0 };
	float2 GenerateRaysFromPosition = { 0,0 };

	// figure and should update the figure on a new frame
	std::vector<FFigureRenderData*> ObjectsToRender;
};

class FRenderManager
{
public:
	static FRenderManager* GetInstance();

	//
	void DrawOneCircleOnSurface(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor);
	//
	void DrawOneCircleOnSurface_Bresenham(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor);
	//
	void DrawMultiCirclesOnSurface(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, uint32_t InColor);


	//
	void DrawOneRectangeOnSurface(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor);
	//
	void DrawMultiRectanglesOnSurface(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, uint32_t InColor);


	//
	void DrawLineOnSurface(SDL_Surface* InSurface, float2& InStart, float2& InEnd, int InThickness, uint32_t InColor);


	//
	void DrawOnePixelOnSurface(SDL_Surface* InSurface, const FRect& InPixelPosition, uint32_t InColor);
	//
	void DrawMultiPixelsOnSurface(SDL_Surface* InSurface, const std::vector<FRect>& Pixels, uint32_t InColor);


	//
	void DrawOneRayOnSurface_ByPixel(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor);
	//
	void DrawOneRayOnSurface_ByPixelBuffer(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor);
	//
	void DrawOneRayOnSurface_ByLine(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor);


	//
	void MakeOneFrame(FMainData& InRenderData);

private:
	FRenderManager();
	~FRenderManager();

	FRenderManager(const FRenderManager&) = delete;
	FRenderManager(const FRenderManager&&) = delete;
	FRenderManager& operator=(const FRenderManager&) = delete;
	FRenderManager& operator=(const FRenderManager&&) = delete;
};

