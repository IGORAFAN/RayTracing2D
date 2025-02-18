
#include "RenderManager.h"
#include "ThreadPoolManager.h"
#include "ProfileMertickCollector.h"
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <SDL_surface.h>
#endif

#if defined(__linux__)
//
#endif

#if defined(__APPLE__)
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_render.h>
#endif

RenderManager::RenderManager()
{

}

RenderManager::~RenderManager()
{

}

void RenderManager::DrawOneCircleOnSurface(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor)
{
	//if (InFigureRD.bShouldUpdateOnScreenAtTheNextFrame)
	//{
		if (FCircle* Circle = static_cast<FCircle*>(InFigureRD.Figure))
		{
			int startX	= static_cast<int>(Circle->X) - static_cast<int>(Circle->R);
			int finishX	= static_cast<int>(Circle->X) + static_cast<int>(Circle->R);
			int startY	= static_cast<int>(Circle->Y) - static_cast<int>(Circle->R);
			int finishY = static_cast<int>(Circle->Y) + static_cast<int>(Circle->R);

			for (int X = startX; X <= finishX; ++X)
			{
				for (int Y = startY; Y <= finishY; ++Y)
				{
					float2 CurrentPosition = { static_cast<float>(X), static_cast<float>(Y) };
					if (Circle->IsIn(CurrentPosition))
					{
						FRect CurrentPixel = { X, Y, 1, 1 };
						RenderManager::DrawOnePixelOnSurface(InSurface, CurrentPixel, InColor);
						//if (InFigureRD.bAlwaysShouldUpdateOnScreen == false)
						//{
						//	InFigureRD.bShouldUpdateOnScreenAtTheNextFrame = false;
						//}
					}
				}
			}
		}
	//}
}

void RenderManager::DrawOneCircleOnSurface_Bresenham(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor)
{
	if (FCircle* Circle = static_cast<FCircle*>(InFigureRD.Figure))
	{
		int cx	= static_cast<int>(Circle->X);
		int cy	= static_cast<int>(Circle->Y);
		int r	= static_cast<int>(Circle->R);
		int r2	= r * r;

		for (int y = -r; y <= r; ++y)
		{
			int dx = static_cast<int>(std::sqrt(r2 - y * y));
			int xStart = cx - dx;
			int xEnd = cx + dx;

			FRect LineRect = { xStart, cy + y, (xEnd - xStart + 1), 1 };
			RenderManager::DrawOnePixelOnSurface(InSurface, LineRect, InColor);
		}
	}
}

void RenderManager::DrawMultiCirclesOnSurface(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, uint32_t InColor)
{
	//PROFILE_METRICS_COLLECTOR("RenderManager_DrawMultiCirclesOnSurface");

	for (FFigureRenderData* FigureRD : InObjectsArray)
	{
		if (FigureRD) DrawOneCircleOnSurface_Bresenham(InSurface, *FigureRD, InColor);
	}
}

void RenderManager::DrawOneRectangeOnSurface(SDL_Surface* InSurface, FFigureRenderData& InFigureRD, uint32_t InColor)
{
	//PROFILE_METRICS_COLLECTOR("RenderManager_DrawOneRectangeOnSurface");

	//if (InFigureRD.bShouldUpdateOnScreenAtTheNextFrame)
	//{
		if (FRect* Rect = static_cast<FRect*>(InFigureRD.Figure))
		{
			SDL_FillSurfaceRect(InSurface, &Rect->Rect, InColor);
			//if (InFigureRD.bAlwaysShouldUpdateOnScreen == false)
			//{
			//	InFigureRD.bShouldUpdateOnScreenAtTheNextFrame = false;
			//}
		}
	//}
} 

void RenderManager::DrawMultiRectanglesOnSurface(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, uint32_t InColor)
{
	//PROFILE_METRICS_COLLECTOR("RenderManager_DrawMultiRectanglesOnSurface");

	for (FFigureRenderData* FigureRD : InObjectsArray)
	{
		if (FigureRD) DrawOneRectangeOnSurface(InSurface, *FigureRD, InColor);
	}
}

