// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triangulate.h"
#include "ProceduralMeshComponent.h"
//#include <carfield_map_api.h>
#include "ConcaveActor.generated.h"

UCLASS()
class PMCMESHRENDER_API AConcaveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConcaveActor();

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "处理顶点和索引"), Category = "ConcaveActor")
	static void HandleConcaveInfo(const TArray<FVector>& inpoints, 
		const TArray<FVector>& normals_in,
		TArray<FVector>& vertices_out,
		TArray<int32>& indices_out,
		TArray<FVector>& normals_out
	);


	UFUNCTION(BlueprintCallable, meta = ( ToolTip = "绘制多边形方法" ), Category = "ConcaveActor")
	static void DrawConcaveFunc(UMaterialInterface* InMaterial, 
		const TArray<FVector>& vertices,
		const TArray<int32>& triangles,
		const TArray<FVector>& normals
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	static UProceduralMeshComponent* DrawMeshComponent;
};
