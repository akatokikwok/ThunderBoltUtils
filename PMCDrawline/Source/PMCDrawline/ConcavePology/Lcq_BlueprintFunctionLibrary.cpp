//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Lcq_BlueprintFunctionLibrary.h"
//
//static TArray<TArray<FVector>> t_polygonDividedArr; //多边形所有点的集合
//
///* 根据点数组获取向量数组: 就是给一些点,然后每邻接2个点组一个向量	*/
//TArray<FVector> ULcq_BlueprintFunctionLibrary::GetVectorArrByPointsArr(const TArray<FVector> _points)
//{
//	TArray<FVector> t_res;
//	int t_pointsNum = _points.Num();//入参点数量
//	if (t_pointsNum > 1)
//	{
//		FVector t_p1;
//		FVector t_p2;
//		for (int i = 0; i < _points.Num(); i++)
//		{
//			t_p1 = _points[i];
//			if (i == t_pointsNum - 1)
//			{
//				t_p2 = _points[0];//查到最后一个就把t2设定为起点
//			}
//			else
//			{
//				t_p2 = _points[i + 1];//没查找到最后就把t2设定为t1的下一个
//			}
//
//			t_res.Add(t_p2 - t_p1);//将各个向量注册进 向量数组t_res
//		}
//	}
//
//	return t_res;
//}
//
////分割
///*检查多边形是否是凹多边形，如果是就切割
//	参数1：点集
//	参数2：需要返回的被切割多边形1
//	参数2：需要返回的被切割多边形2
//	*/
//void ULcq_BlueprintFunctionLibrary::DividePolygonIfConcave(TArray<FVector> _points, bool _antiClockwise)
//{
//	int t_pointsNum = _points.Num();//外部点数量
//
//	// 两种不良情况处理
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
//	// 正常多边形(>=4)的处理
//	if (t_pointsNum > 3)
//	{
//		TArray<FVector> _dividePolygonA;
//		TArray<FVector> _dividePolygonB;
//		FVector t_p1;
//		FVector t_p2;
//
//		float t_hei = _points[0].Z;//取第一个点高度
//
//		SetPointsZvalueBySpecify(_points, 0);//统一化所有点高度为0
//
//		
//
//		TArray<FVector> t_dirs = GetVectorArrByPointsArr(_points);//拿到所有 指向向量 的集合
//
//		bool t_divideResult = false; //切割结果标记
//		int t_indexNew = 0;			
//		/// 遍历所有向量
//		for (int i = 0; i < t_dirs.Num(); i++) 
//		{
//			t_p1 = t_dirs[i]; 
//			if (i == t_dirs.Num() - 1) //查找到了最后一个向量的时候需要将T_P2置为点集起点  并 重置 查找索引
//			{
//				t_p2 = t_dirs[0];
//				t_indexNew = 0;
//			}
//			else // 处于遍历过程中的时候 将t_p2设定为 下一次数 查到的向量 并更新查找索引为下一个
//			{
//				t_p2 = t_dirs[i + 1];
//				t_indexNew = i + 1; //查找索引更新为下一个
//			}
//
//			float t_rotateDir = FVector::CrossProduct(t_p1, t_p2).Z;//保存邻接2个向量叉积的z分量,以此作为评定凹多边形标准
//
//			//根据右手定律检查出是凹多边形;满足任一情况即可
//			if ((t_rotateDir < -0.01f && _antiClockwise == true) || (t_rotateDir>0.01f && _antiClockwise == false))
//			{
//				//打印一下
//				UE_LOG(LogTemp, Warning, TEXT("当前遍历i次数确认是凹多边形--------"));
//
//				//求分割点
//				t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, i, t_pointsNum - 1, _dividePolygonA, _dividePolygonB);
//				if (t_divideResult == false)
//				{
//					t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, 0, i - 1, _dividePolygonA, _dividePolygonB);
//				}
//				if (t_divideResult == false)
//				{
//					UE_LOG(LogTemp, Error, TEXT("线段%d  没有得到分割点"), i);
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
///*利用切割函数从多边形的有序的点数组获取顶点和三角面数据信息（凹凸多边形）
//	参数1： 顶点数组
//	参数2： 是否是逆时针*/
//void ULcq_BlueprintFunctionLibrary::GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise)
//{
//	TArray<FLcq_PolygonData> t_res;//这里理解为 每一个多边面就携带各自的数据;有多少个数据就有多少个多边面
//	int t_pointsNum = _points.Num();
//
//	if (t_pointsNum > 3)//针对4个点及以上的正常情况
//	{
//		t_polygonDividedArr.Empty();
//		DividePolygonIfConcave(_points, _antiClockwise);  //执行递归切割凹多边形;并开启逆时针方向绘制
//		if (t_polygonDividedArr.Num() > 0)
//		{
//			//UE_LOG(LogTemp, Warning, TEXT("多边形分割成了%d个多边形"), t_res.Num());
//		}
//	}
//	else //异常情况
//	{
//		//UE_LOG(LogTemp, Warning, TEXT("多边形分割失败"));
//	}
//}
//
//
///*给定点数组的Z值统一设定定值*/
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
//// 助手方法:检测延长线是否与某一向量相交
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
//// 助手方法;对数组分段取一段
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
///*从向量数组中获取一个向量在这个数组中的延长线与其他向量的交点
//	注意：顺序必须先从这个向量的下标开始，不能是0;交点不包括向量端点
//	参数1：方向向量数组
//	参数2：对应的点数组（长度需保持一致）
//	参数3：这个向量的下标
//	参数4，5：开始和结束下标
//	参数6，7: 根据交点被切分的两组点数组
//	返回值：true 为成功,反之无
//	*/
//bool ULcq_BlueprintFunctionLibrary::GetRayIntersectionOfVecInVecArr(
//	const TArray<FVector> _dirs, //方向向量数组
//	const TArray<FVector> _points, //对应的点数组
//	const int _vecIndex,// 这个向量的下标
//	const int _beginIndex, const int _endIndex,// 开始和结束下标
//	TArray<FVector>& _dividePolygonA, TArray<FVector>& _dividePolygonB)//根据延长线交点被切分的两组点数组;
//{
//	int t_dirsNum = _dirs.Num();//向量数组的数量
//	int t_pointsNum = _points.Num();//点数组的数量
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
//					//检查是不是转到第二轮了,转到第二轮就把计数器重置,没赚到就拿当前i的下一个即i+1
//					t_indexNew理解为这个向量下标的下一个计数
//					*/
//					//int t_indexNew = _vecIndex == (t_dirsNum - 1) ? 0 : _vecIndex + 1;
//
//					//FVector t_beginA = _points[_vecIndex];//保存当前点
//					//FVector t_endA = t_beginA + _dirs[_vecIndex];
//					//FVector t_intersectionPoint;
//
//					//for (int j = _beginIndex; j <= _endIndex; j++) { //遍历 从起始序数要结束序数的每一次操作					
//					//	if (j != _vecIndex && j != t_indexNew)
//					//	{
//					//		FVector t_beginB = _points[j];// 
//					//		if (CheckTwoLineIntersectionResult(t_beginA, t_endA, t_beginB, t_beginB + _dirs[j], t_intersectionPoint)/* == 2*/)
//					//		{
//					//			//给分割的多边形点组加点
//					//			_dividePolygonA = GetPointsByIndexRange(_points, t_indexNew, j);
//					//			_dividePolygonA.Add(t_intersectionPoint);
//					//			UE_LOG(LogTemp, Warning, TEXT("被切割出来的A多边形 里向量数组个数： %d"), _dividePolygonA.Num());
//
//					//			_dividePolygonB = GetPointsByIndexRange(_points, j, t_indexNew);
//					//			if (_dividePolygonB.Num() > 0)
//					//			{
//					//				_dividePolygonB[0] = t_intersectionPoint;
//					//			}
//					//			UE_LOG(LogTemp, Warning, TEXT("被切割出来的B多边形 里向量数组个数： %d"), _dividePolygonB.Num());
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

