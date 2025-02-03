#pragma once
#include <SDL_rect.h>
#include <corecrt_math.h>

#define WIDTH  1280
#define HEIGHT 720

#define COLOR_WHITE		0xffffffff
#define COLOR_BLACK		0x00000000
#define COLOR_GRAY		0x80808080
#define COLOR_RED		0xffff0000
#define COLOR_YELLOW	0xf7f707

#define RAYS_NUMBERS 500
#define RAYS_STEP 5
#define RAY_THICKNESS 2

#define THREADS_NUMBERS 8

struct float2
{
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
	float StartX, StartY, Angle;
};