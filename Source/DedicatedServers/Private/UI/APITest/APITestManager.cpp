#include "UI/APITest/APITestManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestManager::ListFleets()
{
	check(APIData);
	const TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UAPITestManager::ListFleets_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTag::GameSessionsApi::ListFleets);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest();
}

void UAPITestManager::ListFleets_Response(FHttpRequestPtr Request,FHttpResponsePtr Response, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("List Fleets Response Received"));

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) == false)
		return;

	if (ContainsErrors(JsonObject))
	{
		OnListFleetsResponseReceived.Broadcast(FDSListFleetsResponse(), false);
		return;	
	}
	DumpMetaData(JsonObject);

	// $metadata 필드 외의 나머지 전체 JSON을 FDSListFleetsResponse 구조체로 변환
	FDSListFleetsResponse ListFleetsResponse;
	FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &ListFleetsResponse);
	
	// 변환된 결과를 디버깅용으로 출력
	ListFleetsResponse.Dump();

	OnListFleetsResponseReceived.Broadcast(ListFleetsResponse, true);
}