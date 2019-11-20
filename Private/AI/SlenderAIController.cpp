// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AI/SlenderAIController.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ASlenderAIController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ASlenderAIController, BehaviorTree);
}

void ASlenderAIController::BeginPlay()
{
	Super::BeginPlay();


	InitAI();
	if (BehaviorTree)
	{
		if (GEngine) { RunBehaviorTree(BehaviorTree) ? GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "true") : GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "falsee"); }
	}
}

AActor* ASlenderAIController::GetClosestTarget_Implementation(TSubclassOf<AActor> targetClass, float maxDistance = 2000, float minDistance = 0)
{
	if (GetPawn() != nullptr)
	{
		auto pawn = GetPawn();
		if (this->GetWorld() != nullptr)
		{
			TArray<AActor*>actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), targetClass, actors);
			if (actors.Num() > 0)
			{
				for (int i = 0; i < actors.Num(); i++)
				{
					auto dist = FVector::Distance(actors[i]->GetActorLocation(), pawn->GetActorLocation());
					if (IAIInterface::Execute_GetIsDead(actors[i]) == false && dist < maxDistance && dist > minDistance && this != actors[i])
					{
						return actors[i];
					}
				}
				return nullptr;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

void ASlenderAIController::Tick(float DeltaTime) 
{
}