void RenderManager::DrawLineOnSurface(SDL_Surface* InSurface, float2& InStart, float2& InEnd, int InThickness, uint32_t InColor)
{
	int X1 = static_cast<int>(InStart.X);
	int Y1 = static_cast<int>(InStart.Y);
	int X2 = static_cast<int>(InEnd.X);
	int Y2 = static_cast<int>(InEnd.Y);

	int dx = abs(X2 - X1), sx = X1 < X2 ? 1 : -1;
	int dy = -abs(Y2 - Y1), sy = Y1 < Y2 ? 1 : -1;
	int err = dx + dy, e2;

	while (true)
	{
		FRect LinePixel = { X1, Y1, InThickness, InThickness };
		DrawOnePixelOnSurface(InSurface, LinePixel, InColor);

		if (X1 == X2 && Y1 == Y2) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; X1 += sx; }
		if (e2 <= dx) { err += dx; Y1 += sy; }
	}
}

void RenderManager::DrawOnePixelOnSurface(SDL_Surface* InSurface, const FRect& InPixelPosition, uint32_t InColor)
{
	SDL_FillSurfaceRect(InSurface, &InPixelPosition.Rect, InColor);
}

void RenderManager::DrawMultiPixelsOnSurface(SDL_Surface* InSurface, const std::vector<FRect>& Pixels, uint32_t InColor)
{
	for (const FRect& Pixel : Pixels)
	{
		DrawOnePixelOnSurface(InSurface, Pixel, InColor);
	}
}

void RenderManager::DrawOneRayOnSurface_ByPixel(
	SDL_Surface* InSurface,
	const std::vector<FFigureRenderData*>& InObjectsArray,
	const FRay& InRay,
	uint32_t InColor)
{
	float DrawX = InRay.StartX;
	float DrawY = InRay.StartY;

	bool bIsInScreenSpace = true;
	bool bIsNotCollidedWithSomeObject = true;
	while (bIsInScreenSpace && bIsNotCollidedWithSomeObject)
	{
		DrawX += RAYS_STEP * cos(InRay.Angle);
		DrawY += RAYS_STEP * sin(InRay.Angle);

		float2 CurrentPoint = { DrawX, DrawY };
		FRect ScreenSpaceRect = { 0, 0, WIDTH, HEIGHT };
		bIsInScreenSpace = ScreenSpaceRect.IsIn(CurrentPoint);
		for (FFigureRenderData* CurrentFigureRenderData : InObjectsArray)
		{
			if (CurrentFigureRenderData && CurrentFigureRenderData->Figure && !CurrentFigureRenderData->Figure->bTransparent)
			{
				bIsNotCollidedWithSomeObject = !CurrentFigureRenderData->Figure->IsIn(CurrentPoint);
				if (bIsNotCollidedWithSomeObject == false) break;
			}
		}

		FRect CurrentPixel = { (int)DrawX, (int)DrawY, RAY_THICKNESS, RAY_THICKNESS };
		DrawOnePixelOnSurface(InSurface, CurrentPixel, InColor);
	}
}

void RenderManager::DrawOneRayOnSurface_ByPixelBuffer(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor)
{
	float DrawX = InRay.StartX;
	float DrawY = InRay.StartY;

	std::vector<FRect> PixelsToDraw;

	bool bIsInScreenSpace = true;
	bool bIsNotCollidedWithSomeObject = true;

	while (bIsInScreenSpace && bIsNotCollidedWithSomeObject)
	{
		DrawX += RAYS_STEP * cos(InRay.Angle);
		DrawY += RAYS_STEP * sin(InRay.Angle);

		float2 CurrentPoint = { DrawX, DrawY };
		FRect ScreenSpaceRect = { 0, 0, WIDTH, HEIGHT };
		bIsInScreenSpace = ScreenSpaceRect.IsIn(CurrentPoint);

		for (FFigureRenderData* CurrentFigureRenderData : InObjectsArray)
		{
			if (CurrentFigureRenderData && CurrentFigureRenderData->Figure && !CurrentFigureRenderData->Figure->bTransparent)
			{
				bIsNotCollidedWithSomeObject = !CurrentFigureRenderData->Figure->IsIn(CurrentPoint);
				if (!bIsNotCollidedWithSomeObject) break;
			}
		}

		PixelsToDraw.push_back({ (int)DrawX, (int)DrawY, RAY_THICKNESS, RAY_THICKNESS });
	}

	RenderManager::DrawMultiPixelsOnSurface(InSurface, PixelsToDraw, InColor);
}

