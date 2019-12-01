// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLENDER_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnInteraction(AActor*interactor,UPrimitiveComponent*interactedComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool RequestPlayfootstepSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool HasKeyId(int Id);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool AddKeyId(int Id,FName name);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		TArray<int> GetAllKeyIds();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TakeAwayKey(int keyId);
};
