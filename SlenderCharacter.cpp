// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SlenderCharacter.h"
#include "SlenderProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Public/InteractionInterface.h"
#include "Engine.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Public/AI/SlenderAICharacter.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

#ifndef DEBUGPRINT
#define DEBUGPRINT(text,color) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, color, TEXT(text));}
#endif

//////////////////////////////////////////////////////////////////////////
// ASlenderCharacter

ASlenderCharacter::ASlenderCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;


	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	if (CameraSpringArm)
	{
		CameraSpringArm->SetupAttachment(GetMesh(), FName("headSocket"));
		CameraSpringArm->bUsePawnControlRotation = true;
		CameraSpringArm->TargetArmLength = 0;
	}

	/*PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->CastShadow = false;
	PlayerMesh->bCastDynamicShadow = false;
	PlayerMesh->RelativeLocation = FVector(0.0f,0.0f,-93.f);
	PlayerMesh->RelativeRotation = FRotator(0.0f, 0.0f, -90.f);*/
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraSpringArm);
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.


	NameDisplay = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameDisplay"));
	NameDisplay->CastShadow = false;
	NameDisplay->bCastDynamicShadow = false;
	NameDisplay->SetupAttachment(RootComponent);
	NameDisplay->SetRelativeLocation(FVector(0.0f, 20.0f, 120.0f));

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	SetReplicates(true);
	bAlwaysRelevant = true;
	bReplicates = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ASlenderCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, TEXT("Update")); }
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructo

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	

	//setup timer for update flashlight info
	GetWorld()->GetTimerManager().SetTimer(FlashLightUpdateTimer, this, &ASlenderCharacter::CheckFlashlight, 0.01f);
	

	if (!bChagedName)
	{

		auto controller = Cast<APlayerController>(GetController());
		//if (controller != nullptr)
		//{
		//	this->NameDisplay->Text = (controller->PlayerState != nullptr ? FText::FromString(controller->PlayerState->PlayerName) : FText::FromString("LocalPlayer"));
		//	bChagedName = true;
		//}
		if (GetPlayerState() != nullptr)
		{
			this->NameDisplay->SetText(FText::FromString(GetPlayerState()->GetPlayerName()));
			
			bChagedName = true;
		}
		else
		{
			if (GEngine)
			{
				if (GetController() == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, "this actor is not controlled is ");
				}
				if (GetPlayerState() == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, "GetPlayerState() is null");
				}
				if (Cast<APlayerController>(GetController()) == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, "this actor is not controlled by player");
				}

			}
			bChagedName = false;
		}
	}
}

void ASlenderCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	TimeSinceLastStep += DeltaTime;
	if (IsRunning)
	{
		RunningTime += DeltaTime;
		if (RunningTime > MaxRunningTime)
		{
			OnStopRunning();
			if (OutOfBreathSound != nullptr && GetWorld() != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), OutOfBreathSound);
			}
		}
	}
	else if (RunningTime != 0)
	{
		if (RunningTime > 0) { RunningTime -= DeltaTime; }
		if (RunningTime < 0) { RunningTime = 0; }
	}
	if (bAutoFootstep)
	{
		if (TimeSinceLastStep >= (IsRunning ? TimeBetweenSteps / 2 : (bIsCrouched ? TimeBetweenSteps * 2 : TimeBetweenSteps)))
		{
			PlayFootstepSound();
			TimeSinceLastStep = 0.f;
		}
	}

}

void ASlenderCharacter::OnConstruction(const FTransform& Transform)
{
	//PlayerMesh->HideBoneByName(TEXT("head"),EPhysBodyOp::PBO_None);
}

void ASlenderCharacter::StartCrouching()
{
	if (CanCrouch()) { Crouch();  GetMesh()->SetRelativeLocation(FVector(0, 0, -49.f));}
}

void ASlenderCharacter::StopCrouching()
{
	UnCrouch(); GetMesh()->SetRelativeLocation(FVector(0, 0, -93.f));
}

