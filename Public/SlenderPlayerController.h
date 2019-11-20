// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Public/Multiplayer/MultiplayerInterface.h"
#include "SlenderPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API ASlenderPlayerController : public APlayerController,public  IMultiplayerInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	 void ChangeNameDisplayOfPlayer(const FString& newName);

	virtual void ChangeNameDisplayOfPlayer_Implementation(const FString& newName)override;
};
