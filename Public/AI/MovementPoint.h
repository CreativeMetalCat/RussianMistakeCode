// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Components/BoxComponent.h"
#include "MovementPoint.generated.h"

/**
 * Used to expand on concept of target points for slender movement and allow for multiple of them to use it
 */
UCLASS()
class SLENDER_API AMovementPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	AMovementPoint();

	/**Used to check if anyone of specific class is close enough for it to be considered as CurrentlyUsed*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
		UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FVector SizeOfPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bEnabled = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chance)
		bool bUseChance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chance)
		int MaxChanceToBeUsed = 100;

	UFUNCTION(BlueprintCallable)
		void SetIsEnabled(bool enabled) { bEnabled = enabled; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Chance)
		bool GenerateChanceToBeUsed();

	bool GenerateChanceToBeUsed_Implementation() { return FMath::RandBool(); }

	UFUNCTION(BlueprintPure, Category = Movement)
		TArray<AActor*> GetAllActorsNearBy(TSubclassOf<AActor> classType);

	/**Returns true if none of classType is close ,false otherwise*/
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = Movement)
		/**Returns true if none of classType is close ,false otherwise*/
		bool CanBeUsed(TSubclassOf<AActor> classType);
	/**Returns true if none of classType is close ,false otherwise*/
	bool CanBeUsed_Implementation(TSubclassOf<AActor> classType);

	/**Returns true if none of classTypes is close ,false otherwise*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Movement)
		/**Returns true if none of classType is close ,false otherwise*/
		bool CanBeUsedMany(const TArray<TSubclassOf<AActor> >& classTypes,bool FailOnlyIfAll);
	/**Returns true if none of classType is close ,false otherwise*/
	bool CanBeUsedMany_Implementation(const TArray<TSubclassOf<AActor> >& classTypes, bool FailOnlyIfAll);
};
