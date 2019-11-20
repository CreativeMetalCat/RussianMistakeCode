// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AI/SlenderAICharacter.h"
#include "Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASlenderAICharacter::ASlenderAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlenderAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(HuntUpdateTimer, this, &ASlenderAICharacter::ShouldStartHunt, 0.01f, true);
	GetWorldTimerManager().SetTimer(TeleportUpdateTimer, this, &ASlenderAICharacter::TeleportAround , 0.01f, true);
}

// Called every frame
void ASlenderAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlenderAICharacter::StartHunt_Implementation()
{
	if (HuntingMusic->IsValidLowLevel())
	{
		HuntingMusicInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), HuntingMusic, GetActorTransform(), true);
	}
	IsHunting = true;
}

AActor* ASlenderAICharacter::GetClosestTargetOnAllMap_Implementation(TSubclassOf<AActor> targetClass)
{
	if (this->GetWorld() != nullptr)
	{
		TArray<AActor*>actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), targetClass, actors);
		if (actors.Num() > 0)
		{
			float prevDist = -1.0f;
			int prefferdIndex = -1;
			for (int i = 0; i < actors.Num(); i++)
			{

				float dist = FVector::Distance(actors[i]->GetActorLocation(), this->GetActorLocation());
				if (prevDist == -1.0f && IAIInterface::Execute_GetIsDead(actors[i]) == false) { prefferdIndex = i; prevDist = dist; }
				else if (dist < prevDist && IAIInterface::Execute_GetIsDead(actors[i]) == false) { prefferdIndex = i; prevDist = dist; }

			}
			if (prefferdIndex!=-1)
			{
				return actors[prefferdIndex];
			}
			else { return nullptr; }
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

// Called to bind functionality to input
void ASlenderAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

AActor* ASlenderAICharacter::GetClosestTarget_Implementation(TSubclassOf<AActor> targetClass, float maxDistance = 2000, float minDistance = 0)
{
	if (this->GetWorld() != nullptr)
	{
		TArray<AActor*>actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), targetClass, actors);
		if (actors.Num() > 0)
		{
			for (int i = 0; i < actors.Num(); i++)
			{
				auto dist = FVector::Distance(actors[i]->GetActorLocation(), this->GetActorLocation());
				if (IAIInterface::Execute_GetIsDead(actors[i]) == false && dist <maxDistance && dist>minDistance && this != actors[i])
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

void ASlenderAICharacter::UpdateTarget()
{
	if (GetController() != nullptr)
	{
		auto target = GetClosestTarget(TSubclassOf<ACharacter>(),2000,0);
		if (target != nullptr)
		{
			if (UAIBlueprintHelperLibrary::GetBlackboard(this)->GetValueAsObject(BlackboardTargetValueName) != target)
			{

				UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsObject(BlackboardTargetValueName, target);
			}
		}
	}
}

void ASlenderAICharacter::ShouldStartHunt_Implementation()
{
	float DeltaTime = 0.01f;
	if (!IsHunting)
	{
		if (WasRecentlyRendered())
		{
			WasLookedAtFor += DeltaTime;
			if (WasLookedAtFor >= MaxLookAtTime)
			{
				StartHunt();
				WasLookedAtFor = 0.0f;
			}
		}
		else
		{
			WasLookedAtFor -= DeltaTime;
			if (WasLookedAtFor < 0) { WasLookedAtFor = 0; }
		}


		//slender is supposed to be allowed to move only after first note is collected by default
		if (bAllowedToMove)
		{
			TimeSinceLastNoteCollection += DeltaTime;
			if (TimeSinceLastNoteCollection >= MaxTimeForCollectingNotes) { StartHunt(); }
		}
	}
}


void ASlenderAICharacter::AttackPlayer(ACharacter* player)
{
	IAIInterface::Execute_DisableControl(player,true,true);
	if (AttackSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, GetActorLocation());
	}
	player->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), GetActorLocation()));
	
	IAIInterface::Execute_BeKilled(player);
}

void ASlenderAICharacter::TeleportAround_Implementation()
{
	TimeSinceTeleport += 0.01f/*DeltaTime*/;
	if (bAllowedToMove && !IsHunting && TimeSinceTeleport >= TimeBetweenTeleports)
	{
		TimeSinceTeleport = 0.0f;
		if (TeleportPoints.Num() > 0)
		{

			bool teleported = false;
			int failedAttempts = 0;
			for (int o = 0; o < 3/*max failed attempts*/; o++)
			{
				
				int id = FMath::RandRange(0, TeleportPoints.Num() - 1);
				if (TeleportPoints[id]->CanBeUsed(ASlenderAICharacter::StaticClass()))
				{
					SetActorLocation(TeleportPoints[id]->GetActorLocation());
					teleported = true;

					break; break;

				}
				failedAttempts++;

			}
			if (teleported == false)
			{
				//try to find empty spot the hard way
				for (int a = 0; a < TeleportPoints.Num(); a++)
				{
					if (TeleportPoints[a]->CanBeUsed(ASlenderAICharacter::StaticClass()))
					{
						SetActorLocation(TeleportPoints[a]->GetActorLocation());
						break;
					}
				}
			}
			if (TeleportSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), TeleportSound, GetActorLocation());
			}
		}
	}
}

void ASlenderAICharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASlenderAICharacter, BlackboardTargetValueName);
	DOREPLIFETIME(ASlenderAICharacter, MaxSenseDistance);
	DOREPLIFETIME(ASlenderAICharacter, IsHunting);
	DOREPLIFETIME(ASlenderAICharacter, MaxLookAtTime);
	DOREPLIFETIME(ASlenderAICharacter, WasLookedAtFor);

	DOREPLIFETIME(ASlenderAICharacter, TimeSinceTeleport);
	DOREPLIFETIME(ASlenderAICharacter, TimeBetweenTeleports);
	DOREPLIFETIME(ASlenderAICharacter, TeleportPoints);

	DOREPLIFETIME(ASlenderAICharacter, bAllowedToMove);

	DOREPLIFETIME(ASlenderAICharacter, TimeSinceLastNoteCollection);
	DOREPLIFETIME(ASlenderAICharacter, MaxTimeForCollectingNotes);

	DOREPLIFETIME(ASlenderAICharacter, PatrolPoints);
	DOREPLIFETIME(ASlenderAICharacter, PatrolNodeId);
	
}