bool ASlenderCharacter::CanRun_Implementation()
{
	if (this->IsRunning || bIsCrouched) { return false; }
	else
	{
		if (RunningTime >= MaxRunningTime) { return false; }
		else { return true; }
	}

	return false;
}

void ASlenderCharacter::FillSaveData_Implementation(UGameSave* saveObject)
{
	saveObject->AmountOfCollectedNotes = NeededAmountOfNotes;
	saveObject->AmountOfNotesTotal = AmountOfNotes;
	saveObject->TimeOfSaving = FDateTime::Now();
	saveObject->MapName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	TArray<AActor*> slenders;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASlenderAICharacter::StaticClass(), slenders);
	if (slenders.Num() > 0)
	{
		for (int i = 0; i < slenders.Num(); i++)
		{
			saveObject->SlendersLocations.Add(slenders[i]->GetActorLocation());
		}
	}
}

int ASlenderCharacter::IncreaseAmountOfNotes_Implementation(int amount)
{
	if (this->GetWorld() != nullptr)
	{

		if (this->GetWorld()->GetGameState()->PlayerArray.Num() == 1)
		{
			if ((AmountOfNotes + amount) <= NeededAmountOfNotes)
			{
				AmountOfNotes += amount;
				return AmountOfNotes;
			}
			else
			{
				return AmountOfNotes;
			}
		}
		else if (this->GetWorld()->GetGameState()->PlayerArray.Num() > 1)
		{
			int totalAmountOfNotes = 0;
			for (int i = 0; i < this->GetWorld()->GetGameState()->PlayerArray.Num(); i++)
			{

				if (this->GetWorld()->GetGameState()->PlayerArray[i]->GetPawn() != nullptr)
				{
					auto player = Cast<ASlenderCharacter>(this->GetWorld()->GetGameState()->PlayerArray[i]->GetPawn());
					if (player != nullptr)
					{
						totalAmountOfNotes += player->AmountOfNotes;
					}
				}
			}
			if ((totalAmountOfNotes + amount) <= NeededAmountOfNotes)
			{
				AmountOfNotes += amount;
				return AmountOfNotes;
			}
			else
			{
				return AmountOfNotes;
			}
		}
		else { return -1; }
	}
	else
	{
		return -1;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

bool ASlenderCharacter::BeKilled_Implementation()
{
	Dead = true;
	if (GetWorld() != nullptr)
	{
		TArray<AActor*> players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), players);
		if (players.Num() > 0)
		{
			for (int i = 0; i < players.Num(); i++)
			{
				if (players[i] != nullptr && players[i] != this && !IAIInterface::Execute_GetIsDead(players[i]))
				{
					if (GetController() != nullptr && Cast<APlayerController>(GetController()) != nullptr)
					{
						Cast<APlayerController>(GetController())->SetViewTargetWithBlend(players[i]);
					}
				}
			}
		}
	}
	return Dead;
}

void ASlenderCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind jump events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASlenderCharacter::StartCrouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASlenderCharacter::StopCrouching);

	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &ASlenderCharacter::ToggleFlashlight);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASlenderCharacter::OnFire);

	// Bind use event
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ASlenderCharacter::Use);
	
	//bind running controls
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASlenderCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASlenderCharacter::OnStopRunning);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASlenderCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASlenderCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASlenderCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASlenderCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASlenderCharacter::LookUpAtRate);
}

void ASlenderCharacter::OnStartRunning()
{
	if (CanRun()) 
	{ 
		IsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed *= 2;
		if (StartRunningSound != nullptr && GetWorld() != nullptr)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), StartRunningSound);
		}
	}
}

void ASlenderCharacter::OnStopRunning()
{
	if (IsRunning) { IsRunning = false; GetCharacterMovement()->MaxWalkSpeed /= 2; }
}

void ASlenderCharacter::OnFire()
{
}

