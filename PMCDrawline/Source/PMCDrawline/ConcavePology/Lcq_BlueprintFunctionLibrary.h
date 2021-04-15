// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Lcq_BlueprintFunctionLibrary.generated.h"

// ���������ͼԪ����һЩ����
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

	/* �Ӷ���ε�����ĵ������ȡ�����������������Ϣ����͹����Σ�*/
	UFUNCTION( BlueprintCallable, Category = "aoduobianxing")
	static void GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise = true);

	/* �����������Zֵͳһ�� */
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static bool SetPointsZvalueBySpecify(TArray<FVector>& _points, float _zValue);

	/*���ݵ������ȡ�������� 	*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static TArray<FVector> GetVectorArrByPointsArr(const TArray<FVector> _points);

	/* �����������л�ȡһ����������������е��ӳ��������������Ľ���
	ע�⣺˳������ȴ�����������±꿪ʼ��������0;���㲻���������˵�
	����1��������������
	����2����Ӧ�ĵ����飨�����豣��һ�£�
	����3������������±�
	����4��5����ʼ�ͽ����±�
	����6��7: ���ݽ��㱻�зֵ����������
	����ֵ��true Ϊ�ɹ�,��֮��
	*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static bool GetRayIntersectionOfVecInVecArr(
		const TArray<FVector> _dirs, const TArray<FVector> _points, 
		const int _vecIndex, 
		const int _beginIndex, const int _endIndex,
		TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB
	);

	/*
	*   �ָ�
		��������Ƿ��ǰ�����Σ�����Ǿ��и�
		����1���㼯
		����2����Ҫ���صı��и�����1
		����2����Ҫ���صı��и�����2
	*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing")
	static void DividePolygonIfConcave(TArray<FVector> _points, bool _antiClockwise /*= true*/,
		TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB
		);

	/* ToolFunc;�ӳ����н����,���ý�����г�������ͼԪ */
	UFUNCTION( BlueprintCallable, Category = "aoduobianxing | Tool")
	static TArray<FVector> GetPointsByIndexRange(const TArray<FVector>& _points, int IndexParam1, int IndexParam2);
	
	/* ToolFunc*/
	UFUNCTION(BlueprintCallable, Category = "aoduobianxing | Tool")
	static bool CheckTwoLineIntersectionResult(const FVector& fromA, const FVector& fromB, const FVector& toA, const FVector& toB, FVector& _outIntersection);
};
