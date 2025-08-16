#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/DedicatedServersTag.h"
#include "APIData.generated.h"

UCLASS()
class DEDICATEDSERVERS_API UAPIData : public UDataAsset
{
	GENERATED_BODY()
public:
	FString GetAPIEndpoint(const FGameplayTag& APIEndpoint);

protected:
	// Name of this API - for labeling in  the Data Asset; this is not used by any code.
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	// aws api gateway invoke url
	UPROPERTY(EditDefaultsOnly)
	FString InvokeURL;

	// aws api gateway stage
	UPROPERTY(EditDefaultsOnly)
	FString Stage;
	
	// game play tag 에 aws api 리소스를 등록한다
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FString> Resources;
};
