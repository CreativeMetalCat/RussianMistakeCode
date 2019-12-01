// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "DoorKeyInfo.generated.h"

/**
 * IT DOES NOT STORES KEY ID
 */
USTRUCT(BlueprintType)
struct FDoorKeyInfo
{
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Description;
};
