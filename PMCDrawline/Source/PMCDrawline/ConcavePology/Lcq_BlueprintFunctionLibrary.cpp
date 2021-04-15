//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Lcq_BlueprintFunctionLibrary.h"
//
//static TArray<TArray<FVector>> t_polygonDividedArr; //��������е�ļ���
//
///* ���ݵ������ȡ��������: ���Ǹ�һЩ��,Ȼ��ÿ�ڽ�2������һ������	*/
//TArray<FVector> ULcq_BlueprintFunctionLibrary::GetVectorArrByPointsArr(const TArray<FVector> _points)
//{
//	TArray<FVector> t_res;
//	int t_pointsNum = _points.Num();//��ε�����
//	if (t_pointsNum > 1)
//	{
//		FVector t_p1;
//		FVector t_p2;
//		for (int i = 0; i < _points.Num(); i++)
//		{
//			t_p1 = _points[i];
//			if (i == t_pointsNum - 1)
//			{
//				t_p2 = _points[0];//�鵽���һ���Ͱ�t2�趨Ϊ���
//			}
//			else
//			{
//				t_p2 = _points[i + 1];//û���ҵ����Ͱ�t2�趨Ϊt1����һ��
//			}
//
//			t_res.Add(t_p2 - t_p1);//����������ע��� ��������t_res
//		}
//	}
//
//	return t_res;
//}
//
////�ָ�
///*��������Ƿ��ǰ�����Σ�����Ǿ��и�
//	����1���㼯
//	����2����Ҫ���صı��и�����1
//	����2����Ҫ���صı��и�����2
//	*/
//void ULcq_BlueprintFunctionLibrary::DividePolygonIfConcave(TArray<FVector> _points, bool _antiClockwise)
//{
//	int t_pointsNum = _points.Num();//�ⲿ������
//
//	// ���ֲ����������
//	if (_points.Num() < 3)
//	{
//		return;
//	}
//	if (t_pointsNum == 3)
//	{
//		t_polygonDividedArr.Add(_points);
//		return;
//	}
//
//	// ���������(>=4)�Ĵ���
//	if (t_pointsNum > 3)
//	{
//		TArray<FVector> _dividePolygonA;
//		TArray<FVector> _dividePolygonB;
//		FVector t_p1;
//		FVector t_p2;
//
//		float t_hei = _points[0].Z;//ȡ��һ����߶�
//
//		SetPointsZvalueBySpecify(_points, 0);//ͳһ�����е�߶�Ϊ0
//
//		
//
//		TArray<FVector> t_dirs = GetVectorArrByPointsArr(_points);//�õ����� ָ������ �ļ���
//
//		bool t_divideResult = false; //�и������
//		int t_indexNew = 0;			
//		/// ������������
//		for (int i = 0; i < t_dirs.Num(); i++) 
//		{
//			t_p1 = t_dirs[i]; 
//			if (i == t_dirs.Num() - 1) //���ҵ������һ��������ʱ����Ҫ��T_P2��Ϊ�㼯���  �� ���� ��������
//			{
//				t_p2 = t_dirs[0];
//				t_indexNew = 0;
//			}
//			else // ���ڱ��������е�ʱ�� ��t_p2�趨Ϊ ��һ���� �鵽������ �����²�������Ϊ��һ��
//			{
//				t_p2 = t_dirs[i + 1];
//				t_indexNew = i + 1; //������������Ϊ��һ��
//			}
//
//			float t_rotateDir = FVector::CrossProduct(t_p1, t_p2).Z;//�����ڽ�2�����������z����,�Դ���Ϊ����������α�׼
//
//			//�������ֶ��ɼ����ǰ������;������һ�������
//			if ((t_rotateDir < -0.01f && _antiClockwise == true) || (t_rotateDir>0.01f && _antiClockwise == false))
//			{
//				//��ӡһ��
//				UE_LOG(LogTemp, Warning, TEXT("��ǰ����i����ȷ���ǰ������--------"));
//
//				//��ָ��
//				t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, i, t_pointsNum - 1, _dividePolygonA, _dividePolygonB);
//				if (t_divideResult == false)
//				{
//					t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, 0, i - 1, _dividePolygonA, _dividePolygonB);
//				}
//				if (t_divideResult == false)
//				{
//					UE_LOG(LogTemp, Error, TEXT("�߶�%d  û�еõ��ָ��"), i);
//				}
//				break;
//			}
//		}
//
//		if (t_divideResult == false)
//		{
//			SetPointsZvalueBySpecify(_points, t_hei);
//			t_polygonDividedArr.Add(_points);
//		}
//		else
//		{
//			if (_dividePolygonA.Num() > 2)
//			{
//				SetPointsZvalueBySpecify(_dividePolygonA, t_hei);
//				DividePolygonIfConcave(_dividePolygonA, _antiClockwise);
//			}
//			if (_dividePolygonB.Num() > 2)
//			{
//				SetPointsZvalueBySpecify(_dividePolygonB, t_hei);
//				DividePolygonIfConcave(_dividePolygonB, _antiClockwise);
//			}
//		}
//	}
//}
//
//
///*�����и���Ӷ���ε�����ĵ������ȡ�����������������Ϣ����͹����Σ�
//	����1�� ��������
//	����2�� �Ƿ�����ʱ��*/
//void ULcq_BlueprintFunctionLibrary::GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise)
//{
//	TArray<FLcq_PolygonData> t_res;//�������Ϊ ÿһ��������Я�����Ե�����;�ж��ٸ����ݾ��ж��ٸ������
//	int t_pointsNum = _points.Num();
//
//	if (t_pointsNum > 3)//���4���㼰���ϵ��������
//	{
//		t_polygonDividedArr.Empty();
//		DividePolygonIfConcave(_points, _antiClockwise);  //ִ�еݹ��и�����;��������ʱ�뷽�����
//		if (t_polygonDividedArr.Num() > 0)
//		{
//			//UE_LOG(LogTemp, Warning, TEXT("����ηָ����%d�������"), t_res.Num());
//		}
//	}
//	else //�쳣���
//	{
//		//UE_LOG(LogTemp, Warning, TEXT("����ηָ�ʧ��"));
//	}
//}
//
//
///*�����������Zֵͳһ�趨��ֵ*/
//bool ULcq_BlueprintFunctionLibrary::SetPointsZvalueBySpecify(TArray<FVector>& _points, float _zValue)
//{
//	if (_points.Num() > 0)
//	{
//		for (int i = 0; i < _points.Num(); i++)
//		{
//			_points[i].Z = _zValue;
//		}
//		return true;
//	}
//
//	return false;
//}
//
//// ���ַ���:����ӳ����Ƿ���ĳһ�����ཻ
//bool ULcq_BlueprintFunctionLibrary::CheckTwoLineIntersectionResult(const FVector& fromA, const FVector& toA, const FVector& fromB, const FVector& toB, 
//	FVector& _outIntersection)
//{
//	FVector da = fromB - fromA;
//	FVector db = toB - toA;
//	FVector dc = toA - fromA;
//
//	FVector crossDaDb = FVector::CrossProduct(da, db);
//	float prod = crossDaDb.X * crossDaDb.X + crossDaDb.Y * crossDaDb.Y + crossDaDb.Z * crossDaDb.Z;
//	if (prod == 0 || FVector::DotProduct(dc, crossDaDb) != 0)
//	{
//		return false;
//	}
//	float res = FVector::DotProduct(
//		FVector::CrossProduct(dc, db), FVector::CrossProduct(da, db)) / prod;
//
//	_outIntersection = fromA + da * FVector(res, res, res);
//
//	FVector fromAToIntersectPoint = _outIntersection - fromA;
//	FVector fromBToIntersectPoint = _outIntersection - fromB;
//	FVector toAToIntersectPoint = _outIntersection - toA;
//	FVector toBToIntersectPoint = _outIntersection - toB;
//	if (FVector::DotProduct(fromAToIntersectPoint, fromBToIntersectPoint) <= 0 && 
//		FVector::DotProduct(toAToIntersectPoint, toBToIntersectPoint) <= 0
//	) {
//		return true;
//	}
//
//	return false;
//
//}
//
//
//// ���ַ���;������ֶ�ȡһ��
////TArray<FVector> ULcq_BlueprintFunctionLibrary::GetPointsByIndexRange(TArray<FVector> allPoints, int32 rangeStart, int32 rangeEnd)
////{
////	TArray<FVector> outPutPTS;
////	///
////	if (rangeStart>=0 && rangeEnd >=0 && rangeStart<allPoints.Num() && rangeEnd <allPoints.Num() )
////	{
////		if (rangeStart < rangeEnd)
////		{
////			for (int counter = rangeStart; counter <=rangeEnd ; ++counter)
////			{
////				outPutPTS.Add(allPoints[counter]);
////			}
////			
////		} else if (rangeStart > rangeEnd)
////		{
////			TArray<FVector> excludeAry;
////			for (int counter = rangeStart; counter <= rangeEnd; ++counter)
////			{
////				excludeAry.Add(allPoints[counter]);
////			}
////			for (int counter1 = 0; counter1 < excludeAry.Num();++counter1)
////			{
////				allPoints.Remove(excludeAry[counter1]);
////			}
////			outPutPTS = allPoints;
////		}
////	}
////
////	return outPutPTS;
////}
//
///*�����������л�ȡһ����������������е��ӳ��������������Ľ���
//	ע�⣺˳������ȴ�����������±꿪ʼ��������0;���㲻���������˵�
//	����1��������������
//	����2����Ӧ�ĵ����飨�����豣��һ�£�
//	����3������������±�
//	����4��5����ʼ�ͽ����±�
//	����6��7: ���ݽ��㱻�зֵ����������
//	����ֵ��true Ϊ�ɹ�,��֮��
//	*/
//bool ULcq_BlueprintFunctionLibrary::GetRayIntersectionOfVecInVecArr(
//	const TArray<FVector> _dirs, //������������
//	const TArray<FVector> _points, //��Ӧ�ĵ�����
//	const int _vecIndex,// ����������±�
//	const int _beginIndex, const int _endIndex,// ��ʼ�ͽ����±�
//	TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB)//�����ӳ��߽��㱻�зֵ����������;
//{
//	int t_dirsNum = _dirs.Num();//�������������
//	int t_pointsNum = _points.Num();//�����������
//
//	if (t_dirsNum > 3 && t_pointsNum > 3)
//	{
//		if (t_dirsNum == t_pointsNum)
//		{
//			if (_beginIndex >= 0 && _beginIndex < t_dirsNum)
//			{
//				if (_endIndex >= 0 && _endIndex < t_dirsNum)
//				{
//					/*
//					//����ǲ���ת���ڶ�����,ת���ڶ��־ͰѼ���������,û׬�����õ�ǰi����һ����i+1
//					t_indexNew���Ϊ��������±����һ������
//					*/
//					//int t_indexNew = _vecIndex == (t_dirsNum - 1) ? 0 : _vecIndex + 1;
//
//					//FVector t_beginA = _points[_vecIndex];//���浱ǰ��
//					//FVector t_endA = t_beginA + _dirs[_vecIndex];
//					//FVector t_intersectionPoint;
//
//					//for (int j = _beginIndex; j <= _endIndex; j++) { //���� ����ʼ����Ҫ����������ÿһ�β���					
//					//	if (j != _vecIndex && j != t_indexNew)
//					//	{
//					//		FVector t_beginB = _points[j];// 
//					//		if (CheckTwoLineIntersectionResult(t_beginA, t_endA, t_beginB, t_beginB + _dirs[j], t_intersectionPoint)/* == 2*/)
//					//		{
//					//			//���ָ�Ķ���ε���ӵ�
//					//			_dividePolygonA = GetPointsByIndexRange(_points, t_indexNew, j);
//					//			_dividePolygonA.Add(t_intersectionPoint);
//					//			UE_LOG(LogTemp, Warning, TEXT("���и������A����� ��������������� %d"), _dividePolygonA.Num());
//
//					//			_dividePolygonB = GetPointsByIndexRange(_points, j, t_indexNew);
//					//			if (_dividePolygonB.Num() > 0)
//					//			{
//					//				_dividePolygonB[0] = t_intersectionPoint;
//					//			}
//					//			UE_LOG(LogTemp, Warning, TEXT("���и������B����� ��������������� %d"), _dividePolygonB.Num());
//					//			return true;
//					//		}
//					//	}
//					//}
//
//					InsideGetIntersection();
//				}
//			}
//		}
//	}
//
//	return false;
//}


