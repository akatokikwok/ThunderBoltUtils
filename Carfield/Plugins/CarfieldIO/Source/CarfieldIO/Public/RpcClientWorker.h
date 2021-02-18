#pragma once

#include "CoreMinimal.h"

#include "Containers/Queue.h"
#include "ChannelCredentials.h"
#include "HAL/Runnable.h"

#include <memory>
#include <chrono>

class UChannelCredentials;

//namespace grpc
//{
//    class Channel;
//}

/**
 * 基本RPC客户端工作程序，它生命周期在于一个单独的线程中，并用响应更新所有conduits
 */
class CARFIELDIO_API RpcClientWorker : public FRunnable
{
public:
    RpcClientWorker();
	virtual ~RpcClientWorker();

    virtual uint32 Run() override;

    // RpcClientWorker类接口;拿取本类的bPendingStopped
    FORCEINLINE bool IsPendingStopped() const
    {
        return bPendingStopped;
    }

    FORCEINLINE void MarkPendingStopped()
    {
        bPendingStopped = true;
    }

    virtual bool HierarchicalInit() = 0;
	virtual void HierarchicalUpdate() = 0;

    void DispatchError(const FString& ErrorMessage);

//public:
    FString URI;
    UChannelCredentials* ChannelCredentials;

    TQueue<FRpcError>* ErrorMessageQueue;

protected:
    // 线程要关闭的时候设置为true
	volatile bool bPendingStopped;
};
