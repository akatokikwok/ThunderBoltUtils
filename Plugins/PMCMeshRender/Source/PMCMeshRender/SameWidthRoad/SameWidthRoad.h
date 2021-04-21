// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "carfield_map_api/include/hdmap_data_expansion.h"
#include <ThirdParty/carfield_map_api/include/carfield_map_api.h>
#include "SameWidthRoad.generated.h"

class UProceduralMeshComponent;
namespace carfield_map { class CarfieldMapApi; }

USTRUCT( BlueprintType)
struct FCusRoadInfo
{
	GENERATED_USTRUCT_BODY()

	hdmap::RoadPtr road;
	std::vector<hdmap::Point> roadPoints;
};

/* 
* 用于绘制等宽道路的场景actor;
*/
UCLASS()
class PMCMESHRENDER_API ASameWidthRoad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASameWidthRoad();

	/* 
	* 加工外部点集	
	*/
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "拿取相关几何信息"), Category = "SameWidthRoad")
		static void FetchGeoInfo(
			const TArray<FVector>& RoadMidPts_in,
			const TArray<FVector>& normals_in,
			float InWidth,
			TArray<FVector>& vertices_out,
			TArray<int32>& triangles_out,
			TArray<FVector>& normals_out
	);

	/* 
	* 调用此方法来绘制等宽道路
	* InPoints是折线上的点，InWidth是线的宽度，为了简单起见这里只画XY平面的折线，即要确保InPoints中点的Z轴值为0或全都相等
	*/
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "调用此方法来绘制等宽道路"), Category = "SameWidthRoad")
	static void DrawSameWidthRoad(UMaterialInterface* InMaterial,
		const TArray<FVector>& vertices,
		const TArray<int32>& triangles,
		const TArray<FVector>& normals
	);
	
	/* 
	* TO DO	
	*/
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "地图API接口解析"), Category = "SameWidthRoad")
	static TArray<FVector> ParseOutsidePts(FCusRoadInfo cusRoadInfo, TArray<FVector> pts_out);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	static UProceduralMeshComponent* DrawMeshComponent;
	
	UPROPERTY()
	TArray<FVector> dianji;

};