#include "Lcq_BlueprintFunctionLibrary.h"

//˼·���̣�
//1.�������򻯡���
//2.��������ת���ɶ�Ӧ�ķ����������顣
//3.�������ò�˲��ҵ���һ��������εİ��㡣
//4.������㿪ʼ�����ҵ��������������ӳ�����֮��������ϵĵ�һ�����㡣��ע��˳�򣬶����Ǵ�����0��ʼ��
//5.���������Ϊ���ްѰ�����η�Ϊ��������Ρ�
//6.�ݹ�����з��㷨��ֱ�������а�����γ���Ϊֹ��
//7.�õ��Ķ������ζ�������͹����Σ�����͹������㷨���㡣

static TArray<TArray<FVector>> t_polygonDividedArr;

/*
�Ӷ���ε�����ĵ������ȡ�����������������Ϣ����͹����Σ�
����1�� ��������
����2�� �Ƿ�����ʱ��
*/
void ULcq_BlueprintFunctionLibrary::GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise)
{
	//TArray<FLcq_PolygonData> t_res;
	//int t_pointsNum = _points.Num();
	//if (t_pointsNum > 3)
	//{
	//	t_polygonDividedArr.Empty();
	//	DividePolygonIfConcave(_points, _antiClockwise);  //�ݹ�
	//	if (t_polygonDividedArr.Num() > 0)
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("����ηָ����%d  �������"), t_res.Num());
	//	}
	//}
	//else
	//{
	//	//UE_LOG(LogTemp, Error, TEXT("����ηָ�ʧ��"));
	//}
}

