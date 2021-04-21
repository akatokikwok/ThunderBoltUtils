// Fill out your copyright notice in the Description page of Project Settings.


#include "SameWidthRoad.h"
#include <ThirdParty/carfield_map_api/include/carfield_map_api.h>
#include "carfield_map_api/include/hdmap_data_expansion.h"
#include <vector>
#include "carfield_map_api/include/hdmap_data.h"
#include "ProceduralMeshComponent.h"

//UProceduralMeshComponent* ASameWidthRoad::DrawMeshComponent = nullptr;

UProceduralMeshComponent* ASameWidthRoad::DrawMeshComponent;

// Sets default values
ASameWidthRoad::ASameWidthRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	DrawMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("DrawMeshComponent"));
	DrawMeshComponent->SetupAttachment(RootComponent);	
}

void ASameWidthRoad::FetchGeoInfo(
	const TArray<FVector>& RoadMidPts_in,
	const TArray<FVector>& normals_in,
	float InWidth,
	TArray<FVector>& vertices_out,
	TArray<int32>& triangles_out,
	TArray<FVector>& normals_out
)
{

	TArray<FVector> vertices;
	TArray<int32> triangles;
	int32 num = RoadMidPts_in.Num(); // 点数量
	if (num < 2) // 排除意外情况
	{
		return;
	}

	for (int32 i = 0; i < (num); i++)//遍历所有点
	{
		FVector right(0, -1, 0); // right是要绘制线段的右向单位向量，也就是在XY二维平面上与该线段垂直的向量，加减right向量就是将点左右平移

		if (i == num - 1) //若处在绘制折线的最后一个端点
		{
			right = FVector::CrossProduct(RoadMidPts_in[i] - RoadMidPts_in[i - 1], FVector(0, 0, 1));
		}
		else
		{
			right = FVector::CrossProduct(RoadMidPts_in[i + 1] - RoadMidPts_in[i], FVector(0, 0, 1));
		}
		right.Normalize();
		right = -right;//右手坐标系转左手坐标系

		FVector pt1, pt2, pt0, pt3;
		FVector param0, param1;

		if (i == 0) {
			pt0 = RoadMidPts_in[i] - right * InWidth / 2;
			pt1 = RoadMidPts_in[i] + right * InWidth / 2;
			param0 = pt0;
			param1 = pt1;
		}
		if (i >= 1 && i <= (num - 2))
		{
			FVector a = RoadMidPts_in[i - 1] - RoadMidPts_in[i];
			a.Normalize();
			FVector b = RoadMidPts_in[i] - RoadMidPts_in[i + 1];
			b.Normalize();
			FVector avg = a + b;
			FVector CornerDir = FVector(-avg.Y, avg.X, 0);//找到这个半角向量的垂直向量作为扩充长度的基准
			CornerDir.Normalize();
			CornerDir = -CornerDir;
			/* 计算扩充长度,核心是算出法向dir和取代width/2用的真正的扩充长度expandlenghth*/
			float t = FMath::Abs(FMath::Acos((FVector::DotProduct(a, b)))) / 2;  // 单位向量叉乘获得夹角正弦
			float expandlength = (InWidth / 2) / FMath::Cos(t);  // 根据角度调整扩充长度
			pt0 = RoadMidPts_in[i] - CornerDir * expandlength;
			pt1 = RoadMidPts_in[i] + CornerDir * expandlength;

		}
		if (i == (num - 1))
		{
			pt0 = RoadMidPts_in[i] - right * InWidth / 2;
			pt1 = RoadMidPts_in[i] + right * InWidth / 2;
			param0 = pt0;
			param1 = pt1;
		}

		/* 每次循环就把这些顶点注册进顶点数组*/
		param0 = pt0;
		param1 = pt1;
		vertices.Add(param0);
		vertices.Add(param1);

		/* 三角形点数排序*/
		int currFinalvertexIndex = vertices.Num() - 1; //本次绘制的折条的最后一个点的索引(要求i必须大于1)		
		if (i >= 1 /*&& i <= (num-2)*/)
		{
			triangles.Add(currFinalvertexIndex - 3);//前前前个端点   [0]
			triangles.Add(currFinalvertexIndex - 2);//前前个端点     [1] 
			triangles.Add(currFinalvertexIndex - 1);//				[2]
			triangles.Add(currFinalvertexIndex - 1);//				[2]
			triangles.Add(currFinalvertexIndex - 2);// 				 [1]
			triangles.Add(currFinalvertexIndex - 0);//本端点(即最后一个) [3]
		}
	}

	vertices_out = vertices; //传出顶点集
	triangles_out = triangles; //传出索引集
	normals_out = normals_in; //传出法线
}

/* InPoints是折线上的点，InWidth是线的宽度，为了简单起见这里只画XY平面的折线，即要确保InPoints中点的Z轴值为0或全都相等
   right是该线段的右向单位向量，也就是在XY二维平面上与该线段垂直的向量，加减right向量就是将点左右平移，triangles添加索引与vertices加入顶点的顺序有关。
   normals;tangents;vertexColors;uvs;这些变量都没有添加值，具体使用时可根据需要添加，尤其是uvs一般都会有值的。
*/
void ASameWidthRoad::DrawSameWidthRoad(UMaterialInterface* InMaterial,
	const TArray<FVector>& vertices,
	const TArray<int32>& triangles,
	const TArray<FVector>& normals)
{
	/* 利用API进行绘制*/
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
	TArray<FVector2D> uvs;
	DrawMeshComponent->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, true);
	DrawMeshComponent->SetMaterial(0, InMaterial);
}

TArray<FVector> ASameWidthRoad::ParseOutsidePts( FCusRoadInfo cusRoadInfo,  TArray<FVector> pts_out)
{
	auto lbda1 = [cusRoadInfo, &pts_out]()->TArray<FVector> {
		for (int32 i=0; i < cusRoadInfo.roadPoints.size(); ++i)
		{
			pts_out.Add(FVector(cusRoadInfo.roadPoints[i].x, cusRoadInfo.roadPoints[i].y, cusRoadInfo.roadPoints[i].z));
		}
		return pts_out;
	};
	return lbda1();
}

// Called when the game starts or when spawned
void ASameWidthRoad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASameWidthRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


