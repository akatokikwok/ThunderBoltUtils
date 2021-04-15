// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Lcq_BlueprintFunctionLibrary.generated.h"

// 描述多边形图元带的一些属性
struct FLcq_PolygonData
{

}; 

/**
 * 
 */
UCLASS()
class PMCDRAWLINE_API ULcq_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//UFUNCTION( BlueprintCallable, Category="ConcavePolygn")
	//static void GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise = true);
	//UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
	//static void DividePolygonIfConcave(TArray<FVector> _points, bool _antiClockwise = true);
	//UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
	//static bool SetPointsZvalueBySpecify(TArray<FVector>& _points, float _zValue);
	//UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
	//static TArray<FVector> GetVectorArrByPointsArr(const TArray<FVector> _points);
	//UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
	//static bool CheckTwoLineIntersectionResult(const FVector& fromA, const FVector& fromB, const FVector& toA, const FVector& toB, FVector& _outIntersection);
	//
	//UFUNCTION(BlueprintImplementableEvent, Category = "ConcavePolygn")
	//static TArray<FVector> GetPointsByIndexRange(TArray<FVector> allPoints, int32 rangeStart, int32 rangeEnd);

	//UFUNCTION(BlueprintCallable, Category = "ConcavePolygn")
	//static bool GetRayIntersectionOfVecInVecArr(
	//	const TArray<FVector> _dirs,
	//	const TArray<FVector> _points, 
	//	const int _vecIndex, 
	//	const int _beginIndex, const int _endIndex,
	//	TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB);

	//UFUNCTION(BlueprintImplementableEvent, Category = "ConcavePolygn")
	//	static void InsideGetIntersection();

	/* 从多边形的有序的点数组获取顶点和三角面数据信息（凹凸多边形）*/
	UFUNCTION( BlueprintCallable, Category = "aoduobianxing")
	static void GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise = true);

	/* 给定点数组的Z值统一化 */
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static bool SetPointsZvalueBySpecify(TArray<FVector>& _points, float _zValue);

	/*根据点数组获取向量数组 	*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static TArray<FVector> GetVectorArrByPointsArr(const TArray<FVector> _points);

	/* 从向量数组中获取一个向量在这个数组中的延长线与其他向量的交点
	注意：顺序必须先从这个向量的下标开始，不能是0;交点不包括向量端点
	参数1：方向向量数组
	参数2：对应的点数组（长度需保持一致）
	参数3：这个向量的下标
	参数4，5：开始和结束下标
	参数6，7: 根据交点被切分的两组点数组
	返回值：true 为成功,反之无
	*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static bool GetRayIntersectionOfVecInVecArr(
		const TArray<FVector> _dirs, const TArray<FVector> _points, 
		const int _vecIndex, 
		const int _beginIndex, const int _endIndex,
		TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB
	);

	/*
	*   分割
		检查多边形是否是凹多边形，如果是就切割
		参数1：点集
		参数2：需要返回的被切割多边形1
		参数2：需要返回的被切割多边形2
	*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static void DividePolygonIfConcave(TArray<FVector> _points, bool _antiClockwise /*= true*/,
		TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB
		);

	/* ToolFunc;延长线有交点后,利用交点分切成两部分图元 */
	UFUNCTION( BlueprintCallable, Category = "aoduobianxing | Tool")
	static TArray<FVector> GetPointsByIndexRange(const TArray<FVector>& _points, int IndexParam1, int IndexParam2);
	
	/* ToolFunc*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing | Tool")
	static bool CheckTwoLineIntersectionResult(const FVector& fromA, const FVector& fromB, const FVector& toA, const FVector& toB, FVector& _outIntersection);
};
