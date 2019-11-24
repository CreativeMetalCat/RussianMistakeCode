// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddingPlaceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/PlayerInteractions.h"

void AHiddingPlaceBase::OnInteraction_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent)
{
	if (Cast<ACharacter>(interactor) != nullptr)
	{
		if (!IPlayerInteractions::Execute_IsHidding(interactor) && CanHide(interactor, interactedComponent))
		{
			if (Cast<UCharacterMovementComponent>(Cast<ACharacter>(interactor)->GetMovementComponent()) != nullptr)
			{
				Cast<UCharacterMovementComponent>(Cast<ACharacter>(interactor)->GetMovementComponent())->DisableMovement();
			}
			interactor->SetActorLocation(RelativeHiddingPosition + GetActorLocation());
			interactor->SetActorEnableCollision(false);
			IPlayerInteractions::Execute_ForceCrouch(interactor);
			FCameraLimitsInfo info = IPlayerInteractions::Execute_GetCameraLimitations(interactor);
			info.bSupposedToLimit = true;
			IPlayerInteractions::Execute_SetCameraLimitations(interactor, info);
			IPlayerInteractions::Execute_SetIsHidding(interactor, true);

			PlayStartHiddingEffects();
		}
		else
		{
			interactor->SetActorLocation(StepOutToPosition + GetActorLocation());
			interactor->SetActorEnableCollision(true);
			IPlayerInteractions::Execute_ForceUnCrouch(interactor);
			FCameraLimitsInfo info = IPlayerInteractions::Execute_GetCameraLimitations(interactor);
			info.bSupposedToLimit = false;
			IPlayerInteractions::Execute_SetCameraLimitations(interactor, info);
			IPlayerInteractions::Execute_SetIsHidding(interactor, false);
			if (Cast<UCharacterMovementComponent>(Cast<ACharacter>(interactor)->GetMovementComponent()) != nullptr)
			{
				Cast<UCharacterMovementComponent>(Cast<ACharacter>(interactor)->GetMovementComponent())->SetMovementMode(EMovementMode::MOVE_Walking);
			}

			PlayStopHiddingEffects();
		}
	}
}
