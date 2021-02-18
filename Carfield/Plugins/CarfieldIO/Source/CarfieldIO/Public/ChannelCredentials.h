#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <memory>

#include "ChannelCredentials.generated.h"

// namespace grpc
// {
//     class ChannelCredentials;
// }

USTRUCT(BlueprintType)
struct CARFIELDIO_API FRpcError
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadOnly, Category=Grpc)
    FString ErrorMessage;
};


/**
* UChannelCredentials对象封装了客户端所需的所有状态
* 对给定频道进行服务器身份验证。
* 允许作出各种断言，例如，关于客户的身份，角色,该channel的所有调用。

 * A channel credentials object encapsulates all the state needed by a client
 * to authenticate with a server for a given channel.
 * It can make various assertions, e.g., about the client’s identity, role
 * for all the calls on that channel.
 */
UCLASS(NotBlueprintable, NotBlueprintType, notplaceable, noteditinlinenew, hidedropdown, Transient, Abstract)
class CARFIELDIO_API UChannelCredentials : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 使用默认凭据构造
     *
     * \warning Only use these credentials when connecting to a Google endpoint.
     * Using these credentials to connect to any other service may result in this
     * service being able to impersonate your client for requests to Google
     * services.
     * @return Google Default Credentials
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Vizor|RPC Credentials")
    static UChannelCredentials* MakeGoogleDefaultCredentials();

    /**
     * 根据SSL特定选项生成SSL凭据。
     *
     * @param PemRootCerts
     *        The buffer containing the PEM encoding of the server root certificates. If
     *        this parameter is empty, the default roots will be used.  The default
     *        roots can be overridden using the \a GRPC_DEFAULT_SSL_ROOTS_FILE_PATH
     *        environment variable pointing to a file on the file system containing the
     *        roots.
     * @param PemPrivateKey
     *        The buffer containing the PEM encoding of the client's private key. This
     *        parameter can be empty if the client does not have a private key.
     * @param PemCertChain
     *        The buffer containing the PEM encoding of the client's certificate chain.
     *        This parameter can be empty if the client does not have a certificate
     *        chain.
     * @return Ssl Credentials
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Vizor|RPC Credentials")
    static UChannelCredentials* MakeSslCredentials(
        UPARAM(DisplayName="PEM Root Certificates") FString PemRootCerts,
        UPARAM(DisplayName="PEM Private Key") FString PemPrivateKey,
        UPARAM(DisplayName="PEM Certificate Chain") FString PemCertChain
    );

    /**
     * 为未加密、未验证的通道生成凭据。
     *
     * @return Insecure Channel Credentials
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Vizor|RPC Credentials")
    static UChannelCredentials* MakeInsecureChannelCredentials();
};

/**
 * 生成具有合理值的凭据
 *
 * \warning Only use these credentials when connecting to a Google endpoint.
 * Using these credentials to connect to any other service may result in this
 * service being able to impersonate your client for requests to Google
 * services.
 */
UCLASS()
class CARFIELDIO_API UGoogleDefaultCredentials : public UChannelCredentials
{
    GENERATED_BODY()

public:
};

/**
 * 给定SSL特定选项,生成SSL凭据，
 */
UCLASS()
class CARFIELDIO_API USslCredentials : public UChannelCredentials
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Transient, DisplayName="PEM Root Certificates", Category=GrpcCerts)
    FString PemRootCerts;

    UPROPERTY(BlueprintReadOnly, Transient, DisplayName="PEM Private Key", Category=GrpcCerts)
    FString PemPrivateKey;

    UPROPERTY(BlueprintReadOnly, Transient, DisplayName="PEM Certificate Chain", Category=GrpcCerts)
    FString PemCertChain;
};

/**
 * 未加密、未认证频道的凭据
 */
UCLASS()
class CARFIELDIO_API UInsecureChannelCredentials : public UChannelCredentials
{
    GENERATED_BODY()

public:
};

/**
 * 实例化参数用于创建RPC客户端。
 */
USTRUCT(BlueprintType)
struct CARFIELDIO_API FRpcClientInstantiationParameters
{
    GENERATED_BODY()

    /**
     * 要连接到的末端的IP地址。
     */
    UPROPERTY(BlueprintReadWrite, Category=Endpoint)
    FString Ip;

    /**
     * 要连接到的末端的端口
     */
    UPROPERTY(BlueprintReadWrite, Category=EndpointPort)
    int32 Port;

    /**
	 * 用于创建的RPC客户端的凭据。
     */
    UPROPERTY(BlueprintReadWrite, Category=Credentials)
    UChannelCredentials* ChannelCredentials;

    /**
     * 获取当前ip地址和端口的GRPC URI。
     *
     * @return URI for channel instantiation.
     */
    FString GetURI() const
    {
        return FString::Printf(TEXT("%s:%d"), *Ip, Port);
    }

    /**
     * 拿到当前FRpcClientInstantiation参数的字符串
     *
     * @return FRpcClientInstantiationParameters string representation.
     */
    FString GetName() const
    {
        const FString& ParamsURI = GetURI();
        const FString& CredentialsClassName = ChannelCredentials ? *(ChannelCredentials->GetClass()->GetName()) : TEXT("nullptr (DANGER!)");

        return FString::Printf(TEXT("URI: %s, Credentials: %s"), *ParamsURI, *CredentialsClassName);
    }
};
