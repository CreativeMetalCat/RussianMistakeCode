// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "MovingPlatform.generated.h"

UENUM(BlueprintType)
enum class EAxisEnum :uint8
{
	AE_X UMETA(DisplayName = "X"),
	AE_Y UMETA(DisplayName = "Y"),
	AE_Z UMETA(DisplayName = "Z")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartMovementDelegate,bool,Opened);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopMovementDelegate, bool, Opened);

/**
 * 
 */
UCLASS()
class SLENDER_API AMovingPlatform : public AInteractiveActor
{
	GENERATED_BODY()

protected:

public:
	AMovingPlatform();

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FStartMovementDelegate OnStartMovementDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FStopMovementDelegate OnStopMovementDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PlatformMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		FVector OriginalLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		EAxisEnum AxisToMoveAlong;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		float StartDelay = 0.0f;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		float TimeToOpen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		bool bOpened;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		bool bMoving;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		float OpenDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		float MovedDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement, Replicated)
		FTimerHandle UpdateTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement, Replicated)
		FTimerHandle PreStartMovementTimerHandle;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundLegacy)
		USoundBase* MovementSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundLegacy)
		USoundBase* StopSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundLegacy)
		USoundBase* LockedSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundLegacy)
		USoundBase* StartMovementSound;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* MovementSoundEvent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
	FFMODEventInstance MovementSoundEventInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* StartMovementEvent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* LockedSoundEvent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* StopSoundEvent;




	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		USoundAttenuation* Attenuation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UAudioComponent* MovementSoundLoop;

public:

	virtual void OnConstruction(const FTransform& Transform) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StartMovement(bool StartEvenIfCanNotBeUsed);

	void StartMovement_Implementation(bool StartEvenIfCanNotBeUsed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void PreStartMovement();

	void PreStartMovement_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void UpdateLocation();

	void UpdateLocation_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StopMovement();

	void StopMovement_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnInteraction(AActor* interactor);

	virtual void OnInteraction_Implementation(AActor* interactor) override;

	virtual void BeginPlay()override;
};