/*�����������Zֵͳһ�� */
bool ULcq_BlueprintFunctionLibrary::SetPointsZvalueBySpecify(TArray<FVector>& _points, float _zValue)
{
	if (_points.Num() > 0)
	{
		for (int i = 0; i < _points.Num(); i++)
		{
			_points[i].Z = _zValue;
		}
		return true;
	}

	return false;
}

/*���ݵ������ȡ�������� 	*/
TArray<FVector> ULcq_BlueprintFunctionLibrary::GetVectorArrByPointsArr(const TArray<FVector> _points)
{
	TArray<FVector> t_res;
	int t_pointsNum = _points.Num();
	if (t_pointsNum > 1)
	{
		FVector t_p1;
		FVector t_p2;
		for (int i = 0; i < _points.Num(); i++)
		{
			t_p1 = _points[i];
			if (i == t_pointsNum - 1)
			{
				t_p2 = _points[0];
			}
			else
			{
				t_p2 = _points[i + 1];
			}

			t_res.Add(t_p2 - t_p1);
		}
	}

	return t_res;
}

/* �����������л�ȡһ����������������е��ӳ��������������Ľ���
	ע�⣺˳������ȴ�����������±꿪ʼ��������0;���㲻���������˵�
	����1��������������
	����2����Ӧ�ĵ����飨�����豣��һ�£�
	����3������������±�
	����4��5����ʼ�ͽ����±�
	����6��7: ���ݽ��㱻�зֵ����������
	����ֵ��true Ϊ�ɹ�,��֮��
*/
bool ULcq_BlueprintFunctionLibrary::GetRayIntersectionOfVecInVecArr(
	const TArray<FVector> _dirs, 
	const TArray<FVector> _points, 
	const int _vecIndex, //��ǰ�����±�
	const int _beginIndex, const int _endIndex,
	TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB)
{
	int t_dirsNum = _dirs.Num();
	int t_pointsNum = _points.Num();

	if (t_dirsNum > 3 && t_pointsNum > 3)
	{
		if (t_dirsNum == t_pointsNum)
		{
			if (_beginIndex >= 0 && _beginIndex < t_dirsNum)
			{
				if (_endIndex >= 0 && _endIndex < t_dirsNum)//��������ȷ��
				{
					/* t_indexNew����һ��Ҫ�����������±�*/
					int t_indexNew = (_vecIndex == (t_dirsNum - 1)) ? 0 : _vecIndex + 1; //���������� ���鵱ǰ�����±��Ƿ�鵽���һ����,�����ڹ�����,����Ϊ�¸������±�

					FVector t_beginA = _points[_vecIndex]; // ��ʾ��ǰ����ƥ��ĵ�ǰ�ĵ�
					FVector t_endA = t_beginA + _dirs[_vecIndex];// ��ʾ��������ϵ���һ���˵�
					FVector t_intersectionPoint; //�ӳ��߽���

					for (int j = _beginIndex; j <= _endIndex; j++) //�������ָ�����������еĵ�
					{
						if (j != _vecIndex && j != t_indexNew) //ǿ����������������߶���,�������������˵���
						{
							FVector t_beginB = _points[j]; 

							// ÿ��ѭ����ʱ�����ӳ��߽���
							if (CheckTwoLineIntersectionResult(t_beginA, t_endA, t_beginB, t_beginB + _dirs[j], t_intersectionPoint) == true /*2*/)
							{
								//���ָ�Ķ���ε���ӵ�
								_dividePolygonA = GetPointsByIndexRange(_points, t_indexNew, j);
								_dividePolygonA.Add(t_intersectionPoint);
								UE_LOG(LogTemp, Warning, TEXT("_dividePolygonA������������� %d"), _dividePolygonA.Num());

								_dividePolygonB = GetPointsByIndexRange(_points, j, t_indexNew);
								if (_dividePolygonB.Num() > 0)
								{
									_dividePolygonB[0] = t_intersectionPoint;
								}
								UE_LOG(LogTemp, Warning, TEXT("_dividePolygonB������������� %d"), _dividePolygonB.Num());
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}



/*
* �ָ�
	��������Ƿ��ǰ�����Σ�����Ǿ��и�
	����1���㼯
	����2����Ҫ���صı��и�����1
	����2����Ҫ���صı��и�����2
*/
void ULcq_BlueprintFunctionLibrary::DividePolygonIfConcave(TArray<FVector> _points, bool _antiClockwise, TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB)
{
	int t_pointsNum = _points.Num();
	if (_points.Num() < 3)
	{
		return;
	}
	if (t_pointsNum == 3)
	{
		t_polygonDividedArr.Add(_points);
		return;
	}
	if (t_pointsNum > 3)
	{
		//TArray<FVector> _dividePolygonA;
		//TArray<FVector> _dividePolygonB;
		float t_hei = _points[0].Z;
		SetPointsZvalueBySpecify(_points, 0);

		FVector t_p1;
		FVector t_p2;
		TArray<FVector> t_dirs = GetVectorArrByPointsArr(_points);

		bool t_divideResult = false;
		int t_indexNew = 0;
		for (int i = 0; i < t_dirs.Num(); i++)
		{
			t_p1 = t_dirs[i];
			if (i == t_dirs.Num() - 1)
			{
				t_p2 = t_dirs[0];
				t_indexNew = 0;
			}
			else
			{
				t_p2 = t_dirs[i + 1];
				t_indexNew = i + 1;
			}

			float t_rotateDir = FVector::CrossProduct(t_p1, t_p2).Z;

			//�����ǰ������
			if (t_rotateDir < -0.01f && _antiClockwise == true || t_rotateDir>0.01f && _antiClockwise == false)
			{
				//UE_LOG(LogTemp, Warning, TEXT("t_rotateDir:   %f"), t_rotateDir);
				UE_LOG(LogTemp, Warning, TEXT("�ǰ������~~~~~~~~~~~"));
				//��ָ��

				t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, i, t_pointsNum - 1, _dividePolygonA, _dividePolygonB);
				if (t_divideResult == false)
				{
					t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, 0, i - 1, _dividePolygonA, _dividePolygonB);
				}
				
				if (t_divideResult == false)
				{
					UE_LOG(LogTemp, Error, TEXT("�߶�%d  û�еõ��ָ��"), i);
				}
				break;
			}
		}

		if (t_divideResult == false)
		{
			SetPointsZvalueBySpecify(_points, t_hei);
			t_polygonDividedArr.Add(_points);
		}
		else
		{
			if (_dividePolygonA.Num() > 2)
			{
				SetPointsZvalueBySpecify(_dividePolygonA, t_hei);
				DividePolygonIfConcave(_dividePolygonA, _antiClockwise,_dividePolygonA, _dividePolygonB);
			}
			if (_dividePolygonB.Num() > 2)
			{
				SetPointsZvalueBySpecify(_dividePolygonB, t_hei);
				DividePolygonIfConcave(_dividePolygonB, _antiClockwise, _dividePolygonA, _dividePolygonB);
			}
		}
	}
}



TArray<FVector> ULcq_BlueprintFunctionLibrary::GetPointsByIndexRange(const TArray<FVector>& _points, int IndexParam1, int IndexParam2)
{
	TArray<FVector> FinalArr ;
	FinalArr.Empty();

	TArray<FVector> points = _points;
	TArray<FVector> temparr;
	if (IndexParam1 < IndexParam2)
	{
		for (int i = IndexParam1; i <= IndexParam2; ++i)
		{
			FinalArr.Add(_points[i]);
		}

		temparr = FinalArr;
	}
	else if (IndexParam1 > IndexParam2)
	{
		for (int i = 0; i < temparr.Num(); ++i)
		{
			temparr[i];
			points.Remove(temparr[i]);
		}
		FinalArr = points;
	}

	return FinalArr;
}

bool ULcq_BlueprintFunctionLibrary::CheckTwoLineIntersectionResult(const FVector& fromA, const FVector& toA, const FVector& fromB, const FVector& toB, FVector& _outIntersection)
{
	FVector da = fromB - fromA;
 FVector db = toB - toA;
 FVector dc = toA - fromA;
 
 FVector crossDaDb = FVector::CrossProduct(da, db);
 float prod = crossDaDb.X * crossDaDb.X + crossDaDb.Y * crossDaDb.Y + crossDaDb.Z * crossDaDb.Z;
 if(prod == 0 || FVector::DotProduct(dc,crossDaDb) != 0)
 {
     return false;
 }
 float res = FVector::DotProduct(FVector::CrossProduct(dc, db), FVector::CrossProduct(da, db)) / prod;
 
 _outIntersection = fromA + da * FVector(res, res, res);
 
 FVector fromAToIntersectPoint = _outIntersection - fromA;
 FVector fromBToIntersectPoint = _outIntersection - fromB;
 FVector toAToIntersectPoint = _outIntersection - toA;
 FVector toBToIntersectPoint = _outIntersection - toB;
 if (FVector::DotProduct(fromAToIntersectPoint, fromBToIntersectPoint) <= 0 && FVector::DotProduct(toAToIntersectPoint, toBToIntersectPoint) <= 0)
 {
     return true;
 }
 return false;
}

