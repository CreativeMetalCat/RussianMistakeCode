// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealNetwork.h"
#include "AI/AIInterface.h"
#include "Components/TextRenderComponent.h"
#include "Public/Multiplayer/MultiplayerInterface.h"
#include "Public/GameSave.h"
#include "GameFramework/SpringArmComponent.h"
#include "Public/PlayerInteractions.h"
#include "Public/CameraLimitsInfo.h"
#include "SlenderCharacter.generated.h"




class UInputComponent;

UCLASS(config=Game)
class ASlenderCharacter : public ACharacter,public IAIInterface, public  IMultiplayerInterface,public IPlayerInteractions
{
	GENERATED_BODY()





public:
	ASlenderCharacter();

	///** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Character)
	//	class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Character)
		USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Multiplayer,Replicated)
		class UTextRenderComponent* NameDisplay;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

protected:
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
		void StartCrouching();

	UFUNCTION(BlueprintCallable)
		void StopCrouching();

	UFUNCTION(BlueprintCallable)
		void ToggleFlashlight() { bFlashLightOn ? TurnOffFlashlight() : TurnOnFlashlight(); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TurnOnFlashlight();

	void TurnOnFlashlight_Implementation() { if (CanUseFlashlight()) { bFlashLightOn = true; } }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool CanUseFlashlight();

	bool CanUseFlashlight_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetAllowedControl(bool allowCamera = true, bool allowMovement = true);

	void SetAllowedControl_Implementation(bool allowCamera = true, bool allowMovement = true);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CheckFlashlight();

	void CheckFlashlight_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TurnOffFlashlight();

	void TurnOffFlashlight_Implementation() { bFlashLightOn = false; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ChangeNameDisplayOfPlayer(const FString& newName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AllNotesCollected();

	void AllNotesCollected_Implementation() {}

	virtual void ChangeNameDisplayOfPlayer_Implementation(const FString& newName)override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AActor* GetClosestTarget(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance);

	AActor* GetClosestTarget_Implementation(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool PlayFootstepSound();

	//Player Interactions -- BEGIN
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsHidding();

	bool IsHidding_Implementation() { return bHidding; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetIsHidding(bool hidding);

	void SetIsHidding_Implementation(bool hidding) { bHidding = hidding; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ForceCrouch();

	void ForceCrouch_Implementation() { bIsCrouched = true; };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ForceUnCrouch();

	void ForceUnCrouch_Implementation(){ bIsCrouched = false; }


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetCameraLimitations(FCameraLimitsInfo info);

	void SetCameraLimitations_Implementation(FCameraLimitsInfo info) { CameraLimitsInfo = info; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		FCameraLimitsInfo GetCameraLimitations();

	FCameraLimitsInfo GetCameraLimitations_Implementation() { return  CameraLimitsInfo; }

	//Player Interactions -- END

	bool PlayFootstepSound_Implementation() { return false; }

	UFUNCTION(BlueprintCallable)
		EPhysicalSurface GetFootstepSurfaceType();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
		bool GetIsDead();

	bool GetIsDead_Implementation() { return Dead; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool BeKilled();

	bool BeKilled_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		bool CanRun();

	bool CanRun_Implementation();

	/* * increases amount of notes player has and returs it*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int IncreaseAmountOfNotes(int amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void FillSaveData(UGameSave*saveObject);

	void FillSaveData_Implementation(UGameSave* saveObject);

	UFUNCTION(BlueprintPure)
		int GetAmountOfNotes() { return AmountOfNotes; }

	UFUNCTION(BlueprintPure)
		int GetNeededAmountOfNotes() { return NeededAmountOfNotes; }

	int IncreaseAmountOfNotes_Implementation(int amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DisableControl(bool disableMovement, bool disableRotation);

	void DisableControl_Implementation(bool disableMovement, bool disableRotation);

	/* *Mostly based on distance of enemy to player since any other way there would be no threat */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		float GetDangerLevel();

	float GetDangerLevel_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool HasKeyId(int Id);

	bool HasKeyId_Implementation(int Id);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool AddKeyId(int Id, FName name);

	bool AddKeyId_Implementation(int Id, FName name);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		TArray<int> GetAllKeyIds();

	TArray<int> GetAllKeyIds_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TakeAwayKey(int keyId);

	void TakeAwayKey_Implementation(int keyId);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flashlight, Replicated)
	FTimerHandle FlashLightUpdateTimer;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hidding, Replicated)
		FCameraLimitsInfo CameraLimitsInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flashlight, Replicated)
		bool bAllowedToUseFlashLight = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flashlight, Replicated)
		bool bFlashLightOn = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flashlight, Replicated)
		float MaxFlashlightTime = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flashlight, Replicated)
		bool bFlashlightAutoRecharges = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flashlight, Replicated)
		float UsedFlashlightTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Walking, Replicated)
		FTimerHandle FootstepTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxEnemyAwarnessDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		float DistanceToNearestEnemy = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep, Replicated)
		float TimeBetweenSteps = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep, Replicated)
		float TimeSinceLastStep = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep)
		bool bAutoFootstep = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool Dead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool bHidding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool CanRotate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool CanBeAttacked = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float MaxRunningTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float RunningTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundBase* StartRunningSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Sound)
		USoundBase* OutOfBreathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int, FName> Keys;

	/**How much did player already collected
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender, Replicated)
		int AmountOfNotes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Multiplayer, Replicated)
		bool bChagedName = false;
	/*
	*Assigned when game starts
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slender, Replicated)
		int NeededAmountOfNotes = 0;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASlenderProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint32 bUsingMotionControllers : 1;

protected:



	void OnStartRunning();

	void OnStopRunning();

	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:

	UFUNCTION(BlueprintCallable)
		void Use();

	UFUNCTION(BlueprintPure)
		bool HasAllNotes() { return this->AmountOfNotes == this->NeededAmountOfNotes; }
	/** Returns Mesh1P subobject **/
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

