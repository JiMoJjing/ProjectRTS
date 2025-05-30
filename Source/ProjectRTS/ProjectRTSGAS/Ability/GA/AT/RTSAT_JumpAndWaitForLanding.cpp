// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSAT_JumpAndWaitForLanding.h"

#include "GameFramework/Character.h"

URTSAT_JumpAndWaitForLanding::URTSAT_JumpAndWaitForLanding()
{
}

URTSAT_JumpAndWaitForLanding* URTSAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	URTSAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<URTSAT_JumpAndWaitForLanding>(OwningAbility);
	
	return NewTask;
}

void URTSAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (Character)
	{
		Character->LandedDelegate.AddDynamic(this, &URTSAT_JumpAndWaitForLanding::OnLandedCallback);
		Character->Jump();
	}
	SetWaitingOnAvatar();
}

void URTSAT_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (Character)
	{
		Character->LandedDelegate.RemoveDynamic(this, &URTSAT_JumpAndWaitForLanding::OnLandedCallback);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void URTSAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& LandHit)
{
	OnCompleted.Broadcast();
}