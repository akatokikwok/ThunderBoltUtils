// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>

/**
 * 凹多边形判断的算法基类
 */
class PMCMESHRENDER_API Triangulate
{
public:
	// triangulate a contour/polygon, places results in STL vector
	// as series of triangles.
	static bool Process(const TArray<FVector>& contour, TArray<FVector>& result);

	// 计算多边形区域
	static float Area(const TArray<FVector>& contour);

	// decide if point Px/Py is inside triangle defined by
	// (Ax,Ay) (Bx,By) (Cx,Cy)
	static bool InsideTriangle(float Ax, float Ay,
		float Bx, float By,
		float Cx, float Cy,
		float Px, float Py);
private:
	static bool Snip(const TArray<FVector>& contour, int u, int v, int w, int n, int* V);
};
