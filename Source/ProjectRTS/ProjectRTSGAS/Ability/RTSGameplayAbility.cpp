// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameplayAbility.h"

#include "RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"
#include "ProjectRTS/ProjectRTSGAS/Player/RTSPlayerController.h"

URTSGameplayAbility::URTSGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	// 리플리케이션 정책.
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;

	// 인스턴싱 정책.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Network에서 어떻게 실행할 지 정책.
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 서버나 클라에서 이 GA를 어떻게 보호할지.
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

URTSAbilitySystemComponent* URTSGameplayAbility::GetRTSAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<URTSAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ARTSPlayerController* URTSGameplayAbility::GetRTSPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ARTSPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* URTSGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

ARTSCharacterPlayer* URTSGameplayAbility::GetRTSCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ARTSCharacterPlayer>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

void URTSGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URTSGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FGameplayEffectContextHandle URTSGameplayAbility::MakeEffectContext(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo) const
{
	FGameplayEffectContextHandle ContextHandle = Super::MakeEffectContext(Handle, ActorInfo);

	UObject* SourceObject = GetSourceObject(Handle, ActorInfo);
	AActor* Instigator = ActorInfo ? ActorInfo->OwnerActor.Get() : nullptr;

	ContextHandle.AddSourceObject(SourceObject);
	ContextHandle.AddInstigator(Instigator, nullptr);

	return ContextHandle;
}
