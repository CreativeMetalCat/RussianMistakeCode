// Fill out your copyright notice in the Description page of Project Settings.


#include "SlenderGameInstance.h"

void USlenderGameInstance::SetSavedValues_Implementation(bool AllowCustscene, const TArray<FVector>& SlendersPositions, bool Hunting, bool AllowedToMove)
{
	this->bAllowCustscene = AllowCustscene;
	this->SlendersLocations = SlendersPositions;
	this->bHunting = Hunting;
	this->bAllowedToMove = AllowedToMove;
}