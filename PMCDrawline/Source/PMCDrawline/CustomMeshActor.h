// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D:\Work\UE426\Engine\Plugins\Runtime\ProceduralMeshComponent\Source\ProceduralMeshComponent\Public\ProceduralMeshComponent.h"
#include "CustomMeshActor.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PMCDRAWLINE_API ACustomMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomMeshActor();

	UPROPERTY()
	UProceduralMeshComponent* DrawMeshComponent;

	UFUNCTION( BlueprintCallable)
	/* InPoints是折线上的点，InWidth是线的宽度，为了简单起见这里只画XY平面的折线，即要确保InPoints中点的Z轴值为0或全都相等*/
	void DrawLine(UMaterialInterface* InMaterial, const TArray<FVector>& InPoints, float InWidth);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
