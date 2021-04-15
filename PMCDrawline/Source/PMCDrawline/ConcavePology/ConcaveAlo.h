//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "ConcaveAlo.generated.h"
//
//
//// 描述多边形图元带的一些属性
//USTRUCT( BlueprintType)
//struct FLcq_PolygonData
//{
//	GENERATED_USTRUCT_BODY();
//public:
//	UMaterialInterface* InMaterial;
//	FLinearColor color;
//
//};
//
//
//UCLASS()
//class PMCDRAWLINE_API AConcaveAlo : public AActor
//{
//	GENERATED_BODY()
//	
//public:	
//	// Sets default values for this actor's properties
//	AConcaveAlo();
//
//public:
//	UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
//		 void GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise = true);
//	UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
//		 void DividePolygonIfConcave(TArray<FVector> _points, bool _antiClockwise = true);
//	UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
//		 bool SetPointsZvalueBySpecify(TArray<FVector>& _points, float _zValue);
//	UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
//		 TArray<FVector> GetVectorArrByPointsArr(const TArray<FVector> _points);
//	UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
//		 bool CheckTwoLineIntersectionResult(const FVector& fromA, const FVector& fromB, const FVector& toA, const FVector& toB, FVector& _outIntersection);
//
//	//UFUNCTION(BlueprintImplementableEvent, Category = "ConcavePolygn")
//	//	 TArray<FVector> GetPointsZaiIndexRange(const TArray<FVector> allPoints, int32 rangeStart, int32 rangeEnd);
//
//	UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
//		 bool GetRayIntersectionOfVecInVecArr(
//			const TArray<FVector> _dirs,
//			const TArray<FVector> _points,
//			const int _vecIndex,
//			const int _beginIndex, const int _endIndex,
//			TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB);
//
//	//助手方法
//	UFUNCTION(BlueprintImplementableEvent, Category = "ConcavePolygn")
//		TArray<FVector> GetPointsByIndexRange(const TArray<FVector>& _points, int32 xushuParam1, int32 xushuParam2);
//
//	//UFUNCTION( BlueprintCallable, Category = "ConcavePolygn")
//	//	TArray<FVector> CppGetPointsByIndexRange(const TArray<FVector>& _points, int32 xushuParam1, int32 xushuParam2);
//
//	//助手方法
//	UFUNCTION(BlueprintImplementableEvent, Category = "ConcavePolygn")
//		void InsideGetIntersection(const int _vecIndex,// 这个向量的下标
//			const int _beginIndex, const int _endIndex,// 开始和结束下标
//			TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB);
//
//	
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//};
