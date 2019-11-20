// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/InteractiveActor.h"

#include "Engine.h"

#ifndef DEBUGPRINT
#define DEBUGPRINT(text,color) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, color, TEXT(text));}
#endif

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



bool AInteractiveActor::IsOnBlackList(AActor* actor)
{
	if (BlacklistClasses.Num() > 0)
	{
		for (int i = 0; i < BlacklistClasses.Num(); i++)
		{
			if (actor->GetClass() == BlacklistClasses[i] || actor->GetClass()->IsChildOf(BlacklistClasses[i]))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

bool AInteractiveActor::CanUse(AActor* actor)
{
	if (!IsOnBlackList(actor))
	{
		if (bCanBeUsed)
		{
			return true;
		}
		else
		{
			if (SpecialClasses.Num() > 0)
			{
				for (int i = 0; i < SpecialClasses.Num(); i++)
				{
					if (actor->GetClass() == SpecialClasses[i] || actor->GetClass()->IsChildOf(SpecialClasses[i]))
					{
						return bCanOnlySpecialClassesUse ? true : false;
					}
				}
				return bCanOnlySpecialClassesUse ? false : true;
			}
			else
			{
				return bCanOnlySpecialClassesUse ? false : true;
			}
		}
	}
	else
	{
		return true;
	}
}


bool AInteractiveActor::IsOneOfSpecial(AActor* actor)
{

	if (SpecialClasses.Num() > 0)
	{
		for (int i = 0; i < SpecialClasses.Num(); i++)
		{
			if (actor->GetClass() == SpecialClasses[i] || actor->GetClass()->IsChildOf(SpecialClasses[i]))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

void AInteractiveActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveActor, bCanBeUsed);
	DOREPLIFETIME(AInteractiveActor, BlacklistClasses);
	DOREPLIFETIME(AInteractiveActor, SpecialClasses);
}

