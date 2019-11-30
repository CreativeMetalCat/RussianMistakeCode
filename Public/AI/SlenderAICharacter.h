// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UnrealNetwork.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "AIInterface.h"
#include  "Public/AI/MovementPoint.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SlenderAICharacter.generated.h"



UCLASS()
class /*SLENDER_API*/ ASlenderAICharacter : public ACharacter, public IAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlenderAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//--perception--begin


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<ACharacter>>EnemyClasses;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SightRange = 3000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimerHandle SenseUpdateTimerHandle;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateSight(UAIPerceptionComponent* Perception);

	void UpdateSight_Implementation(UAIPerceptionComponent* Perception);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateSense();

	void UpdateSense_Implementation() { }

	//--perception--end


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AActor* GetClosestTarget(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance);

	AActor* GetClosestTarget_Implementation(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance)override;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		FName GetBlackboardTargetValueName();

	FName GetBlackboardTargetValueName_Implementation()override { return BlackboardTargetValueName; }

	

	/**just gets the closest charcter*/
	UFUNCTION(BlueprintCallable)
		void UpdateTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ShouldStartHunt();

		void ShouldStartHunt_Implementation();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
		void TeleportAround();

	void TeleportAround_Implementation();

	UFUNCTION(BlueprintCallable)
		void AttackPlayer(ACharacter* player);

	

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
		void PlayFootstepSound();

	void PlayFootstepSound_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetMaxSenseDistance();

	float GetMaxSenseDistance_Implementation()override { return MaxSenseDistance; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetMinSenseDistance();

	float GetMinSenseDistance_Implementation()override { return 0.0f; }

	/*
	*For SlenderAi like Ai's
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool GetIsHunting();

	bool GetIsHunting_Implementation() { return IsHunting; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool CheckLocation();

	bool CheckLocation_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StopLookingAround();

	void StopLookingAround_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool GetIsDead();

	bool GetIsDead_Implementation() { return false; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartHunt();

	void StartHunt_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool GetIsLookingAround();
		bool GetIsLookingAround_Implementation(){return bIsLookingAround; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AActor* GetClosestTargetOnAllMap(TSubclassOf<AActor> targetClass);

	AActor* GetClosestTargetOnAllMap_Implementation(TSubclassOf<AActor> targetClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StopHunt();

	void StopHunt_Implementation();

	
	FTimerHandle HuntUpdateTimer;

	FTimerHandle TeleportUpdateTimer;

	FTimerHandle FootstepUpdateTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	FTimerHandle LookAroundTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EAITypeEnum AIType = EAITypeEnum::EAIT_Overwatch;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* HuntingMusic;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		FFMODEventInstance HuntingMusicInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* HuntingMusicEnd;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		FFMODEventInstance HuntingMusicEndInstance;

	/**Name of object to move to(player that tries to escape) in the black board*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		FName BlackboardTargetValueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Hunting, Replicated)
		float MaxSenseDistance = 1000.f;

	//Only for co-op
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		bool IsHunting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool bAllowedToMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		float MaxLookAtTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		float WasLookedAtFor = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Replicated)
		float DefaultMovementSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Replicated)
		float HuntingMovementSpeed = 600.f;

	/**Points that slender teleports to at random if not hunting*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		TArray<AMovementPoint*>TeleportPoints;


	/**Points that slender walks to if not hunting*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patroling, Replicated)
		TArray<AMovementPoint*>PatrolPoints;

	/**Points that slender walks to if not hunting*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patroling, Replicated)
		int PatrolNodeId = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		int GetCurrentPatrolId();

	int GetCurrentPatrolId_Implementation() { return PatrolNodeId; }


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetCurrentPatrolId(int id);

	void SetCurrentPatrolId_Implementation(int id) { PatrolNodeId = id; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		TArray<AMovementPoint*> GetPatrolPoints();

	TArray<AMovementPoint*> GetPatrolPoints_Implementation() { return PatrolPoints; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		EAITypeEnum GetAIType();

	EAITypeEnum GetAIType_Implementation() { return AIType; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		float TimeBetweenTeleports = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		float TimeSinceTeleport = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		float TimeSinceLastNoteCollection = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hunting, Replicated)
		float MaxTimeForCollectingNotes = 30.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* AttackSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound, Replicated)
		FFMODEventInstance AttackSoundInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* TeleportSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound, Replicated)
		FFMODEventInstance TeleportSoundInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
		UFMODEvent* FootstepSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound, Replicated)
		FFMODEventInstance FootstepSoundInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		bool bIsLookingAround = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
