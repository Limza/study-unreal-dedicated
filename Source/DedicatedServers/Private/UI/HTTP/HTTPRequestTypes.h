#pragma once

// 언리얼의 리플렉션 시스템을 사용하기 위해 필요한 헤더 파일을 포함합니다.
#include "HTTPRequestTypes.generated.h"

namespace  HTTPStatusMessages
{
	/*
	* DEDICATEDSERVERS_API는 언리얼 엔진에서 DLL(동적 라이브러리)로 내보낼 심볼(함수, 변수 등)에 붙이는 매크로입니다.
	* 이 매크로는 빌드 환경에 따라 __declspec(dllexport) 또는 __declspec(dllimport)로 정의되어, 해당 심볼을 외부 모듈에서 사용할 수 있게 해줍니다.
	*
	* extern DEDICATEDSERVERS_API const FString SomethingWentWrong;는
	* SomethingWentWrong라는 전역 상수를 DLL로 내보내서, 다른 모듈이나 프로젝트에서도 접근할 수 있도록 선언한 것입니다.
	* 주로 공통 에러 메시지나 상태 값을 여러 곳에서 공유할 때 사용합니다.
	 */
	extern DEDICATEDSERVERS_API const FString SomethingWentWrong;
}

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

// aws gamelift ListFleets API의 응답을 저장하는 구조체입니다.
// https://docs.aws.amazon.com/ko_kr/gameliftservers/latest/apireference/API_ListFleets.html#API_ListFleets_ResponseSyntax
USTRUCT()
struct FDSListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds{};

	UPROPERTY()
	FString NextToken {};

	void Dump() const;
};

// aws gamelift FindOrCreateGameSession API의 응답을 저장하는 구조체입니다.
// https://docs.aws.amazon.com/ko_kr/gameliftservers/latest/apireference/API_CreateGameSession.html#API_CreateGameSession_ResponseSyntax
USTRUCT()
struct FDSGameSession
{
	GENERATED_BODY()

	UPROPERTY()
	double CreationTime{};

	UPROPERTY()
	FString CreatorId{};

	UPROPERTY()
	int32 CurrentPlayerSessionCount{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	TMap<FString, FString> GameProperties{};

	UPROPERTY()
	FString GameSessionData{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString Location{};

	UPROPERTY()
	FString MatchmakerData{};
	
	UPROPERTY()
	int32 MaximumPlayerSessionCount{};
	
	UPROPERTY()
	FString Name{};
	
	UPROPERTY()
	FString PlayerSessionCreationPolicy{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString StatusReason{};

	UPROPERTY()
	double TerminationTime{};

	void Dump() const;
};