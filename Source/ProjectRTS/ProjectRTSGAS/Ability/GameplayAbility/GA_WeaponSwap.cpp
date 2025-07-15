// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WeaponSwap.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

UGA_WeaponSwap::UGA_WeaponSwap(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

void UGA_WeaponSwap::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	WeaponIndex = 0;
	if (!WeaponSwapMontage)
	{
		bool bReplicateEndAbility = true;
		bool bWasCancelled = true;

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
	}

	WeaponIndex = (uint8)TriggerEventData->EventMagnitude;

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(TEXT("WeaponSwap")), WeaponSwapMontage, 1.5f);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_WeaponSwap::SwapCancelled);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGA_WeaponSwap::SwapCancelled);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_WeaponSwap::SwapFinished);

	PlayMontageAndWait->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_WeaponSwap::SwapCancelled()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_WeaponSwap::SwapFinished()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;

	GetRTSCharacterFromActorInfo()->WeaponSwapAbilitySuccess(WeaponIndex);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