void ASlenderCharacter::ChangeNameDisplayOfPlayer_Implementation(const FString& newName)
{
	NameDisplay->SetText(FText::FromString(newName));
	return;
}

//inefficent, need to be updated to be more efficent
//or maybe it is efficent enough
AActor* ASlenderCharacter::GetClosestTarget_Implementation(TSubclassOf<AActor> targetClass, float maxDistance, float minDistance)
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

EPhysicalSurface ASlenderCharacter::GetFootstepSurfaceType()
{

	FHitResult OutHit;
	FCollisionQueryParams params;
	params.bReturnPhysicalMaterial = true;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation(), GetActorLocation() - FVector(0, 0, 100), ECollisionChannel::ECC_Visibility, params);
	if (OutHit.bBlockingHit)
	{		
		return (OutHit.PhysMaterial.Get() == nullptr) ? EPhysicalSurface::SurfaceType_Default : OutHit.PhysMaterial->SurfaceType;
	}
	else
	{
		return EPhysicalSurface::SurfaceType_Default;
	}
	
}

void ASlenderCharacter::Use()
{
	if (this->GetWorld() != nullptr)
	{
		FHitResult hit;
		FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
		params.AddIgnoredActor(this);

		this->GetWorld()->LineTraceSingleByChannel(hit, FVector(0, 0, bHidding ? 0 : 70) + GetActorLocation(), GetActorLocation() + FVector(0, 0, 70) + GetFirstPersonCameraComponent()->GetForwardVector() * 500, ECollisionChannel::ECC_Visibility, params);

		/*DrawDebugLine(GetWorld(), FVector(0, 0, bHidding ? 0 : 70) + GetActorLocation(), GetActorLocation() + FVector(0, 0, 70) + GetFirstPersonCameraComponent()->GetForwardVector() * 500, FColor::Red, true);*/
		if (hit.bBlockingHit)
		{
			if (hit.Actor != nullptr)
			{
				IInteractionInterface* interface = Cast<IInteractionInterface>(hit.GetActor());
				if (interface)
				{
					IInteractionInterface::Execute_OnInteraction(hit.GetActor(), this, hit.GetComponent());
				}
			}
			else if (hit.Component != nullptr)
			{

			}
			else
			{

			}
		}

		else
		{
			//failed
		}
	}
	else
	{
		DEBUGPRINT("World is null", FColor::Red)
	}
}

void ASlenderCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASlenderCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASlenderCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ASlenderCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ASlenderCharacter::MoveForward(float Value)
{
	if (CanMove)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorForwardVector(), Value);
		}
	}
}

void ASlenderCharacter::MoveRight(float Value)
{
	if (CanMove)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorRightVector(), Value);
		}
	}
}

void ASlenderCharacter::TurnAtRate(float Rate)
{
	if (CanRotate)		
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	if (CameraLimitsInfo.bSupposedToLimit && CameraLimitsInfo.bLimitYaw)
	{
		GetController()->SetControlRotation(FRotator(GetControlRotation().Pitch, FMath::ClampAngle(GetControlRotation().Yaw, CameraLimitsInfo.MaxYaw, CameraLimitsInfo.MinYaw), GetControlRotation().Roll));
	}

}

void ASlenderCharacter::LookUpAtRate(float Rate)
{
	if (CanRotate)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}

	if (CameraLimitsInfo.bSupposedToLimit && CameraLimitsInfo.bLimitPitch)
	{
		GetController()->SetControlRotation(FRotator(FMath::ClampAngle(GetControlRotation().Pitch, CameraLimitsInfo.MaxPitch, CameraLimitsInfo.MinPitch), GetControlRotation().Yaw, GetControlRotation().Roll));
	}
}

bool ASlenderCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASlenderCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASlenderCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASlenderCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void ASlenderCharacter::DisableControl_Implementation(bool disableMovement = true, bool disableRotation = true)
{
	CanMove = !disableMovement;
	CanRotate = !disableRotation;
}

