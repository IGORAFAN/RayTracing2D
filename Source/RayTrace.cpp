
#include "RayTrace.h"
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <corecrt_math_defines.h>
#include "ProfileMertickCollector.h"

void RayTrace::GenerateRays(float2& InGenerateRaysFrom, FRay* InRayArray)
{
	PROFILE_METRICS_COLLECTOR("RayTrace_GenerateRays");

	for (int i = 0; i <= RAYS_NUMBERS; ++i)
	{
		float Angle = ((float)i / RAYS_NUMBERS) * 2 * M_PI;
		FRay Ray = { InGenerateRaysFrom.X, InGenerateRaysFrom.Y, Angle };
		InRayArray[i] = Ray;
	}
}

void RayTrace::FillCirclesOnSurface(SDL_Window* InWindow, SDL_Surface* InSurface, const std::vector<IFigure*>& InObjectsArray, uint32_t InColor)
{
	PROFILE_METRICS_COLLECTOR("RayTrace_FillCirclesOnSurface");

	if (InSurface)
	{
		for (IFigure* Figure : InObjectsArray)
		{
			if (FCircle* Circle = static_cast<FCircle*>(Figure))
			{
				for (float X = Circle->X - Circle->R; X <= Circle->X + Circle->R; ++X)
				{
					for (float Y = Circle->Y - Circle->R; Y <= Circle->Y + Circle->R; ++Y)
					{
						float2 CurrentPosition = { X, Y };
						if (Circle->IsIn(CurrentPosition))
						{
							SDL_Rect CurrentPixel = { CurrentPosition.X, CurrentPosition.Y, 1, 1 };
							SDL_FillSurfaceRect(InSurface, &CurrentPixel, InColor);
						}
					}
				}
			}
		}
	}
}

void RayTrace::FillRectanglesOnSurface(SDL_Window* InWindow, SDL_Surface* InSurface, const std::vector<IFigure*>& InObjectsArray, uint32_t InColor)
{
	PROFILE_METRICS_COLLECTOR("RayTrace_FillRectanglesOnSurface");

	for (IFigure* Figure : InObjectsArray)
	{
		if (FRect* Rect = static_cast<FRect*>(Figure))
		{
			SDL_FillSurfaceRect(InSurface, &Rect->Rect, InColor);
		}
	}
}

void RayTrace::FillRaysOnSurface(SDL_Window* Window, SDL_Surface* InSurface, const std::vector<IFigure*>& InObjectsArray, FRay* InRayArray, uint32_t InColor)
{
	PROFILE_METRICS_COLLECTOR("RayTrace_FillRaysOnSurface");

	for (int i = 0; i <= RAYS_NUMBERS; ++i)
	{
		FRay& Ray = InRayArray[i];

		float DrawX = Ray.StartX;
		float DrawY = Ray.StartY;

		bool bIsInScreenSpace = true;
		bool bIsNotCollidedWithSomeObject = true;
		while (bIsInScreenSpace && bIsNotCollidedWithSomeObject)
		{
			DrawX += RAYS_STEP * cos(Ray.Angle);
			DrawY += RAYS_STEP * sin(Ray.Angle);

			float2 CurrentPoint = { DrawX, DrawY };
			FRect ScreenSpaceRect = { 0, 0, WIDTH, HEIGHT };
			bIsInScreenSpace = ScreenSpaceRect.IsIn(CurrentPoint);
			for (IFigure* Figure : InObjectsArray)
			{
				if (Figure && !Figure->bTransparent)
				{
					bIsNotCollidedWithSomeObject = !Figure->IsIn(CurrentPoint);
					if (bIsNotCollidedWithSomeObject == false) break;
				}
			}

			SDL_Rect CurrentPixel = { (int)DrawX, (int)DrawY, RAY_THICKNESS, RAY_THICKNESS };
			SDL_FillSurfaceRect(InSurface, &CurrentPixel, COLOR_YELLOW);
		}
	}
}
