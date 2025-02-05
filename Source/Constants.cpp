#include "Constants.h"
#include "ProfileMertickCollector.h"


// FRect
FRect::FRect()
{
	Rect = { 0,0,0,0 };
}

FRect::FRect(int InX, int InY, int InW, int InH)
{
	Rect = SDL_Rect{ InX, InY,  InW, InH};
}

FRect::FRect(const SDL_Rect& InRect)
{
	Rect = InRect;
}

bool FRect::IsIn(float2& InData)
{
	return (InData.X >= Rect.x &&
			InData.X <= (Rect.x + Rect.w) &&
			InData.Y >= Rect.y &&
			InData.Y <= (Rect.y + Rect.h));
}

void FRect::Move(float2& InData)
{
	Rect.x = InData.X - (int)(Rect.w / 2);
	Rect.y = InData.Y - (int)(Rect.h / 2);
}

// FCircle
FCircle::FCircle(float InX, float InY, float InR) 
	: X(InX), Y(InY), R(InR)
{
}

FCircle::FCircle()
	: X(0.f), Y(0.f), R(0.f)
{
}

bool FCircle::IsIn(float2& InData)
{
	float RadiusSquare = pow(R, 2);
	float DistanceSquare = pow(InData.X - X, 2) + pow(InData.Y - Y, 2);
	return (DistanceSquare < RadiusSquare);
}

void FCircle::Move(float2& InData)
{
	X = InData.X;
	Y = InData.Y;
}
