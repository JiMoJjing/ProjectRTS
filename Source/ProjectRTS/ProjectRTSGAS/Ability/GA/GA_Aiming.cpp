// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Aiming.h"

#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

void UGA_Aiming::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo();
	if (RTSCharacter)
	{
		RTSCharacter->StartAiming();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RTSCharacter is nullptr"));
	}
}

void UGA_Aiming::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Aiming::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo();
	if (RTSCharacter)
	{
		RTSCharacter->StopAiming();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RTSCharacter is nullptr"));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
