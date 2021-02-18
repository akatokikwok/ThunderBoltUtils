#include "RpcClient.h"

#include "CarfieldIO.h"
#include "RpcClientWorker.h"
#include "GrpcUriValidator.h"

#include "Containers/Ticker.h"
#include "Misc/CoreDelegates.h"

#include "Misc/DefaultValueHelper.h"
#include "HAL/RunnableThread.h"
#include "Kismet/KismetStringLibrary.h"

// ============ 客户端实现 ===========

bool URpcClient::Init(const FString& URI, UChannelCredentials* ChannelCredentials)
{
    if (bCanSendRequests)
    {
        UE_LOG(LogTemp, Error, TEXT("不要尝试重复初始化客户端"));
        return true;
    }
    
    FString ErrorMessage;
    if (!FGrpcUriValidator::Validate(URI, ErrorMessage))
    {
        UE_LOG(LogTemp, Error, TEXT("%s 无法确认的URI: %s"), *(GetClass()->GetName()), *ErrorMessage);
    }

    // 线程未被创建时的操作
    if (Thread == nullptr)
    {
		// 启动分层级的初始化并创建一个具体实现
        HierarchicalInit();

        // Retrieve and set an Error Message Queue
        if (InnerWorker)
        {
            InnerWorker->URI = URI;
            InnerWorker->ChannelCredentials = ChannelCredentials;

            InnerWorker->ErrorMessageQueue = &ErrorMessageQueue;

            const FString ThreadName(FString::Printf(TEXT("客户端线程 %s %d"), *(GetClass()->GetName()), FMath::RandRange(0, TNumericLimits<int32>::Max())));
            Thread = FRunnableThread::Create(InnerWorker.Get(), *ThreadName);

            bCanSendRequests = true;
            UE_LOG(LogTemp, Verbose, TEXT("正在构造线程: %s, address %lld"), *ThreadName, reinterpret_cast<int64>(InnerWorker.Get()));
        }
        else
        {
            UE_LOG(LogTemp, Fatal, TEXT("一个内置工厂 %s 并没有被初始化"), *(GetClass()->GetName()));
        }
    }

    if (CanSendRequests())
    {
        TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float)
        {
            if (!ErrorMessageQueue.IsEmpty())
            {
                FRpcError ReceivedError;
                ErrorMessageQueue.Dequeue(ReceivedError);
                EventRpcError.Broadcast(this, ReceivedError);

            }
            else
            {
                HierarchicalUpdate();
            }

            return true;
        }));
    }

    return bCanSendRequests;
}

URpcClient::URpcClient() : InnerWorker(nullptr), TickDelegateHandle()
{
}

URpcClient::~URpcClient()
{
    UE_LOG(LogTemp, Verbose, TEXT("客户端实例生命周期结束. 仍然在发送以下请求: %s"),
           *UKismetStringLibrary::Conv_BoolToString(CanSendRequests()));
}

void URpcClient::Update()
{
}

bool URpcClient::CanSendRequests() const
{
    return bCanSendRequests;
}

URpcClient* URpcClient::CreateRpcClient(TSubclassOf<URpcClient> Class, FRpcClientInstantiationParameters InstantiationParameters, UObject* Outer)
{
    const FString& URI = FString::Printf(TEXT("%s:%d"), *(InstantiationParameters.Ip), InstantiationParameters.Port);
    return CreateRpcClientUri(Class, URI, InstantiationParameters.ChannelCredentials, Outer);
}

URpcClient* URpcClient::CreateRpcClientUri(TSubclassOf<URpcClient> Class, const FString& URI, UChannelCredentials* ChannelCredentials, UObject* Outer)
{
    UObject* const RealOuter = Outer ? Outer : (UObject*)GetTransientPackage();
    
    if (URpcClient* const CreatedClient = NewObject<URpcClient>(RealOuter, *Class))
    {
        bool IsClientInitialized = CreatedClient->Init(URI, ChannelCredentials);
        if (!IsClientInitialized)
        {
            UE_LOG(LogTemp, Error, TEXT("无法初始化客户端 (%s::Init() failed"), *(Class->GetName()));
            return nullptr;
        }
        else
        {
            UE_LOG(LogTemp, Verbose, TEXT(" %s 已经被创建且初始化"), *(Class->GetName()));
            return CreatedClient;
        }
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("无法创建客户端实例 (NewObject<%s>() failed)"), *(Class->GetName()));
        return nullptr;
    }
}

void URpcClient::BeginDestroy()
{
	// 垃圾回收的时候被同步调用
	if (CanSendRequests())
    {
        Stop(true);
    }

    Super::BeginDestroy();
}

void URpcClient::Stop(bool bSynchronous)
{
    FRunnableThread* ThreadToStop = Thread.Exchange(nullptr);

    if (ThreadToStop)
    {
        if (!InnerWorker->IsPendingStopped())
            InnerWorker->MarkPendingStopped();

        bCanSendRequests = false;
        UE_LOG(LogTemp, Verbose, TEXT("借由设置 'bCanSendRequests = false', address %lld %s 来计划停止"), *(GetClass()->GetName()), reinterpret_cast<int64>(InnerWorker.Get()));

        // 确保在任意时间操作的时候都调用
        ThreadToStop->Kill(bSynchronous);

        delete ThreadToStop;
        ThreadToStop = nullptr;
        
        FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("不能调用Stop方法给一个已经停止的实例 '%s'"), *(GetClass()->GetName()));
    }
}
