// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_JumpAndWaitForLanding.h"

#include "GameFramework/Character.h"

UAT_JumpAndWaitForLanding::UAT_JumpAndWaitForLanding()
{
}

UAT_JumpAndWaitForLanding* UAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UAT_JumpAndWaitForLanding>(OwningAbility);
	
	return NewTask;
}

void UAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (Character)
	{
		Character->LandedDelegate.AddDynamic(this, &UAT_JumpAndWaitForLanding::OnLandedCallback);
		Character->Jump();
	}
	SetWaitingOnAvatar();
}

void UAT_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (Character)
	{
		Character->LandedDelegate.RemoveDynamic(this, &UAT_JumpAndWaitForLanding::OnLandedCallback);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& LandHit)
{
	OnCompleted.Broadcast();
}