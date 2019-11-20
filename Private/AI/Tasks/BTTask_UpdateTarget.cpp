// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UpdateTarget.h"
#include "AIController.h"
#include "Engine.h"
#include "AI/AIInterface.h"
#include "SlenderCharacter.h"
#include "Public/AI/SlenderAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"



void UBTTask_UpdateTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("TickTask")); }
	
}

EBTNodeResult::Type UBTTask_UpdateTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner() != nullptr)
	{
		if (OwnerComp.GetAIOwner()->GetPawn() != nullptr)
		{
			if (FailIfNotOverwatchMode ? IAIInterface::Execute_GetAIType(OwnerComp.GetAIOwner()->GetPawn()) == EAITypeEnum::EAIT_Overwatch : true)
			{
				if (!IAIInterface::Execute_GetIsDead(OwnerComp.GetAIOwner()->GetPawn()) && IAIInterface::Execute_GetIsHunting(OwnerComp.GetAIOwner()->GetPawn()))
				{

					auto pawn = OwnerComp.GetAIOwner()->GetPawn();

					auto target = IAIInterface::Execute_GetClosestTargetOnAllMap(pawn, ASlenderCharacter::StaticClass());
					if (target != nullptr)
					{
						if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName) != target)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(Target.SelectedKeyName, target);
						}
					}
					else
					{

					}
					return EBTNodeResult::Type::Succeeded;
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->ClearValue(Target.SelectedKeyName);

					return EBTNodeResult::Type::Succeeded;
				}
			}
			else
			{
				return EBTNodeResult::Type::Failed;
			}
		}
		else
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No pawn")); }
		}
	}
	else
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No controller")); }
		return EBTNodeResult::Type::Failed;
	}
	return EBTNodeResult::Type::Failed;
}

void UBTTask_UpdateTarget::OnGameplayTaskActivated(UGameplayTask& Task)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("TaskActivated")); }
}
