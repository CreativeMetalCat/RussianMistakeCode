// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AI/SlenderAICharacter.h"
#include "Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "fmod_errors.h"
#include "Public/PlayerInteractions.h"


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

	GetWorldTimerManager().SetTimer(SenseUpdateTimerHandle, this, &ASlenderAICharacter::UpdateSense, 0.1f, true);

	GetWorldTimerManager().SetTimer(FootstepUpdateTimer, this, &ASlenderAICharacter::PlayFootstepSound, 0.3f, true);

	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
}

// Called every frame
void ASlenderAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASlenderAICharacter::CheckLocation_Implementation()
{
	if (GetController() != nullptr)
	{
		UAIBlueprintHelperLibrary::GetBlackboard(this)->ClearValue("LastKnownLocation");
	}

	GetWorldTimerManager().SetTimer(LookAroundTimer, this, &ASlenderAICharacter::StopLookingAround, 5.f, false);
	bIsLookingAround = true;
	return true; 
}

void ASlenderAICharacter::StartHunt_Implementation()
{
	FMOD_STUDIO_PLAYBACK_STATE state;
	HuntingMusicInstance.Instance->getPlaybackState(&state);
	if (HuntingMusic->IsValidLowLevel() && state != FMOD_STUDIO_PLAYBACK_STATE::FMOD_STUDIO_PLAYBACK_PLAYING)
	{
		HuntingMusicInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), HuntingMusic, GetActorTransform(), true);
	}
	GetCharacterMovement()->MaxWalkSpeed = HuntingMovementSpeed;
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

void ASlenderAICharacter::StopHunt_Implementation()
{
	if (HuntingMusicInstance.Instance->isValid())
	{
		FMOD_RESULT res = HuntingMusicInstance.Instance->setParameterByName("Looping", 1.f);
		if (res != FMOD_RESULT::FMOD_OK)
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FMOD_ErrorString(res)); }
			HuntingMusicInstance.Instance->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
		}
	}
	WasLookedAtFor = 0;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
	IsHunting = false;
}

// Called to bind functionality to input
void ASlenderAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ASlenderAICharacter::UpdateSight_Implementation(UAIPerceptionComponent* Perception)
{
	/*if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Perception->GetFName().ToString()); }*/
	TArray<AActor*>actors;
	Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), actors);
	if (actors.Num() > 0)
	{
		if (GetController() != nullptr)
		{
			if (UAIBlueprintHelperLibrary::GetBlackboard(this)->GetValueAsObject("Player") != nullptr)
			{
				for (int i = 0; i < actors.Num(); i++)
				{
					if (UAIBlueprintHelperLibrary::GetBlackboard(this)->GetValueAsObject("Player") == actors[i])
					{
						/*actor is still visible*/
						UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsVector("LastKnownLocation", actors[i]->GetActorLocation());
						return;
					}
				}
				UAIBlueprintHelperLibrary::GetBlackboard(this)->ClearValue("Player");
				StopHunt();
			}
			//can't see anymore or haven't seen yet
			if (EnemyClasses.Num() > 0)
			{
				for (int i = 0; i < actors.Num(); i++)
				{
					for (int a = 0; a < EnemyClasses.Num(); a++)
					{
						if (actors[i]->GetClass()->IsChildOf(EnemyClasses[a]) || actors[i]->GetClass() == EnemyClasses[a]) 
						{
							if (!IPlayerInteractions::Execute_IsHidding(actors[i])&& !IAIInterface::Execute_GetIsDead(actors[i]))
							{
								UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsObject("Player", actors[i]);
								UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsBool("CanSeeTarget", true);
								StartHunt();
								return;
							}
						}
					}
				}
			}
			UAIBlueprintHelperLibrary::GetBlackboard(this)->ClearValue("Player");
			UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsBool("CanSeeTarget", false);

		}
	}
	UAIBlueprintHelperLibrary::GetBlackboard(this)->ClearValue("Player");
	UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsBool("CanSeeTarget", false);
	
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

	if (AttackSound->IsValidLowLevel())
	{
		AttackSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), AttackSound, GetActorTransform(), true);
	}
	player->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), GetActorLocation()));
	StopHunt();
	
	IAIInterface::Execute_BeKilled(player);
}

void ASlenderAICharacter::PlayFootstepSound_Implementation()
{
	if (GetVelocity().Size() > 50)
	{
		if (FootstepSound->IsValidLowLevel())
		{
			FootstepSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), FootstepSound, GetActorTransform(), true);
		}
	}
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

			if (TeleportSound->IsValidLowLevel())
			{
				TeleportSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), TeleportSound, GetActorTransform(), true);
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

	DOREPLIFETIME(ASlenderAICharacter, TeleportSoundInstance);
	DOREPLIFETIME(ASlenderAICharacter, AttackSoundInstance);

	DOREPLIFETIME(ASlenderAICharacter, HuntingMovementSpeed);
	DOREPLIFETIME(ASlenderAICharacter, DefaultMovementSpeed);

	DOREPLIFETIME(ASlenderAICharacter, bIsLookingAround);
	DOREPLIFETIME(ASlenderAICharacter, LookAroundTimer);
	
}
