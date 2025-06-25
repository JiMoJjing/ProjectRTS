// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Reload.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Attribute/RTSAmmoSet.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"
#include "ProjectRTS/ProjectRTSGAS/Weapon/RTSWeaponContext.h"

UGA_Reload::UGA_Reload(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

void UGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo();
	if (!RTSCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_Reload ActivateAbility RTSCharacterPlayer is nullptr"));
		return;
	}

	URTSWeaponContext* CurrentWeaponContext = RTSCharacter->GetCurrentWeaponContext();
	if (!CurrentWeaponContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_Reload ActivateAbility CurrentWeaponContext is nullptr"));
		return;
	}

	UAnimMontage* ReloadMontage = CurrentWeaponContext->ReloadMontage;
	if (!ReloadMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_Reload ActivateAbility ReloadMontage is nullptr"));
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontageAndWait"), ReloadMontage);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGA_Reload::ReloadCancelled);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_Reload::ReloadCancelled);
	//PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UGA_Reload::ReloadCancelled);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Reload::ReloadSuccess);
	PlayMontageAndWait->ReadyForActivation();

	URTSAbilitySystemComponent* RTSASC = GetRTSAbilitySystemComponentFromActorInfo();
	if (!RTSASC)
	{
		return;
	}
	const URTSAmmoSet* RTSAmmoSet = RTSASC->GetSet<URTSAmmoSet>();
	if (!RTSAmmoSet)
	{
		return;
	}

	float CurrentAmmo = RTSAmmoSet->GetCurrentAmmo();
	float MaxAmmo = RTSAmmoSet->GetMaxAmmo();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UGA_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{	
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	URTSAbilitySystemComponent* RTSASC = GetRTSAbilitySystemComponentFromActorInfo();
	if (!RTSASC)
	{
		return false;
	}

	const URTSAmmoSet* RTSAmmoSet = RTSASC->GetSet<URTSAmmoSet>();
	if (!RTSAmmoSet)
	{
		return false;
	}

	float CurrentAmmo = RTSAmmoSet->GetCurrentAmmo();
	float MaxAmmo = RTSAmmoSet->GetMaxAmmo();
	
	return CurrentAmmo < MaxAmmo;
}		

void UGA_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Reload::ReloadCancelled()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = true;

	UE_LOG(LogTemp, Warning, TEXT("ReloadCancelled"));
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Reload::ReloadSuccess()
{
	if (IsLocallyControlled() && ReloadEffectClass)
	{
		BP_ApplyGameplayEffectToOwner(ReloadEffectClass, 1, 1);
	}
	
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}