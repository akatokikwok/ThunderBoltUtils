// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>


/**
 * 
 */
class PMCDRAWLINE_API Triangulate
{
public:

	// triangulate a contour/polygon, places results in STL vector
	// as series of triangles.
	static bool Process(const TArray<FVector2D>& contour, TArray<FVector2D>& result);

	// compute area of a contour/polygon
	static float Area(const TArray<FVector2D>& contour);

	// decide if point Px/Py is inside triangle defined by
	// (Ax,Ay) (Bx,By) (Cx,Cy)
	static bool InsideTriangle(float Ax, float Ay,
		float Bx, float By,
		float Cx, float Cy,
		float Px, float Py);
private:
	static bool Snip(const TArray<FVector2D>& contour, int u, int v, int w, int n, int* V);
};
