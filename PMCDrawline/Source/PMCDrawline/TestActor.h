// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Triangulate.h"
#include "ProceduralMeshComponent.h"
#include "TestActor.generated.h"

UCLASS()
class PMCDRAWLINE_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	UFUNCTION( BlueprintCallable,Category="Test")
	void DrawMyTestFunc(UMaterialInterface* InMaterial, const TArray<FVector2D>& inpoints );

	UFUNCTION(BlueprintCallable, Category = "Test")
		void DrawTheRangeLastTriangle(UMaterialInterface* InMaterial);

	UPROPERTY()
		UProceduralMeshComponent* DrawMeshComponent;
	UPROPERTY()
		UProceduralMeshComponent* MeshLast;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
