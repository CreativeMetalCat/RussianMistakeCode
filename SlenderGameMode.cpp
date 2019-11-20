// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SlenderGameMode.h"
#include "SlenderHUD.h"
#include "SlenderCharacter.h"
#include "Public/Multiplayer/MultiplayerInterface.h"
#include "GameFramework/PlayerState.h"
#include "Engine.h"
#include "UObject/ConstructorHelpers.h"

ASlenderGameMode::ASlenderGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASlenderHUD::StaticClass();

	/*static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateFinder(TEXT("/Game/Blueprints/BP_SlenderGameState"));

	GameStateClass = GameStateFinder.Class;*/
}

