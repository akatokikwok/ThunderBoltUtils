// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "ProceduralMeshComponent.h"
#include "PMCDrawline.h"

#include "Materials/MaterialInterface.h"


// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	DrawMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("DrawMeshComponent"));
	DrawMeshComponent->SetupAttachment(RootComponent);

	MeshLast = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshLast"));
	MeshLast->SetupAttachment(RootComponent);

}

bool LastTriVertexOpen = false;
bool LastTriIndexOpen = false;
void ATestActor::DrawMyTestFunc(UMaterialInterface* InMaterial, const TArray<FVector2D>& inpoints )
{
	TArray<FVector> vertices;
	TArray<int32> triangles;

	TArray<FVector> verticesLast;
	TArray<int32> trianglesLast;


	/*TArray<FVector2D> a;*/
	//a.Add(FVector2D(0, 6));
	//a.Add(FVector2D(0, 0));
	//a.Add(FVector2D(3, 0));
	//a.Add(FVector2D(4, 1));
	//a.Add(FVector2D(6, 1));
	//a.Add(FVector2D(8, 0));
	//a.Add(FVector2D(12, 0));
	//a.Add(FVector2D(13, 2));
	//a.Add(FVector2D(8, 2));
	//a.Add(FVector2D(8, 4));
	//a.Add(FVector2D(11, 4));
	//a.Add(FVector2D(11, 6));
	//a.Add(FVector2D(6, 6));
	//a.Add(FVector2D(4, 3));
	//a.Add(FVector2D(2, 6));

	TArray<FVector2D> result;

	Triangulate::Process(inpoints, result);

	// ��ѧ���ó����Ķ����������ǻ�
	int tcount = result.Num() / 3;

	for (int i = 0; i < (tcount); i++) 
	{
		const FVector2D& p11 = result[i * 3 + 0];
		const FVector p1 = FVector(p11, 0);

		const FVector2D& p22 = result[i * 3 + 1];
		const FVector p2 = FVector(p22, 0);

		const FVector2D& p33 = result[i * 3 + 2];
		const FVector p3 = FVector(p33, 0);

		// ��ӡÿһ��ͼԪ,�趨�����������۲������
		UE_LOG(LogTemp, Warning,
			TEXT("within i+1 mode; Triangle %d => (%0.0f,%0.0f) (%0.0f,%0.0f) (%0.0f,%0.0f)\n"), 
			i + 1, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y
		);
		// ͬ��,ֻ�Ƕ��ⷽ����ѭ�������һ��
		UE_LOG(OutputVertexStatus, Log,
			TEXT("within i mode; Triangle %d => (%0.0f,%0.0f) (%0.0f,%0.0f) (%0.0f,%0.0f)\n"),
			i, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y
		);

		 FVector qidian;
		 int32 qidiansuoyin; //ת�۵������
		

		if (i >= /*1*/0 && i <= (tcount - 1))
		{
			if (i == 0)
			{
				// TO do,����
				qidian = -p1;
				qidiansuoyin = i;
			}
			if ( i == tcount - 1)
			{
				UE_LOG(LogTemp, Log, TEXT("has been last triangle,need to specially handle current Vertexs turn"));
				LastTriVertexOpen = true;
				/*FVector vLastp1 = FVector(result[(tcount - 1) * 3 + 0], 0);
				FVector vLastp2 = FVector(result[(tcount - 1) * 3 + 1], 0);
				FVector vLastp3 = FVector(result[(tcount - 1) * 3 + 2], 0);*/
			}

			// ������Ӷ���;ȡ����ִ������ϵת��
			vertices.Add(-p1);
			vertices.Add(-p2);
			vertices.Add(-p3);
		}	
		
		/// ������������׼��
		int currFinalvertexIndex = vertices.Num() - 1;
		if ( i >= 0 && i <= (tcount - 1 ))
		{
			if (i == tcount - 1)
			{
				UE_LOG(LogTemp, Log, TEXT("has been last triangle,need to specially handle current Indexes turn"));
				LastTriIndexOpen = true;
				
			}

			triangles.Add(currFinalvertexIndex - 0);
			triangles.Add(currFinalvertexIndex - 1);
			triangles.Add(currFinalvertexIndex - 2);			
		}
		
		//	
		//	triangles.Add(currFinalvertexIndex - 0);
		//	triangles.Add(qidiansuoyin);
		//	triangles.Add(currFinalvertexIndex - 1);
		//}
		
	}

	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
	TArray<FVector2D> uvs;
	DrawMeshComponent->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, true);
	DrawMeshComponent->SetMaterial(0, InMaterial);
}

void ATestActor::DrawTheRangeLastTriangle(UMaterialInterface* InMaterial)
{
	//TArray<FVector> vertices;
	//TArray<int32> triangles;

	//vertices.Add();
	//vertices.Add();
	//vertices.Add();

	//triangles.Add();
	//vertices.Add();
	//vertices.Add();


	//TArray<FVector> normals;
	//TArray<FProcMeshTangent> tangents;
	//TArray<FLinearColor> vertexColors;
	//TArray<FVector2D> uvs;
	//MeshLast->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, true);
	//MeshLast->SetMaterial(0, InMaterial);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

