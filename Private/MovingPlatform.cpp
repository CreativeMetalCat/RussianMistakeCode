// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "Kismet/GameplayStatics.h"

AMovingPlatform::AMovingPlatform()
{
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));

	RootComponent = PlatformMesh;

	MovementSoundLoop = CreateDefaultSubobject<UAudioComponent>(TEXT("MovementSoundLoopComponent"));

	
	/*MovementSoundLoop->AddToRoot();*/

}

void AMovingPlatform::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMovingPlatform, UpdateTimerHandle);
}

void AMovingPlatform::OnConstruction(const FTransform& Transform)
{
	OriginalLocation = GetActorLocation();

	if (MovementSound != nullptr)
	{
		MovementSoundLoop->SetSound(MovementSound);
	}
}

void AMovingPlatform::StartMovement_Implementation(bool StartEvenIfCanNotBeUsed)
{
	if (!bMoving)
	{
		if (bCanBeUsed)
		{
			//start movement
			GetWorldTimerManager().SetTimer(UpdateTimerHandle,this,&AMovingPlatform::UpdateLocation, 0.01f, true, StartDelay);
			
			if (!bUseFMODAudio && MovementSound != nullptr)
			{
				MovementSoundLoop->Play();
			}
			else if (MovementSoundEvent != nullptr)
			{
				MovementSoundEventInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), MovementSoundEvent, GetActorTransform(), true);
			}
			bMoving = true;
			if (StartDelay != 0.0f) { PreStartMovement(); }
		}
		else if (StartEvenIfCanNotBeUsed)
		{
			//startmovememt
			GetWorldTimerManager().SetTimer(UpdateTimerHandle, 0.01f, true, StartDelay);
			if (bUseFMODAudio && MovementSound != nullptr)
			{
				MovementSoundLoop->Play();
			}
			else if (MovementSoundEvent != nullptr)
			{
				MovementSoundEventInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), MovementSoundEvent, GetActorTransform(), true);
			}
			GetWorldTimerManager().SetTimer(UpdateTimerHandle, this, &AMovingPlatform::UpdateLocation, 0.01f, true, StartDelay);
			bMoving = true;
			OnStopMovementDelegate.Broadcast(bOpened);
			if (StartDelay != 0.0f) { PreStartMovement(); }
		}
		else
		{
			if (!bUseFMODAudio&&LockedSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), LockedSound, PlatformMesh->RelativeLocation + GetActorLocation(), 1.f, 1.f, 0.f, Attenuation);
			}
			else if (LockedSoundEvent != nullptr)
			{
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), LockedSoundEvent, GetActorTransform(), true);
			}
			//play failed sound
		}
	}
}

void AMovingPlatform::PreStartMovement_Implementation()
{
	if (!bUseFMODAudio&& StartMovementSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), StartMovementSound, PlatformMesh->RelativeLocation + GetActorLocation(), 1.f, 1.f, 0.f, Attenuation);
	}
	else if (StartMovementEvent != nullptr)
	{
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), StartMovementEvent, GetActorTransform(), true);
	}
}

void AMovingPlatform::UpdateLocation_Implementation()
{
	float DeltaTime = 0.01f;

	MovedDistance += (bOpened ? Speed * (OpenDistance > 0.f ? (-1.f) : (1.f))* DeltaTime : Speed * (OpenDistance > 0.f ? (1.f) : (-1.f))* DeltaTime);
	if (AxisToMoveAlong == EAxisEnum::AE_X)
	{
		SetActorLocation(FVector(OriginalLocation.X + MovedDistance, GetActorLocation().Y, GetActorLocation().Z));
	}
	else if (AxisToMoveAlong == EAxisEnum::AE_Y)
	{
		SetActorLocation(FVector(GetActorLocation().X, OriginalLocation.Y + MovedDistance, GetActorLocation().Z));
	}
	else if (AxisToMoveAlong == EAxisEnum::AE_Z)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, OriginalLocation.Z + MovedDistance));
	}

	if ((bOpened ? MovedDistance <= 0.0f : MovedDistance >= OpenDistance))
	{
		bOpened = !bOpened;
		StopMovement();
	}
}

void AMovingPlatform::StopMovement_Implementation()
{
	bMoving = false;
	if (MovementSoundLoop->IsPlaying()) { MovementSoundLoop->Stop(); }
	MovementSoundEventInstance.Instance->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
	if (!bUseFMODAudio && StopSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), StopSound, PlatformMesh->RelativeLocation + GetActorLocation(), 1.f, 1.f, 0.f, Attenuation);
	}
	else if (StopSoundEvent != nullptr)
	{
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), StopSoundEvent, GetActorTransform(), true);
	}
	GetWorldTimerManager().ClearTimer(UpdateTimerHandle);
	OnStopMovementDelegate.Broadcast(bOpened);
}

void AMovingPlatform::OnInteraction_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent)
{
	if (CanUse(interactor)) 
	{
		OnBeenUsedDelegate.Broadcast(interactor);
		StartMovement(IsOneOfSpecial(interactor));
	}
	else
	{
		OnFailedToUseDelegate.Broadcast(interactor);
	}
}

void AMovingPlatform::BeginPlay()
{

	

	bOpened ? MovedDistance = OpenDistance : MovedDistance = 0;
	if (AxisToMoveAlong == EAxisEnum::AE_X)
	{
		SetActorLocation(FVector(OriginalLocation.X + MovedDistance, GetActorLocation().Y, GetActorLocation().Z));
	}
	else if (AxisToMoveAlong == EAxisEnum::AE_Y)
	{
		SetActorLocation(FVector(GetActorLocation().X, OriginalLocation.Y + MovedDistance, GetActorLocation().Z));
	}
	else if (AxisToMoveAlong == EAxisEnum::AE_Z)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, OriginalLocation.Z + MovedDistance));
	}

}
