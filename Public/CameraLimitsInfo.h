// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CameraLimitsInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCameraLimitsInfo
{
	GENERATED_BODY();
public:


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSupposedToLimit = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bLimitRoll = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bLimitPitch = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bLimitYaw = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxRoll = 180;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MinRoll = -180;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxPitch = -10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MinPitch = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxYaw = -180;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MinYaw = 0;
};