//思路流程：
//1.顶点有序化。（
//2.顶点数组转换成对应的方向向量数组。
//3.依次利用叉乘查找到第一个凹多边形的凹点。
//4.从这个点开始往后找到这个凹点的向量延长先在之后的向量上的第一个交点。（注意顺序，而不是从数组0开始）
//5.以这个交点为界限把凹多边形分为两个多边形。
//6.递归这个切分算法，直至不再有凹多边形出现为止。
//7.得到的多个多边形都是属于凸多边形，利用凸多边形算法计算。

static TArray<TArray<FVector>> t_polygonDividedArr;

/*
从多边形的有序的点数组获取顶点和三角面数据信息（凹凸多边形）
参数1： 顶点数组
参数2： 是否是逆时针
*/
void ULcq_BlueprintFunctionLibrary::GetPolygonDataFromOrderVertexs(TArray<FVector> _points, bool _antiClockwise)
{
	//TArray<FLcq_PolygonData> t_res;
	//int t_pointsNum = _points.Num();
	//if (t_pointsNum > 3)
	//{
	//	t_polygonDividedArr.Empty();
	//	DividePolygonIfConcave(_points, _antiClockwise);  //递归
	//	if (t_polygonDividedArr.Num() > 0)
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("多边形分割成了%d  个多边形"), t_res.Num());
	//	}
	//}
	//else
	//{
	//	//UE_LOG(LogTemp, Error, TEXT("多边形分割失败"));
	//}
}

