// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "InteractionInterface.h"

#include "HiddingPlaceBase.generated.h"

/**
 * 
 */
UCLASS()
class SLENDER_API AHiddingPlaceBase : public AInteractiveActor
{
	GENERATED_BODY()

public:
	/**Positon where player will end up when getting out of this place*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (key=MakeEditWidget))
		/**Positon where player will end up when getting out of this place*/
		FVector StepOutToPosition;

	/**Positon where player will hide relative to actor to avoid clipping into the model*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere/*, meta = (key = MakeEditWidget)*/)
		/**Positon where player will hide relative to actor to avoid clipping into the model*/
		FVector RelativeHiddingPosition = FVector(0, 5, 90);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnInteraction(AActor* interactor);

	void OnInteraction_Implementation(AActor* interactor);
};
