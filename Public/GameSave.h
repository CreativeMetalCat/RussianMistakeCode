// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSave.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API UGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Map)
		FString MapName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Map)
		bool bAllowCustscene = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender)
		TArray<FVector> SlendersLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender)
		bool bHunting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender)
		bool bAllowedToMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerInfo)
		int AmountOfCollectedNotes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerInfo)
		int AmountOfNotesTotal = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveInfo)
		FDateTime TimeOfSaving;


	UGameSave();
};
