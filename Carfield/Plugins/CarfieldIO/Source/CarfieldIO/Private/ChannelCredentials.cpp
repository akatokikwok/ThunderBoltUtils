#include "ChannelCredentials.h"
#include "CarfieldIO.h"

#include "GrpcIncludesBegin.h"

#include <grpc++/security/credentials.h>

#include "GrpcIncludesEnd.h"

UChannelCredentials* UChannelCredentials::MakeGoogleDefaultCredentials()
{
    return NewObject<UGoogleDefaultCredentials>();
}

UChannelCredentials* UChannelCredentials::MakeSslCredentials(FString PemRootCerts, FString PemPrivateKey, FString PemCertChain)
{
    USslCredentials* const SslCredentials = NewObject<USslCredentials>();

    SslCredentials->PemRootCerts = PemRootCerts;
    SslCredentials->PemPrivateKey = PemPrivateKey;
    SslCredentials->PemCertChain = PemCertChain;

    return SslCredentials;
}

UChannelCredentials* UChannelCredentials::MakeInsecureChannelCredentials()
{
    return NewObject<UInsecureChannelCredentials>();
}

#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformTypes.h"
#endif
