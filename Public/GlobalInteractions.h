// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Public/NoteActor.h"
#include "GlobalInteractions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGlobalInteractions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLENDER_API IGlobalInteractions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void NoteHasBeenCollected(ANoteActor* note);
};
