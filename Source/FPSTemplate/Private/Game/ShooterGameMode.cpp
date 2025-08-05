#include "Game/ShooterGameMode.h"

// LogShooterGameMode라는 새로운 로그 카테고리를 정의합니다.
// 이 카테고리를 사용해 게임 모드 관련 로그 메시지를 구분하여 출력할 수 있습니다.
DEFINE_LOG_CATEGORY(LogShooterGameMode)

// 아래 WITH_GAMELIFT define은 빌드 타겟이 Server일 때 활성화됩니다.
#if WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#include "GameLiftServerSDKModels.h"
#endif

AShooterGameMode::AShooterGameMode() :
	ProcessParameters(nullptr)
{
	UE_LOG(LogShooterGameMode, Log, TEXT("Initializing AGameLiftUnrealAppGameMode..."));
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void AShooterGameMode::InitGameLift()
{
#if WITH_GAMELIFT
	UE_LOG(LogShooterGameMode, Log, TEXT("Calling InitGameLift..."));

	// 먼저 GameLiftServerSDK 모듈을 로드합니다.
	FGameLiftServerSDKModule* GameLiftSdkModule = 
        &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	// GameLift Anywhere 플릿을 위한 서버 파라미터를 정의합니다.
	// 이 파라미터들은 GameLift에서 관리하는 EC2 플릿에는 필요하지 않습니다.
	FServerParameters ServerParametersForAnywhere;
    SetServerParameters(ServerParametersForAnywhere);

    UE_LOG(LogShooterGameMode, Log, TEXT("Initializing the GameLift Server..."));

    // InitSDK는 GameLift의 agent와 로컬 연결을 설정하여 추가 통신이 가능하게 합니다.
	if (FGameLiftGenericOutcome InitSdkOutcome = GameLiftSdkModule->InitSDK(ServerParametersForAnywhere); 
        InitSdkOutcome.IsSuccess())
    {
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_GREEN);
        UE_LOG(LogShooterGameMode, Log, TEXT("GameLift InitSDK succeeded!"));
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_NONE);
    }
    else
    {
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_RED);
        UE_LOG(LogShooterGameMode, Log, TEXT("ERROR: InitSDK failed : ("));
        FGameLiftError GameLiftError = InitSdkOutcome.GetError();
        UE_LOG(LogShooterGameMode, Log, TEXT("ERROR: %s"), *GameLiftError.m_errorMessage);
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_NONE);
        return;
    }

    ProcessParameters = MakeShared<FProcessParameters>();

    // 게임 세션이 생성되면 GameLift 서버가 게임 서버에 활성화 요청을 보내고,
    // 게임 속성 및 기타 설정이 포함된 게임 세션 객체를 전달합니다.
    // 이 시점에서 게임 서버는 게임 세션 객체에 따라 필요한 처리를 해야 합니다.
    // 게임 서버가 플레이어의 접속을 받을 준비가 되면 GameLiftServerAPI.ActivateGameSession()을 호출해야 합니다.
    ProcessParameters->OnStartGameSession.BindLambda([=](const Aws::GameLift::Server::Model::GameSession& InGameSession)
    {
        const FString GameSessionId = FString(InGameSession.GetGameSessionId());
        UE_LOG(LogShooterGameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
        GameLiftSdkModule->ActivateGameSession();
    });

    // OnProcessTerminate callback.
    // GameLift 서버는 이 게임 서버를 호스팅하는 인스턴스를 종료하기 전에 이 callback을 호출합니다.
    // 이때 게임 서버는 상태 저장, 서비스와의 통신 등 종료 전에 필요한 작업을 할 수 있습니다.
    // 여기서는 GameLift 서버에 종료할 것임을 알립니다.
    ProcessParameters->OnTerminate.BindLambda([=]()
    {
        UE_LOG(LogShooterGameMode, Log, TEXT("Game Server Process is terminating"));
        GameLiftSdkModule->ProcessEnding();
    });

    // HealthCheck callback입니다.
    // GameLift 서버는 약 60초마다 이 callback을 호출합니다.
    // 이곳에서 게임 서버는 의존성 등의 정상 동작 여부를 점검할 수 있습니다.
    // 정상일 경우 true, 그렇지 않으면 false를 반환하면 됩니다.
    // 게임 서버는 60초 이내에 응답해야 하며, 응답하지 않으면 GameLift 서버는 기본적으로 'false'로 간주합니다.
    // 여기서는 항상 정상(true)로 반환합니다.
    ProcessParameters->OnHealthCheck.BindLambda([]()
    {
        UE_LOG(LogShooterGameMode, Log, TEXT("Performing Health Check"));
        return true;
    });

    // 이 코드는 커맨드라인 인자 중 -port=숫자 형식이 있으면 해당 숫자를
    // 서버 포트로 사용하고, 없으면 기본 포트를 사용하도록 합니다.
    // 서버 실행 시 포트 변경이 필요할 때 유용합니다.
    // GameServer.exe -port=7777 LOG=server.mylog
    int32 Port = FURL::UrlConfig.DefaultPort;
    ParseCommandLinePort(Port);
    ProcessParameters->port = Port;

    // 여기서 게임 서버는 GameLift 서버에 게임 세션 로그 파일의 위치를 알려줍니다.
    // 게임 세션이 끝나면 GameLift 서버가 지정된 위치의 모든 파일을 업로드하여
    // 클라우드에 저장하고 나중에 접근할 수 있게 합니다.
    TArray<FString> Logfiles;
    // Gamelift에 저장된 로그 경로
    Logfiles.Add(TEXT("FPSTemplate/Saved/Logs/FPSTemplate.log"));
    ProcessParameters->logParameters = Logfiles;

    // 게임 서버는 ProcessReady()를 호출하여 GameLift 서버에 게임 세션을 호스팅할 준비가 되었음을 알립니다.
    UE_LOG(LogShooterGameMode, Log, TEXT("Calling Process Ready..."));

	if (FGameLiftGenericOutcome ProcessReadyOutcome = GameLiftSdkModule->ProcessReady(*ProcessParameters);
        ProcessReadyOutcome.IsSuccess())
    {
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_GREEN);
        UE_LOG(LogShooterGameMode, Log, TEXT("Process Ready!"));
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_NONE);
    }
    else
    {
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_RED);
        UE_LOG(LogShooterGameMode, Log, TEXT("ERROR: Process Ready Failed!"));
        FGameLiftError ProcessReadyError = ProcessReadyOutcome.GetError();
        UE_LOG(LogShooterGameMode, Log, TEXT("ERROR: %s"), *ProcessReadyError.m_errorMessage);
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_NONE);
    }

    UE_LOG(LogShooterGameMode, Log, TEXT("InitGameLift completed!"));