/*给定点数组的Z值统一化 */
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

/*根据点数组获取向量数组 	*/
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

/* 从向量数组中获取一个向量在这个数组中的延长线与其他向量的交点
	注意：顺序必须先从这个向量的下标开始，不能是0;交点不包括向量端点
	参数1：方向向量数组
	参数2：对应的点数组（长度需保持一致）
	参数3：这个向量的下标
	参数4，5：开始和结束下标
	参数6，7: 根据交点被切分的两组点数组
	返回值：true 为成功,反之无
*/
bool ULcq_BlueprintFunctionLibrary::GetRayIntersectionOfVecInVecArr(
	const TArray<FVector> _dirs, 
	const TArray<FVector> _points, 
	const int _vecIndex, //当前向量下标
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
				if (_endIndex >= 0 && _endIndex < t_dirsNum)//以上条件确保
				{
					/* t_indexNew是下一个要查找向量的下标*/
					int t_indexNew = (_vecIndex == (t_dirsNum - 1)) ? 0 : _vecIndex + 1; //索引查找器 检验当前向量下标是否查到最后一个了,若处于过程中,就设为下个向量下标

					FVector t_beginA = _points[_vecIndex]; // 表示当前向量匹配的当前的点
					FVector t_endA = t_beginA + _dirs[_vecIndex];// 表示这个向量上的下一个端点
					FVector t_intersectionPoint; //延长线交点

					for (int j = _beginIndex; j <= _endIndex; j++) //遍历入参指定区间内所有的点
					{
						if (j != _vecIndex && j != t_indexNew) //强制这个交点必须打在线段上,不允许在两个端点上
						{
							FVector t_beginB = _points[j]; 

							// 每次循环的时候检查延长线交点
							if (CheckTwoLineIntersectionResult(t_beginA, t_endA, t_beginB, t_beginB + _dirs[j], t_intersectionPoint) == true /*2*/)
							{
								//给分割的多边形点组加点
								_dividePolygonA = GetPointsByIndexRange(_points, t_indexNew, j);
								_dividePolygonA.Add(t_intersectionPoint);
								UE_LOG(LogTemp, Warning, TEXT("_dividePolygonA向量数组个数： %d"), _dividePolygonA.Num());

								_dividePolygonB = GetPointsByIndexRange(_points, j, t_indexNew);
								if (_dividePolygonB.Num() > 0)
								{
									_dividePolygonB[0] = t_intersectionPoint;
								}
								UE_LOG(LogTemp, Warning, TEXT("_dividePolygonB向量数组个数： %d"), _dividePolygonB.Num());
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
* 分割
	检查多边形是否是凹多边形，如果是就切割
	参数1：点集
	参数2：需要返回的被切割多边形1
	参数2：需要返回的被切割多边形2
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

			//检查出是凹多边形
			if (t_rotateDir < -0.01f && _antiClockwise == true || t_rotateDir>0.01f && _antiClockwise == false)
			{
				//UE_LOG(LogTemp, Warning, TEXT("t_rotateDir:   %f"), t_rotateDir);
				UE_LOG(LogTemp, Warning, TEXT("是凹多边形~~~~~~~~~~~"));
				//求分割点

				t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, i, t_pointsNum - 1, _dividePolygonA, _dividePolygonB);
				if (t_divideResult == false)
				{
					t_divideResult = GetRayIntersectionOfVecInVecArr(t_dirs, _points, i, 0, i - 1, _dividePolygonA, _dividePolygonB);
				}
				
				if (t_divideResult == false)
				{
					UE_LOG(LogTemp, Error, TEXT("线段%d  没有得到分割点"), i);
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

