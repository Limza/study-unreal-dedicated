#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "PortalManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBroadcastJoinGameSessionMessage, const FString&, SessionId);

UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHTTPRequestManager
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FBroadcastJoinGameSessionMessage BroadcastJoinGameSessionMessage;

	void JoinGameSession();
};
