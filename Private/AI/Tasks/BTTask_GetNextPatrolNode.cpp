// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetNextPatrolNode.h"
#include "AIController.h"
#include "Engine.h"
#include "AI/AIInterface.h"
#include "Public/AI/SlenderAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTTask_GetNextPatrolNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}

void UBTTask_GetNextPatrolNode::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

EBTNodeResult::Type UBTTask_GetNextPatrolNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner() != nullptr)
	{
		if (OwnerComp.GetAIOwner()->GetPawn() != nullptr)
		{
			auto ai = OwnerComp.GetAIOwner()->GetPawn();
			int nodeId = IAIInterface::Execute_GetCurrentPatrolId(ai);
			auto patrolPoints = IAIInterface::Execute_GetPatrolPoints(ai);
			
			if (patrolPoints.Num() > 0)
			{
				if (nodeId + 1 < patrolPoints.Num())
				{

					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ReslutKey.SelectedKeyName, patrolPoints[nodeId + 1]);
					IAIInterface::Execute_SetCurrentPatrolId(ai, nodeId + 1);
					return EBTNodeResult::Type::Succeeded;
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ReslutKey.SelectedKeyName, patrolPoints[0]);
					IAIInterface::Execute_SetCurrentPatrolId(ai, 0);
					return EBTNodeResult::Type::Succeeded;
				}
			}
			else
			{
				return EBTNodeResult::Type::Failed;
			}
		}
		return EBTNodeResult::Type::Failed;
	}
	return EBTNodeResult::Type::Failed;
}
