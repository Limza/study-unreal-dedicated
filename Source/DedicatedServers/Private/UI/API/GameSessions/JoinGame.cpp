// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/API/GameSessions/JoinGame.h"

#include "Components/TextBlock.h"

void UJoinGame::SetStatusMessage(const FString& Message) const
{
	check(TextBlock_StatusMessage);
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
}
