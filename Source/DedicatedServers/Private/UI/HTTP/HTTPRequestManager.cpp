#include "UI/HTTP/HTTPRequestManager.h"

#include "HTTPRequestTypes.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"


bool UHTTPRequestManager::ContainsErrors(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("Unknown Error");
		FString ErrorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("Unknown Error");

		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Message: %s"), *ErrorMessage);
		
		LogJsonObjectError(JsonObject);
		
		return true;
	}
	if (JsonObject->HasField(TEXT("$fault")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("Unknown Error");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);

		LogJsonObjectError(JsonObject);
		
		return true;
	}

	return false;
}

void UHTTPRequestManager::LogJsonObjectError(TSharedPtr<FJsonObject> JsonObject)
{
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	UE_LOG(LogDedicatedServers, Error, TEXT("JsonObject: %s"), *JsonString);
}

void UHTTPRequestManager::DumpMetaData(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("$metadata")))
	{
		const TSharedPtr<FJsonObject> MetaDataJsonObject = JsonObject->GetObjectField(TEXT("$metadata"));
		FDSMetaData DSMetaData;
		FJsonObjectConverter::JsonObjectToUStruct(MetaDataJsonObject.ToSharedRef(), &DSMetaData);
		DSMetaData.Dump();
	}
}

FString UHTTPRequestManager::SerializeJsonContent(const TMap<FString, FString>& Params)
{
	TSharedPtr<FJsonObject> ContentJsonObject = MakeShareable(new FJsonObject);

	for (const auto& Param: Params)
	{
		ContentJsonObject->SetStringField(Param.Key, Param.Value);
	}

	FString Content;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(ContentJsonObject.ToSharedRef(), JsonWriter);

	return Content;
}
