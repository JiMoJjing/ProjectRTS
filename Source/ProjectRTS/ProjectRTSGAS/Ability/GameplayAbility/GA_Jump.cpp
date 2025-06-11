// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Jump.h"

#include "Abilities/Tasks/AbilityTask_StartAbilityState.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/Character.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

UGA_Jump::UGA_Jump(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ARTSCharacterPlayer* RTSCharacter = Cast<ARTSCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (!RTSCharacter || !RTSCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CharacterJumpStart();
	UAbilityTask_StartAbilityState* StartAbilityState = UAbilityTask_StartAbilityState::StartAbilityState(this, TEXT("Jumping"), true);
	if (StartAbilityState)
	{
		StartAbilityState->OnStateEnded.AddDynamic(this, &UGA_Jump::CharacterJumpStop);
		StartAbilityState->OnStateInterrupted.AddDynamic(this, &UGA_Jump::CharacterJumpStop);
		StartAbilityState->ReadyForActivation();
	}

	UAbilityTask_WaitInputRelease* WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	if (WaitInputRelease)
	{
		WaitInputRelease->OnRelease.AddDynamic(this, &UGA_Jump::OnWaitInputReleasedCallback);
		WaitInputRelease->ReadyForActivation();
	}
}

void UGA_Jump::OnWaitInputReleasedCallback(float TimeHeld)
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Jump::CharacterJumpStart()
{
	if (ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo())
	{
		if (RTSCharacter->IsLocallyControlled() && !RTSCharacter->bPressedJump)
		{
			RTSCharacter->Jump();
		}
	}
}

void UGA_Jump::CharacterJumpStop()
{
	if (ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo())
	{
		if (RTSCharacter->IsLocallyControlled() && RTSCharacter->bPressedJump)
		{
			RTSCharacter->StopJumping();
		}
	}
}
