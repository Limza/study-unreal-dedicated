// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameLiftServerSDKModels.h"
#include "DS_GameMode.generated.h"

struct FProcessParameters; // GameLift 사용

// LogDS_GameMode 게임 모드 관련 로그 출력을 위한 카테고리입니다.
// 이 선언을 통해 소스 파일에서 LogDS_GameMode(다른 이름도 가능)를
// 사용하여 로그를 구분할 수 있다.
// 로그 레벨 및 필터링을 통해 디버깅과 모니터링에 활용.
DECLARE_LOG_CATEGORY_EXTERN(LogDS_GameMode, Log, All);

UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void InitGameLift();

	static void SetServerParameters(FServerParameters& OutServerParameters);
	static void ParseCommandLinePort(int32& OutPort);

private:
	TSharedPtr<FProcessParameters> ProcessParameters;
};
