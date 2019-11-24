// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteActor.h"
#include "SlenderCharacter.h"
#include "SlenderAICharacter.h"
#include "Engine.h"
#include "Public/GlobalInteractions.h"
#include "Kismet/GameplayStatics.h"

void ANoteActor::OnInteractionServer_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent)
{
	if (Cast<ASlenderCharacter>(interactor) != nullptr)
	{
		if (PickUpSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
		}
		TArray<AActor*>slenders;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASlenderAICharacter::StaticClass(), slenders);
		if (slenders.Num() > 0 && SpawnPoints.Num() > 0)
		{
			for (int i = 0; i < slenders.Num(); i++)
			{
				bool teleported = false;
				int failedAttempts = 0;
				for (int o = 0; o < 3/*max failed attempts*/; o++)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::FromInt(o));
					int id = FMath::RandRange(0, SpawnPoints.Num() - 1);
					if (SpawnPoints[id]->CanBeUsed(ASlenderAICharacter::StaticClass()))
					{
						slenders[i]->SetActorLocation(SpawnPoints[id]->GetActorLocation());
						teleported = true;
						
						 break; break;

					}
					failedAttempts++;
					
				}
				if (teleported == false)
				{
					//try to find empty spot the hard way
					for (int a = 0; a < SpawnPoints.Num(); a++)
					{
						if (SpawnPoints[a]->CanBeUsed(ASlenderAICharacter::StaticClass()))
						{
							slenders[i]->SetActorLocation(SpawnPoints[a]->GetActorLocation());
							break;
						}
					}
				}
				
				IAIInterface::Execute_StopHunt(slenders[i]);
				Cast< ASlenderAICharacter>(slenders[i])->bAllowedToMove = true;
				Cast< ASlenderAICharacter>(slenders[i])->TimeSinceLastNoteCollection = 0.f;
			}


		}
		TArray<AActor*>allGlobalActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UGlobalInteractions::StaticClass(), allGlobalActors);
		if (allGlobalActors.Num() > 0) 
		{
			for (int i = 0; i < allGlobalActors.Num(); i++)
			{
				IGlobalInteractions::Execute_NoteHasBeenCollected(allGlobalActors[i], this);
			}
		}

		Destroy();
	}
}

ANoteActor::ANoteActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}
