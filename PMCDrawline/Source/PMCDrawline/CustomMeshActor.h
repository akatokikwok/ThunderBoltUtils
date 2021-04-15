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
	/* InPoints�������ϵĵ㣬InWidth���ߵĿ�ȣ�Ϊ�˼��������ֻ��XYƽ������ߣ���Ҫȷ��InPoints�е��Z��ֵΪ0��ȫ�����*/
	void DrawLine(UMaterialInterface* InMaterial, const TArray<FVector>& InPoints, float InWidth);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