void RenderManager::DrawOneRayOnSurface_ByLine(SDL_Surface* InSurface, const std::vector<FFigureRenderData*>& InObjectsArray, const FRay& InRay, uint32_t InColor)
{
	float DrawX = InRay.StartX;
	float DrawY = InRay.StartY;

	bool bIsInScreenSpace = true;
	bool bIsNotCollidedWithSomeObject = true;

	while (bIsInScreenSpace && bIsNotCollidedWithSomeObject)
	{
		float NewX = DrawX + RAYS_STEP * cos(InRay.Angle);
		float NewY = DrawY + RAYS_STEP * sin(InRay.Angle);

		float2 CurrentPoint = { NewX, NewY };
		FRect ScreenSpaceRect = { 0, 0, WIDTH, HEIGHT };
		bIsInScreenSpace = ScreenSpaceRect.IsIn(CurrentPoint);

		for (FFigureRenderData* CurrentFigureRenderData : InObjectsArray)
		{
			if (CurrentFigureRenderData && CurrentFigureRenderData->Figure && !CurrentFigureRenderData->Figure->bTransparent)
			{
				bIsNotCollidedWithSomeObject = !CurrentFigureRenderData->Figure->IsIn(CurrentPoint);
				if (!bIsNotCollidedWithSomeObject) break;
			}
		}

		float2 Start = { DrawX, DrawY };
		float2 End = { NewX, NewY };
		RenderManager::DrawLineOnSurface(InSurface, Start, End, 1, InColor);

		DrawX = NewX;
		DrawY = NewY;
	}
}

void RenderManager::MakeOneFrame(FMainData& InMainData)
{
	PROFILE_METRICS_COLLECTOR("RenderManager_MakeOneFrame");

	// Erase the screen
	{
		PROFILE_METRICS_COLLECTOR("ClearScreen");
        SDL_RenderClear(InMainData.Renderer);
		if (InMainData.EraseRectRD) DrawOneRectangeOnSurface(InMainData.Surface, *InMainData.EraseRectRD, COLOR_BLACK);
	}
	
	// Generate new rays
	{
		PROFILE_METRICS_COLLECTOR("GenerateRays");
		RayTrace::GenerateRays(InMainData.GenerateRaysFromPosition, InMainData.RaysArray);
	}
	
	// Fill the rays on the screen
	{
		PROFILE_METRICS_COLLECTOR("FillRaysOnSurface_Async");
		RayTrace::FillRaysOnSurface_Async(InMainData);
	}

	// Fill the rectangles
	{
		PROFILE_METRICS_COLLECTOR("DrawMultiRectanglesOnSurface");
		DrawMultiRectanglesOnSurface(InMainData.Surface, InMainData.ObjectsToRender, COLOR_WHITE);
	}
	
	// Fill the circles
	{
		PROFILE_METRICS_COLLECTOR("DrawMultiCirclesOnSurface");
		DrawMultiCirclesOnSurface(InMainData.Surface, InMainData.ObjectsToRender, COLOR_WHITE);
	}
	
	// Update the screen
	{
    	PROFILE_METRICS_COLLECTOR("SDL_UpdateRenderer");

		{
			PROFILE_METRICS_COLLECTOR("SDL_DestroyTexture");
			if (InMainData.RenderTexture) SDL_DestroyTexture(InMainData.RenderTexture);
		}

		{
			PROFILE_METRICS_COLLECTOR("SDL_CreateTextureFromSurface");
			InMainData.RenderTexture = SDL_CreateTextureFromSurface(InMainData.Renderer, InMainData.Surface);
		}

    	// Очищаем рендер перед рисованием
    	{
			PROFILE_METRICS_COLLECTOR("SDL_RenderClear");
			SDL_RenderClear(InMainData.Renderer);
		}

    	// Копируем текстуру на экран
		{
			PROFILE_METRICS_COLLECTOR("SDL_RenderTexture");
			SDL_FRect Rect = { 0, 0, WIDTH, HEIGHT };
    		SDL_RenderTexture(InMainData.Renderer, InMainData.RenderTexture, &Rect, &Rect);
		}
    	
		// Показываем обновлённый кадр
    	{
			PROFILE_METRICS_COLLECTOR("SDL_RenderPresent");
			SDL_RenderPresent(InMainData.Renderer);
		}
	}
	// Hard Vsync
	//SDL_Delay(16.6);
}
