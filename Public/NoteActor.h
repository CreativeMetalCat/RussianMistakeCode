// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"

#include "Public/InteractInterface.h"
#include "Public/AI/AIInterface.h"
#include"Public/AI/MovementPoint.h"
#include "NoteActor.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API ANoteActor : public AInteractiveActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundBase* PickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SlenderMovement)
		TArray<AMovementPoint*>SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* Mesh;
public:

	ANoteActor();

	/**Must be called in blueprint on OnInteraction */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnInteractionServer(AActor* interactor, UPrimitiveComponent* interactedComponent);

	void OnInteractionServer_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent);
};
