#include "RpcClientWorker.h"

#include "CarfieldIO.h"

#include "HAL/PlatformTime.h"
#include "GenUtils.h"

#include "GrpcIncludesBegin.h"

#include <grpc++/channel.h>
#include <grpc++/create_channel.h>

#include "GrpcIncludesEnd.h"


// ========= 客户端工厂实现 ========

RpcClientWorker::RpcClientWorker() : bPendingStopped(false)
{
}

RpcClientWorker::~RpcClientWorker()
{
}

uint32 RpcClientWorker::Run()
{
    // Channel未被创建时的操作
    if (HierarchicalInit())
        bPendingStopped = false;
    else
        return 1;

    // 更新至未挂起停止
    while (!bPendingStopped)
    {
        UE_LOG(LogTemp, Verbose, TEXT("使用HierarchicalUpdate()进行更新"));

        HierarchicalUpdate();
        FPlatformProcess::Sleep(0.1f);
    }

    return 0;
}

void RpcClientWorker::DispatchError(const FString& ErrorMessage)
{
    UE_CLOG(!ErrorMessageQueue, LogTemp, Fatal, TEXT("由于错误消息为空所以无法派送"));

    FRpcError Error;
    Error.ErrorMessage = ErrorMessage;

    ErrorMessageQueue->Enqueue(Error);
}

#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformTypes.h"
#endif
