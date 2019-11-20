// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SlenderGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API USlenderGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Map)
		bool bAllowCustscene = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender)
		TArray<FVector> SlendersLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender)
		bool bHunting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender)
		bool bAllowedToMove;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetSavedValues(bool AllowCustscene, const TArray<FVector>& SlendersPositions, bool Hunting, bool AllowedToMove);

	void SetSavedValues_Implementation(bool AllowCustscene,  const TArray<FVector>& SlendersPositions, bool Hunting, bool AllowedToMove);
};
