// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UnrealNetwork.h"
#include "AIModule.h"
#include "AIModuleClasses.h"
#include "AIInterface.h"
#include "SlenderAIController.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API ASlenderAIController : public AAIController, public IAIInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
		UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintPure)
		UBlackboardData* GetBlackboardData() { return BehaviorTree->BlackboardAsset; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AActor* GetClosestTarget(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance);

	AActor* GetClosestTarget_Implementation(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance)override;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		FName GetBlackboardTargetValueName();

	FName GetBlackboardTargetValueName_Implementation()override { return BlackboardTargetValueName; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		 void InitAI();

	void InitAI_Implementation() {}

	/**Class of player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TSubclassOf<AActor> TargetClass;

	/**Name of object to move to(player that tries to escape) in the black board*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		FName BlackboardTargetValueName;
	
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;
};
