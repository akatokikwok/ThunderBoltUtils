// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcaveActor.h"
#include "Triangulate.h"
#include "GameFramework/Actor.h"
#include "..\ThirdParty\carfield_map_api\include\carfield_map_api.h"
#include "..\ThirdParty\carfield_map_api\include\hdmap_data.h"

UProceduralMeshComponent* AConcaveActor::DrawMeshComponent = nullptr;

// Sets default values
AConcaveActor::AConcaveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	DrawMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("DrawMeshComponent"));
	DrawMeshComponent->SetupAttachment(RootComponent);
}

void AConcaveActor::HandleConcaveInfo(const TArray<FVector>& inpoints, const TArray<FVector>& normals_in, TArray<FVector>& vertices_out, TArray<int32>& indices_out, TArray<FVector>& normals_out)
{
	TArray<FVector> vertices;
	TArray<int32> triangles;

	// ���ȥ
	TArray<FVector> result;

	Triangulate::Process(inpoints, result);
	// ��ѧ���ó����Ķ����������ǻ�
	int tcount = result.Num() / 3;

	for (int i = 0; i < (tcount); i++)
	{
		const FVector p1 = result[i * 3 + 0];

		const FVector p2 = result[i * 3 + 1];

		const FVector p3 = result[i * 3 + 2];

		// ��ӡÿһ��ͼԪ,�趨�����������۲������
		UE_LOG(LogTemp, Warning,
			TEXT("within i+1 mode; Triangle %d => (%0.0f,%0.0f) (%0.0f,%0.0f) (%0.0f,%0.0f)\n"),
			i + 1, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y
		);
		// ͬ��,ֻ�Ƕ��ⷽ����ѭ�������һ��
		UE_LOG(LogTemp, Log,
			TEXT("within i mode; Triangle %d => (%0.0f,%0.0f) (%0.0f,%0.0f) (%0.0f,%0.0f)\n"),
			i, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y
		);

		FVector qidian;
		int32 qidiansuoyin; //ת�۵������

		if (i >= 0 && i <= (tcount - 1))
		{
			if (i == 0)
			{
				// TO do,���������������
				qidian = -p1;
				qidiansuoyin = i;
			}
			if (i == tcount - 1)
			{
				UE_LOG(LogTemp, Log, TEXT("has been last triangle,need to specially handle current Vertexs turn"));
			}

			// ������Ӷ���;ȡ����ִ������ϵת��
			vertices.Add(-p1);
			vertices.Add(-p2);
			vertices.Add(-p3);
		}

		/// ������������׼��
		int currFinalvertexIndex = vertices.Num() - 1;
		if (i >= 0 && i <= (tcount - 1))
		{
			if (i == tcount - 1)
			{
				UE_LOG(LogTemp, Log, TEXT("has been last triangle,need to specially handle current Indexes turn"));
			}

			triangles.Add(currFinalvertexIndex - 0);
			triangles.Add(currFinalvertexIndex - 1);
			triangles.Add(currFinalvertexIndex - 2);
		}
	}

	vertices_out = vertices;
	indices_out = triangles;
	normals_out = normals_in;
}

void  AConcaveActor::DrawConcaveFunc(UMaterialInterface* InMaterial,
	const TArray<FVector>& vertices,
	const TArray<int32>& triangles,
	const TArray<FVector>& normals
)
{
	/* ����api���ƶ����*/	
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
	TArray<FVector2D> uvs;
	DrawMeshComponent->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, true);
	DrawMeshComponent->SetMaterial(0, InMaterial);
}



// Called when the game starts or when spawned
void AConcaveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConcaveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

