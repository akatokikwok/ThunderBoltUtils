/*
 */

#include "GrpcUriValidator.h"
#include "CarfieldIO.h"
#include "Misc/DefaultValueHelper.h"

class FGrpcUriValidator_Internal
{
public:
    static bool ValidatePort(const FString& MaybePort, FString& OutError);
    static bool ValidateIp(const FString& MaybeIpAddress, FString& OutError);
    static bool ValidateDomainName(const FString& MaybeDomainName, FString& OutError);
    
    static bool DoesHostLookLikeIp(const FString& MaybeIpAddress);
    
private:
    static bool TCharIsLetter(TCHAR character);
    static bool TCharIsDigit(TCHAR character);
};


/// FGrpcUriValidator_Internal类内部接口实现

bool FGrpcUriValidator_Internal::ValidatePort(const FString& MaybePort, FString& OutError)
{
    int32 PortAsInteger;
    if (FDefaultValueHelper::ParseInt(MaybePort, PortAsInteger))
    {
        const TRange<int32> ValidPortRange(0, 0xFFFF);
        
        if (!ValidPortRange.Contains(PortAsInteger))
        {
            OutError = FString::Printf(TEXT("非法端口号: \"%d\", 必须位于以下范围 [%d - %d)"), PortAsInteger, 0, 0xFFFF);
            return false;
        }
    }
    else
    {
        OutError = FString::Printf(TEXT("无法解析端口 \"%s\" 为一个整数. 格式非法."), *MaybePort);
        return false;
    }
    
    return true;
}

bool FGrpcUriValidator_Internal::ValidateIp(const FString& MaybeIpAddress, FString& OutError)
{
    TArray<FString> Octets;
    MaybeIpAddress.ParseIntoArray(Octets, TEXT("."));
    
    if (Octets.Num() == 4)
    {
        const TRange<int32> OctetRange(0, 0xFF);
        for (const FString& Octet : Octets)
        {
            int32 Out;
            if (FDefaultValueHelper::ParseInt(Octet, Out))
            {
                if (!OctetRange.Contains(Out))
                {
                    OutError = FString::Printf(TEXT(" \"%s\" 若位于IPV4地址，需要满足合法范围 (\"%s\") is of range [0 - 256)"), *Octet, *MaybeIpAddress);
                    return false;
                }
            }
            else
            {
                OutError = FString::Printf(TEXT("\"%s\" in \"%s\" 非int32型"), *Octet, *MaybeIpAddress);
                return false;
            }
        }
    }
    else
    {
        OutError = FString::Printf(TEXT("不能解析IPV4地址 (which is \"%s\") 为一个字符串集合,也可能是大小超出容量"), *MaybeIpAddress);
        return false;
    }
    
    return true;
}

bool FGrpcUriValidator_Internal::ValidateDomainName(const FString& MaybeDomainName, FString& OutError)
{
    for (TCHAR Character : MaybeDomainName)
    {
        if (!TCharIsLetter(Character) && !TCharIsDigit(Character) && (Character != TEXT('-')) && (Character != TEXT('.')))
        {
            OutError = FString::Printf(TEXT("\"%s\" 域名包含非法字符: \"%c\""), *MaybeDomainName, Character);
            return false;
        }
    }
    
    return true;
}

bool FGrpcUriValidator_Internal::DoesHostLookLikeIp(const FString& MaybeIpAddress)
{
    for (TCHAR Character : MaybeIpAddress)
    {
        if (!TCharIsDigit(Character) && (Character != TEXT('.')))
            return false;
    }
    
    return true;
}

bool FGrpcUriValidator_Internal::TCharIsLetter(TCHAR character)
{
    const bool bUpperCase = (character >= TEXT('A')) && (character <= TEXT('Z'));
    const bool bLowerCase = (character >= TEXT('a')) && (character <= TEXT('z'));
    
    return bUpperCase || bLowerCase;
}

bool FGrpcUriValidator_Internal::TCharIsDigit(TCHAR character)
{
    return (character >= TEXT('0')) && (character <= TEXT('9'));
}


/// FGrpcUriValidator 接口

bool FGrpcUriValidator::Validate(const FString& MaybeGrpcUri, FString& OutError)
{
    static const FString SchemeSeparator(TEXT("://"));
    
    const int32 IndexOfSchemeSeparator = MaybeGrpcUri.Find(SchemeSeparator);
    if (IndexOfSchemeSeparator >= 0)
    {
        const FString& Scheme = MaybeGrpcUri.Mid(0, IndexOfSchemeSeparator);
        
        OutError = FString::Printf(TEXT("GRPC URI \"%s\" must not contain a URL scheme (\"%s\" provided).GRPC禁止显式语义."), *MaybeGrpcUri, *Scheme);
        return false;
    }
    
    const int32 PathSeparatorIndex = MaybeGrpcUri.Find(TEXT("/"));
    const bool bHasPathSeparator = PathSeparatorIndex >= 0;
    
    const int32 PortSeparatorIndex = MaybeGrpcUri.Find(TEXT(":"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, (PathSeparatorIndex >= 0) ? PathSeparatorIndex : INDEX_NONE);
    const bool bHasPortSeparator = PortSeparatorIndex >= 0;
    
    FString GrpcHostName = TEXT("");
    FString GrpcPort = TEXT("80");
    
    if (bHasPortSeparator)
    {
        GrpcHostName = MaybeGrpcUri.Mid(0, PortSeparatorIndex);
        
        const int32 PortSubstringStart = PortSeparatorIndex + 1;
        if (bHasPathSeparator)
        {
            GrpcPort = MaybeGrpcUri.Mid(PortSubstringStart, (PathSeparatorIndex - PortSubstringStart));
        }
        else
        {
            GrpcPort = MaybeGrpcUri.Mid(PortSubstringStart);
        }
    }
    else
    {
        if (bHasPathSeparator)
        {
            GrpcHostName = MaybeGrpcUri.Mid(0, PathSeparatorIndex);
        }
        else
        {
            GrpcHostName = MaybeGrpcUri;
        }
    }
    
    if (bHasPathSeparator)
    {
        const FString& RestOfAddress = MaybeGrpcUri.Mid(PathSeparatorIndex);
        if (!RestOfAddress.IsEmpty())
        {
            OutError = FString::Printf(TEXT("Path of the \"%s\" uri, must be empty. 实际上使用到的路径是: \"%s\""), *MaybeGrpcUri, *RestOfAddress);
            return false;
        }
    }
    
    if (FGrpcUriValidator_Internal::DoesHostLookLikeIp(GrpcHostName))
    {
        // 验证为IP地址
        if (!FGrpcUriValidator_Internal::ValidateIp(GrpcHostName, OutError))
            return false;
    }
    else
    {
        // 验证为域名
        if (!FGrpcUriValidator_Internal::ValidateDomainName(GrpcHostName, OutError))
            return false;
    }
    
    // 正常情况验证为端口
    if (!FGrpcUriValidator_Internal::ValidatePort(GrpcPort, OutError))
        return false;
    
    return true;
}
