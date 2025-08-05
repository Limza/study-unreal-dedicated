

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDKModels.h"
#include "ShooterGameModeBase.h"
#include "ShooterGameMode.generated.h"

struct FProcessParameters; // GameLift 사용

// LogShooterGameMode는 게임 모드 관련 로그 출력을 위한 카테고리입니다.
// 이 선언을 통해 소스 파일에서 LogShooterGameMode(다른 이름도 가능)를
// 사용하여 로그를 구분할 수 있다.
// 로그 레벨 및 필터링을 통해 디버깅과 모니터링에 활용.
DECLARE_LOG_CATEGORY_EXTERN(LogShooterGameMode, Log, All);

// minimalapi는 이 클래스의 필수 API만 외부 모듈에 노출하여 빌드 속도와 캡슐화를 향상.
// UCLASS(minimalapi)를 사용하면 외부에서 접근 가능한 인터페이스가 최소화.
// 즉, 외부 모듈에서는 이 클래스의 포인터(예: AShooterGameMode*)로만 접근할 수 있고,
// 멤버 함수나 변수 등은 별도로 UPROPERTY, UFUNCTION 등으로 명시적으로 노출하지 않는 한 접근할 수 없다.
// 완전히 숨기는 것이 아니라, 포인터 등 최소한의 인터페이스만 노출.
UCLASS(minimalapi)
class AShooterGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()
public:
	AShooterGameMode();

protected:
	virtual void BeginPlay() override;

private:
	void InitGameLift();

	static void SetServerParameters(FServerParameters& OutServerParameters);
	static void ParseCommandLinePort(int32& OutPort);

private:
	TSharedPtr<FProcessParameters> ProcessParameters;
};
