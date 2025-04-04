// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerAniminstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectRTS/Characters/Players/RTSPlayerBase.h"

URTSPlayerAniminstance::URTSPlayerAniminstance()
{
}

void URTSPlayerAniminstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RTSPlayerBase = Cast<ARTSPlayerBase>(GetOwningActor());
	if (RTSPlayerBase)
	{
		MovementComponent = RTSPlayerBase->GetCharacterMovement();
	}
}

void URTSPlayerAniminstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!RTSPlayerBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("URTSPlayerAniminstance::RTSPlayerBase nullptr"));
		return;
	}
	if(!MovementComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("URTSPlayerAniminstance::MovementComponent nullptr"));
		return;
	}
	
	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();
	bIsMove = GroundSpeed > 3.0f;
	bIsFalling = MovementComponent->IsFalling();
	bIsAiming = RTSPlayerBase->IsAiming();
	bIsBooster = RTSPlayerBase->IsBooster();

	FVector LastInputVector = MovementComponent->GetLastInputVector();
	FVector VelocityXY = FVector(Velocity.X, Velocity.Y, 0.0f);
	FRotator ControlRotation = RTSPlayerBase->GetControlRotation();
	FRotator BaseAimRotation = RTSPlayerBase->GetBaseAimRotation();

	Direction = UKismetAnimationLibrary::CalculateDirection(LastInputVector, ControlRotation);
	BoosterDirection = UKismetAnimationLibrary::CalculateDirection(VelocityXY, ControlRotation);
	Pitch = BaseAimRotation.Pitch;
}
