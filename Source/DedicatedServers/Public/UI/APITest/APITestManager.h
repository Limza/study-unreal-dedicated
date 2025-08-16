#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "APITestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFleetsResponseReceived, const FDSListFleetsResponse&, ListFleetsResponse, bool, bWasSuccessful);

UCLASS()
class DEDICATEDSERVERS_API UAPITestManager : public UHTTPRequestManager
{
	GENERATED_BODY()
public:
	// FUNCTION()은 언리얼 엔진에서 C++ 함수에 메타데이터를 추가해주는 매크로입니다.
	// 이 매크로를 사용하면 해당 함수를 블루프린트에서 호출하거나, 리플렉션 시스템(예: 이벤트 바인딩, 네트워크 복제 등)에서 사용할 수 있게 등록합니다.
	// 즉, 버튼 클릭 같은 UI 이벤트에 함수가 연결될 수 있도록 언리얼 엔진에 알려주는 역할을 합니다.
	UFUNCTION()
	void ListFleets();

	// ListFleets API 응답을 외부에서 구독할 수 있도록 하는 델리게이트
	// ListFleets_Response 함수에서 브로드캐스트됨
	UPROPERTY()
	FOnListFleetsResponseReceived OnListFleetsResponseReceived;
	
	// http 응담이 오면 호출될 콜백 함수
	void ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};