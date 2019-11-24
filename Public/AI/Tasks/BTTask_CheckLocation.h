// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckLocation.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API UBTTask_CheckLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FBlackboardKeySelector LocationKey;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override {}

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

};
