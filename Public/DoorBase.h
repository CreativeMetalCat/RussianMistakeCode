// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "DoorBase.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API ADoorBase : public AInteractiveActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int RequiredKeyId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int>RequiredKeyIds;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bCanBeUsedWithMultipleKeys = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bTakesAwayKey = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bNeedsKey = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bOpened = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* OpenSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		FFMODEventInstance OpenSoundInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* CloseSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		FFMODEventInstance CloseSoundInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* LockSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		FFMODEventInstance LockSoundInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* LockedSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		FFMODEventInstance LockedSoundInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* UnLockSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		FFMODEventInstance UnLockSoundInstance;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnInteraction(AActor* interactor, UPrimitiveComponent* interactedComponent);

	void OnInteraction_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent);

	UFUNCTION(BlueprintPure)
		bool IsLocked() { return bNeedsKey; }

	UFUNCTION(BlueprintPure)
		bool IsOpened() { return bOpened; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Open();

	void Open_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Close();

	void Close_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UnLock();

	void UnLock_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Lock();

	void Lock_Implementation();
};
