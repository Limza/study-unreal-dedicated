#pragma once

// 언리얼의 리플렉션 시스템을 사용하기 위해 필요한 헤더 파일을 포함합니다.
#include "HTTPRequestTypes.generated.h"

// USTRUCT() 매크로는 언리얼 엔진의 리플렉션 시스템에 구조체를 등록하기 위해 사용됩니다.
USTRUCT()
// HTTP 응답에 있는 Metadata를 저장하는 구조체입니다.
struct FDSMetaData
{
	// GENERATED_BODY() 매크로는 언리얼 엔진이 구조체에 필요한 코드와 메타데이터를 자동으로 생성하도록 합니다.
	GENERATED_BODY()

	UPROPERTY()
	// C++11 이상에서는 {}로 멤버 변수를 초기화하면 int32는 0, 포인터는 nullptr 등 타입에 맞는 기본값으로 초기화됩니다.
	int32 httpStatusCode {};

	UPROPERTY()
	FString requestId {};

	UPROPERTY()
	int32 attempts {};

	UPROPERTY()
	double totalRetryDelay {};

	void Dump() const;
};

USTRUCT()
// aws gamelift ListFleets API의 응답을 저장하는 구조체입니다.
// https://docs.aws.amazon.com/ko_kr/gameliftservers/latest/apireference/API_ListFleets.html#API_ListFleets_ResponseSyntax
struct FDSListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds{};

	UPROPERTY()
	FString NextToken {};

	void Dump() const;
};
