// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLENDER_API IAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AActor* GetClosestTarget(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AActor* GetClosestTargetOnAllMap(TSubclassOf<AActor> targetClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		FName GetBlackboardTargetValueName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableControl(bool disableMovement, bool disableRotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool GetIsDead();

	/*
	*For SlenderAi like Ai's
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool GetIsHunting();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StopHunt();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool BeKilled();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetMaxSenseDistance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetMinSenseDistance();
};