#endif
}

void AShooterGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
    bool bIsAnywhereActive = false;
    if (FParse::Param(FCommandLine::Get(), TEXT("glAnywhere")))
    {
        bIsAnywhereActive = true;
    }

    if (bIsAnywhereActive)
    {
        UE_LOG(LogShooterGameMode, Log, TEXT("Configuring server parameters for Anywhere..."));

        // GameLift Anywhere가 활성화된 경우, 커맨드라인 인자를 파싱하여 ServerParameters 객체에 전달합니다.
        FString glAnywhereWebSocketUrl = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereWebSocketUrl="), glAnywhereWebSocketUrl))
        {
            OutServerParameters.m_webSocketUrl = TCHAR_TO_UTF8(*glAnywhereWebSocketUrl);
        }

        FString glAnywhereFleetId = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereFleetId="), glAnywhereFleetId))
        {
            OutServerParameters.m_fleetId = TCHAR_TO_UTF8(*glAnywhereFleetId);
        }

        FString glAnywhereProcessId = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereProcessId="), glAnywhereProcessId))
        {
            OutServerParameters.m_processId = TCHAR_TO_UTF8(*glAnywhereProcessId);
        }
        else
        {
            // ProcessId가 커맨드라인 인자로 전달되지 않은 경우, 임의의 고유 문자열을 생성합니다.
            FString TimeString = FString::FromInt(std::time(nullptr));
            FString ProcessId = "ProcessId_" + TimeString;
            OutServerParameters.m_processId = TCHAR_TO_UTF8(*ProcessId);
        }

        FString glAnywhereHostId = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereHostId="), glAnywhereHostId))
        {
            OutServerParameters.m_hostId = TCHAR_TO_UTF8(*glAnywhereHostId);
        }

        FString glAnywhereAuthToken = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereAuthToken="), glAnywhereAuthToken))
        {
            OutServerParameters.m_authToken = TCHAR_TO_UTF8(*glAnywhereAuthToken);
        }

        FString glAnywhereAwsRegion = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereAwsRegion="), glAnywhereAwsRegion))
        {
            OutServerParameters.m_awsRegion = TCHAR_TO_UTF8(*glAnywhereAwsRegion);
        }

        FString glAnywhereAccessKey = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereAccessKey="), glAnywhereAccessKey))
        {
            OutServerParameters.m_accessKey = TCHAR_TO_UTF8(*glAnywhereAccessKey);
        }

        FString glAnywhereSecretKey = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereSecretKey="), glAnywhereSecretKey))
        {
            OutServerParameters.m_secretKey = TCHAR_TO_UTF8(*glAnywhereSecretKey);
        }

        FString glAnywhereSessionToken = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereSessionToken="), glAnywhereSessionToken))
        {
            OutServerParameters.m_sessionToken = TCHAR_TO_UTF8(*glAnywhereSessionToken);
        }

        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_YELLOW);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> WebSocket URL: %s"), *OutServerParameters.m_webSocketUrl);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Fleet ID: %s"), *OutServerParameters.m_fleetId);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Process ID: %s"), *OutServerParameters.m_processId);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Host ID (Compute Name): %s"), *OutServerParameters.m_hostId);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Auth Token: %s"), *OutServerParameters.m_authToken);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Aws Region: %s"), *OutServerParameters.m_awsRegion);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Access Key: %s"), *OutServerParameters.m_accessKey);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Secret Key: %s"), *OutServerParameters.m_secretKey);
        UE_LOG(LogShooterGameMode, Log, TEXT(">>>> Session Token: %s"), *OutServerParameters.m_sessionToken);
        UE_LOG(LogShooterGameMode, SetColor, TEXT("%s"), COLOR_NONE);
    }
}

void AShooterGameMode::ParseCommandLinePort(int32& OutPort)
{
    TArray<FString> CommandLineTokens;
    TArray<FString> CommandLineSwitches;
    FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
    for (FString SwitchStr : CommandLineSwitches)
    {
        FString Key;
        FString Value;
        if (SwitchStr.Split("=", &Key, &Value))
        {
            if (Key.Equals("port", ESearchCase::IgnoreCase))
            {
                OutPort = FCString::Atoi(*Value);
                return;
            }
        }
    }
}