// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_Fire.h"

#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

UGC_Fire::UGC_Fire(const FObjectInitializer& ObjectInitializer)
{
}

bool UGC_Fire::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{	
	ARTSCharacterPlayer* RTSCharacter = Cast<ARTSCharacterPlayer>(MyTarget);
	if (RTSCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GC_Fire RTSCharacter nullptr"));
		return false;
	}

	FVector Location = Parameters.Location;
	FVector Normal = Parameters.Normal;
	
	TArray<FVector> ImpactPositions;
	ImpactPositions.Add(Location);
	
	TArray<FVector> ImpactNormals;
	ImpactNormals.Add(Normal);
	
	RTSCharacter->SpawnFireEffectActor(ImpactPositions, ImpactNormals);
	
	return true;
}
