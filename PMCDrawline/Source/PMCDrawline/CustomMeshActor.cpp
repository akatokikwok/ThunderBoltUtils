// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomMeshActor.h"
#include "Chaos/Rotation.h"
#include "Templates/Casts.h"

// Sets default values
ACustomMeshActor::ACustomMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	DrawMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("DrawMeshComponent"));
	DrawMeshComponent->SetupAttachment(RootComponent);

}


// Called when the game starts or when spawned
void ACustomMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* InPoints�������ϵĵ㣬InWidth���ߵĿ�ȣ�Ϊ�˼��������ֻ��XYƽ������ߣ���Ҫȷ��InPoints�е��Z��ֵΪ0��ȫ�����
   right�Ǹ��߶ε�����λ������Ҳ������XY��άƽ��������߶δ�ֱ���������Ӽ�right�������ǽ�������ƽ�ƣ�triangles���������vertices���붥���˳���йء�
   normals;tangents;vertexColors;uvs;��Щ������û�����ֵ������ʹ��ʱ�ɸ�����Ҫ��ӣ�������uvsһ�㶼����ֵ�ġ�
*/
void ACustomMeshActor::DrawLine(UMaterialInterface* InMaterial, const TArray<FVector>& InPoints, float InWidth)
{
	TArray<FVector> vertices;
	TArray<int32> triangles;

	int32 num = InPoints.Num(); // ������
	if (num < 2) // �ų��������
	{
		return;
	}
		
	for (int32 i = 0; i < (num); i++)//�������е�
	{
		FVector right(0, -1, 0); // right��Ҫ�����߶ε�����λ������Ҳ������XY��άƽ��������߶δ�ֱ���������Ӽ�right�������ǽ�������ƽ��

		if (i == num - 1) //�����ڻ������ߵ����һ���˵�
		{
			right = FVector::CrossProduct(InPoints[i] - InPoints[i - 1], FVector(0, 0, 1));
		}		
		else 
		{
			right = FVector::CrossProduct(InPoints[i + 1] - InPoints[i], FVector(0, 0, 1));
		}
		right.Normalize();
		right = -right;//��������ϵת��������ϵ

		FVector pt1, pt2, pt0, pt3;
		FVector param0, param1;
		
		// ԭʼ����
		/*pt0 = InPoints[i] - right * InWidth / 2;
		pt1 = InPoints[i] + right * InWidth / 2;
		param0 = pt0;
		param1 = pt1;
		arc
		*/

		//FVector LastThirdPt;
		//if ( i <= (num -3))
		//{
		//	FVector a = InPoints[i] - InPoints[i + 1];
		//	a.Normalize();
		//	FVector b = InPoints[i + 1] - InPoints[i + 2];
		//	b.Normalize();
		//	FVector avg = a + b;
		//	FVector dir = FVector(-avg.Y, avg.X, 0);//�ҵ������������Ĵ�ֱ������Ϊ���䳤�ȵĻ�׼
		//	dir.Normalize();
		//	dir = -dir;
		//	/* �������䳤��,�������������dir��ȡ��width/2�õ����������䳤��expandlenghth*/
		//	float t = FMath::Abs(FMath::Acos((FVector::DotProduct(a, b)))) / 2;  // ��λ������˻�üн�����
		//	float expandlength = (InWidth / 2) / FMath::Cos(t);  // ���ݽǶȵ������䳤��

		//	pt0 = InPoints[i] - dir * expandlength;
		//	pt1 = InPoints[i] + dir * expandlength;

		//	if (i == (num-3))
		//	{
		//		LastThirdPt = InPoints[num - 3];
		//	}
		//} else if( i ==(num-2) )
		//{				
		//	FVector a = LastThirdPt - InPoints[num - 2];
		//	a.Normalize();
		//	FVector b = InPoints[num-2 ] - InPoints[i+1];
		//	b.Normalize();
		//	FVector avg = a + b;
		//	FVector dir = FVector(-avg.Y, avg.X, 0);//�ҵ������������Ĵ�ֱ������Ϊ���䳤�ȵĻ�׼
		//	dir.Normalize();
		//	dir = -dir;
		//	/* �������䳤��,�������������dir��ȡ��width/2�õ����������䳤��expandlenghth*/
		//	float t = FMath::Abs(FMath::Acos((FVector::DotProduct(a, b)))) / 2;  // ��λ������˻�üн�����
		//	float expandlength = (InWidth / 2) / FMath::Cos(t);  // ���ݽǶȵ������䳤��

		//	pt0 = InPoints[i] - dir * expandlength;
		//	pt1 = InPoints[i] + dir * expandlength;
		//} else if ( i == (num-1) )
		//{

		//}

		if (i == 0){
			pt0 = InPoints[i] - right * InWidth / 2;
			pt1 = InPoints[i] + right * InWidth / 2;
			param0 = pt0;
			param1 = pt1;
		}
		if ( i >= 1 && i <= (num-2) )
		{
			FVector a = InPoints[i - 1] - InPoints[i];
			a.Normalize();
			FVector b = InPoints[i] - InPoints[i + 1];
			b.Normalize();
			FVector avg = a + b;
			FVector CornerDir = FVector(-avg.Y, avg.X, 0);//�ҵ������������Ĵ�ֱ������Ϊ���䳤�ȵĻ�׼
			CornerDir.Normalize();
			CornerDir = -CornerDir;
			/* �������䳤��,�������������dir��ȡ��width/2�õ����������䳤��expandlenghth*/
			float t = FMath::Abs(FMath::Acos((FVector::DotProduct(a, b)))) / 2;  // ��λ������˻�üн�����
			float expandlength = (InWidth / 2) / FMath::Cos(t);  // ���ݽǶȵ������䳤��
			pt0 = InPoints[i] - CornerDir * expandlength;
			pt1 = InPoints[i] + CornerDir * expandlength;

		}
		if ( i == (num-1) )
		{
			pt0 = InPoints[i] - right * InWidth / 2;
			pt1 = InPoints[i] + right * InWidth / 2;
			param0 = pt0;
			param1 = pt1;
		}
		
		/* ÿ��ѭ���Ͱ���Щ����ע�����������*/
		param0 = pt0;
		param1 = pt1;
		vertices.Add(param0);
		vertices.Add(param1);		
		
		/* �����ε�������*/
		int currFinalvertexIndex = vertices.Num() - 1; //���λ��Ƶ����������һ���������(Ҫ��i�������1)		
		if (i >= 1 /*&& i <= (num-2)*/ )
		{
			triangles.Add(currFinalvertexIndex - 3);//ǰǰǰ���˵�   [0]
			triangles.Add(currFinalvertexIndex - 2);//ǰǰ���˵�     [1] 
			triangles.Add(currFinalvertexIndex - 1);//				[2]
			triangles.Add(currFinalvertexIndex - 1);//				[2]
			triangles.Add(currFinalvertexIndex - 2);// 				 [1]
			triangles.Add(currFinalvertexIndex - 0);//���˵�(�����һ��) [3]
		}				
	}	

	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
	TArray<FVector2D> uvs;
	DrawMeshComponent->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, true);
	DrawMeshComponent->SetMaterial(0, InMaterial);
}


