// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/InteractionInterface.h"
#include "UnrealNetwork.h"
#include "InteractiveActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeenUsedDelegate,AActor*,User);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFailedToUseDelegate, AActor*, User);

UCLASS()
class SLENDER_API AInteractiveActor : public AActor,public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FBeenUsedDelegate OnBeenUsedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FFailedToUseDelegate OnFailedToUseDelegate;

	/**IF you want to have both fmod and Unreal Audio objects and need a way to choose between them*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**IF you want to have both fmod and Unreal Audio objects and need a way to choose between them*/
		bool bUseFMODAudio = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Replicated)
		bool bCanBeUsed = true;

	/**Classes that can use object even if bCanBeUsed = false*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		/**Classes that can use object even if bCanBeUsed = false*/
		TArray<TSubclassOf<AActor>> SpecialClasses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		TArray<TSubclassOf<AActor>> BlacklistClasses;
		
	/**If true only children of special classes can use, if false everyone except them can*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**If true only children of special classes can use, if false everyone except them can*/
		bool bCanOnlySpecialClassesUse = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
		bool IsOnBlackList(AActor* actor);

	UFUNCTION(BlueprintPure)
		bool IsOneOfSpecial(AActor* actor);

	UFUNCTION(BlueprintPure)
		bool CanUse(AActor* actor);

};
