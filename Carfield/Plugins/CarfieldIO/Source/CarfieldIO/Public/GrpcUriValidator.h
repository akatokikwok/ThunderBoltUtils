#pragma once

#include "CoreMinimal.h"

class FGrpcUriValidator
{
public:
    /**
     * 验证URL字符串,并将其进一步提供给GRPC::CreateChannel方法;此方法仅用于检查格式
     *
     * @param MaybeGrpcUri Grpc URI to validate.
     * @param OutError Error message if any.
     *
     * @return True if the URI is valid and thus could be used, false otherwise.
     */
    static bool Validate(const FString& MaybeGrpcUri, FString& OutError);
};