float ASlenderCharacter::GetDangerLevel_Implementation()
{
	auto enemy = GetClosestTarget(ASlenderAICharacter::StaticClass(), MaxEnemyAwarnessDistance, 0);
	if (enemy != nullptr)
	{
		float distance = FVector::Distance(enemy->GetActorLocation(), this->GetActorLocation());
		return (1 - (distance / MaxEnemyAwarnessDistance));
	}
	else
	{
		return 0.0f;
	}
}

bool ASlenderCharacter::HasKeyId_Implementation(int Id)
{
	TArray<int> keys; 
	Keys.GetKeys(keys);
	if (keys.Num() > 0) 
	{
		for (int i = 0; i < keys.Num(); i++)
		{
			if (keys[i] == Id) { return true; }
		}
	}
	return false;
}

bool ASlenderCharacter::AddKeyId_Implementation(int Id, FDoorKeyInfo info)
{
	if (Keys.Find(Id) != nullptr) { return false; }
	else
	{
		Keys.Add(Id, info);
		return true;
	}
}

TArray<int> ASlenderCharacter::GetAllKeyIds_Implementation()
{
	TArray<int> keys;
	Keys.GetKeys(keys);
	return keys;
}

void ASlenderCharacter::TakeAwayKey_Implementation(int keyId)
{
	if (Keys.Find(keyId) != nullptr) { Keys.Remove(keyId); }
}

void ASlenderCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ASlenderCharacter, AmountOfNotes);
	DOREPLIFETIME(ASlenderCharacter, NeededAmountOfNotes);
	DOREPLIFETIME(ASlenderCharacter, CanMove);
	DOREPLIFETIME(ASlenderCharacter, CanRotate);
	DOREPLIFETIME(ASlenderCharacter, Dead);
	DOREPLIFETIME(ASlenderCharacter, NameDisplay);
	DOREPLIFETIME(ASlenderCharacter, MaxRunningTime);
	DOREPLIFETIME(ASlenderCharacter, RunningTime);
	DOREPLIFETIME(ASlenderCharacter, CanBeAttacked);
	DOREPLIFETIME(ASlenderCharacter, IsRunning);

	DOREPLIFETIME(ASlenderCharacter, DistanceToNearestEnemy);

	DOREPLIFETIME(ASlenderCharacter, bFlashLightOn);
	DOREPLIFETIME(ASlenderCharacter, MaxFlashlightTime);
	DOREPLIFETIME(ASlenderCharacter, UsedFlashlightTime);
	DOREPLIFETIME(ASlenderCharacter, FlashLightUpdateTimer);
	DOREPLIFETIME(ASlenderCharacter, bFlashlightAutoRecharges);
	DOREPLIFETIME(ASlenderCharacter, bAllowedToUseFlashLight);

	DOREPLIFETIME(ASlenderCharacter, FootstepTimerHandle);

	DOREPLIFETIME(ASlenderCharacter, bHidding);
}

bool ASlenderCharacter::CanUseFlashlight_Implementation()
{
	return (UsedFlashlightTime < MaxFlashlightTime && bAllowedToUseFlashLight);
}

void ASlenderCharacter::SetAllowedControl_Implementation(bool allowCamera, bool allowMovement)
{
	GetController()->SetIgnoreLookInput(!allowCamera);
	GetController()->SetIgnoreMoveInput(!allowMovement);
}

void ASlenderCharacter::CheckFlashlight_Implementation(void)
{
	
	float UpdateStep = 0.01f;
	if (bFlashLightOn) 
	{
		UsedFlashlightTime += UpdateStep;
		if (UsedFlashlightTime >= MaxFlashlightTime) { TurnOffFlashlight(); }
	}
	else if (bFlashlightAutoRecharges && UsedFlashlightTime != 0.f)
	{
		UsedFlashlightTime -= UpdateStep;
		if (UsedFlashlightTime < 0.f) { UsedFlashlightTime = 0.f; }
	}
}
