// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorBase.h"
#include "Public/InteractionInterface.h"
#include "Engine.h"

void ADoorBase::OnInteraction_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent)
{
	if (bNeedsKey)
	{
		if (bCanBeUsedWithMultipleKeys && RequiredKeyIds.Num() > 0)
		{
			auto keys = Execute_GetAllKeyIds(interactor);
			if (keys.Num() > 0)
			{
				for (int i = 0; i < keys.Num(); i++)
				{
					for (int a = 0; a < RequiredKeyIds.Num(); a++)
					{
						if (keys[i] == RequiredKeyIds[a]) 
						{
							/*Open door now*/
							if (bTakesAwayKey) { Execute_TakeAwayKey(interactor,RequiredKeyIds[a]); }
							UnLock();							
							return;
						}
					}
				}
			}
		}
		else
		{
			auto keys = Execute_GetAllKeyIds(interactor);
			
			if (keys.Num() > 0)
			{
				for (int i = 0; i < keys.Num(); i++)
				{
					
					if (keys[i] == RequiredKeyId)
					{
						if (bTakesAwayKey)
						{ Execute_TakeAwayKey(interactor, RequiredKeyId); }
						UnLock();
						return;
					}
				}
			}
		}
		if (LockedSound->IsValidLowLevel())
		{
			LockedSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), LockedSound, GetActorTransform(), true);
		}
	}
	else
	{
		Open();
	}
}

void ADoorBase::Open_Implementation()
{
	if (OpenSound->IsValidLowLevel())
	{
		OpenSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), OpenSound, GetActorTransform(), true);
	}
}

void ADoorBase::Close_Implementation()
{
	if (CloseSound->IsValidLowLevel())
	{
		CloseSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), CloseSound, GetActorTransform(), true);
	}
}

void ADoorBase::UnLock_Implementation()
{
	if (UnLockSound->IsValidLowLevel())
	{
		UnLockSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), UnLockSound, GetActorTransform(), true);
	}
	bNeedsKey = false;
}

void ADoorBase::Lock_Implementation()
{
	if (LockSound->IsValidLowLevel())
	{
		LockSoundInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), LockSound, GetActorTransform(), true);
	}
	bNeedsKey = true;
}
