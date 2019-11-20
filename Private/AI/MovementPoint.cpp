// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AI/MovementPoint.h"

AMovementPoint::AMovementPoint()
{

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(SizeOfPoint);
	CollisionBox->SetRelativeLocation(FVector(0, 0, 0));
	CollisionBox->bDynamicObstacle = false;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	CollisionBox->SetCanEverAffectNavigation(false);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	GetRootComponent()->SetRelativeLocation(FVector(0, 0, 0));
}

TArray<AActor*> AMovementPoint::GetAllActorsNearBy(TSubclassOf<AActor> classType)
{
	TArray<AActor*> result;
	if (CollisionBox != nullptr)
	{
		CollisionBox->GetOverlappingActors(result, classType);		
	}
	return result;
}

bool AMovementPoint::CanBeUsed_Implementation(TSubclassOf<AActor> classType)
{
	if (!bEnabled) { return false; }
	else
	{
		if (GetAllActorsNearBy(classType).Num() > 0) { return false; }
		else { return true && (bUseChance ? GenerateChanceToBeUsed() : true); }
	}
}

bool AMovementPoint::CanBeUsedMany_Implementation(const TArray<TSubclassOf<AActor> >& classTypes, bool FailOnlyIfAll)
{
	if (classTypes.Num() > 0)
	{
		bool result;
		for (int i = 0; i < classTypes.Num(); i++)
		{
			bool can = CanBeUsed(classTypes[i]);
			if (FailOnlyIfAll && !can) { return false; }
			else { i == 0 ? result == can : result &= can; }
		}
		return result;
	}
	else
	{
		return true;
	}
}
