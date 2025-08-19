// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "UI/Portal/PortalManager.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(PortalManagerClass)
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	check(JoinGameWidget);
	check(JoinGameWidget->Button_JoinGame);
	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked);
}

void USignInOverlay::OnJoinGameButtonClicked()
{
	check(PortalManager);
	PortalManager->BroadcastJoinGameSessionMessage.AddDynamic(this, &USignInOverlay::UpdateJoinGameStatusMessage);
	PortalManager->JoinGameSession();
	
	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
}

void USignInOverlay::UpdateJoinGameStatusMessage(const FString& Message)
{
	JoinGameWidget->SetStatusMessage(Message);
	
	PortalManager->BroadcastJoinGameSessionMessage.RemoveDynamic(this, &USignInOverlay::UpdateJoinGameStatusMessage);
}
