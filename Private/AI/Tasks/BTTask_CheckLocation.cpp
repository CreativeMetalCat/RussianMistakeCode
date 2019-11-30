// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckLocation.h"
#include "AIController.h"
#include "Engine.h"
#include "AI/AIInterface.h"
#include "SlenderCharacter.h"
#include "Public/AI/SlenderAICharacter.h"
#include "Engine.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CheckLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner() != nullptr)
	{
		if (OwnerComp.GetAIOwner()->GetPawn() != nullptr)
		{

			
			IAIInterface::Execute_CheckLocation(OwnerComp.GetAIOwner()->GetPawn());
			OwnerComp.GetBlackboardComponent()->ClearValue(LocationKey.SelectedKeyName);
			return EBTNodeResult::Succeeded;
		}
		
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
