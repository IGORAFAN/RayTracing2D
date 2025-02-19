#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include "../SDL3-devel-3.2.0-VC/SDL3-3.2.0/include/SDL3/SDL_rect.h"
#include <corecrt_math.h>
#endif

#if defined(__linux__)
//
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <SDL3/SDL_rect.h>
#include <math.h>
#endif


#define DEBUG_MODE true

#define WIDTH 1280
#define HEIGHT 720

#define COLOR_WHITE		0xffffffff
#define COLOR_BLACK		0x00000000
#define COLOR_GRAY		0x80808080
#define COLOR_RED		0xffff0000
#define COLOR_YELLOW	0xf7f707

#define RAYS_NUMBERS 1024
#define RAYS_STEP 5
#define RAY_THICKNESS 1

#define THREADS_NUMBERS 8

struct float2
{
	float2() : X(0), Y(0) {}
	float2(float InX, float InY) : X(InX), Y(InY) {}

	float X;
	float Y;
};

struct IFigure
{
	virtual bool IsIn(float2& InData) = 0;
	virtual	void Move(float2& InData) = 0;
	virtual ~IFigure() = default;

public:
	bool bTransparent = false;
	bool bDoesItEmitLight = false;
};

struct FRect : public IFigure
{

public:
	FRect();
	FRect(int InX, int InY, int InH, int InW);
	FRect(const SDL_Rect& InRect);
	~FRect() {}

	virtual bool IsIn(float2& InData);
	virtual void Move(float2& InData);

public:
	SDL_Rect Rect;
};

struct FCircle : public IFigure
{

public:
	FCircle();
	FCircle(float InX, float InY, float InR);
	~FCircle() {}

	virtual bool IsIn(float2& InData);
	virtual void Move(float2& InData);

public:
	float X, Y, R;
};

struct FRay
{
	float StartX, StartY;
	double Angle;
};