// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ARTSPlayerBase::ARTSPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(100.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -100));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	SpringArmComponent->TargetArmLength = DefaultSpringArmLength;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 20.0f;
	SpringArmComponent->CameraLagMaxDistance = 150.0f;
	SpringArmComponent->bDoCollisionTest = true;

	CameraComponent->SetRelativeLocation(DefaultCameraPosition);

	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxAcceleration = 5000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->GroundFriction = 0.1f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.75f;
	GetCharacterMovement()->MaxFlySpeed = BoosterSpeed;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

void ARTSPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	// Add Mapping Context. 
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
}

void ARTSPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ARTSPlayerBase::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ARTSPlayerBase::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ARTSPlayerBase::Jump);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ARTSPlayerBase::StopJumping);
		EnhancedInputComponent->BindAction(LeftMouseInputAction, ETriggerEvent::Triggered, this, &ARTSPlayerBase::LeftMouseTriggered);
		EnhancedInputComponent->BindAction(LeftMouseInputAction, ETriggerEvent::Completed, this, &ARTSPlayerBase::LeftMouseCompleted);
		EnhancedInputComponent->BindAction(RightMouseInputAction, ETriggerEvent::Triggered, this, &ARTSPlayerBase::RightMouseTriggered);
		EnhancedInputComponent->BindAction(RightMouseInputAction, ETriggerEvent::Completed, this, &ARTSPlayerBase::RightMouseCompleted);
	}

	if (AimingTimelineCurveFloat)
	{
		OnAimingTimelineFloat.BindUFunction(this, FName(TEXT("AimingUpdate")));
		FOnTimelineEvent FinishEvent;
		
		AimingTimeline.AddInterpFloat(AimingTimelineCurveFloat, OnAimingTimelineFloat);
		AimingTimeline.SetTimelineFinishedFunc(FinishEvent);
		AimingTimeline.SetTimelineLength(0.15f);
		AimingTimeline.SetLooping(false);
	}
}

void ARTSPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimingTimeline.TickTimeline(DeltaTime);
}

void ARTSPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	const FRotator YawRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.Y);
	AddMovementInput(RightDirection, InputValue.X);
}

void ARTSPlayerBase::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}

void ARTSPlayerBase::Jump()
{
	Super::Jump();
}

void ARTSPlayerBase::StopJumping()
{
	Super::StopJumping();
}

void ARTSPlayerBase::LeftMouseTriggered()
{
	UseControlRotation();
}

void ARTSPlayerBase::LeftMouseCompleted()
{
	UseMovementRotation();
}

void ARTSPlayerBase::RightMouseTriggered()
{
	BeginAimingSettings();
}

void ARTSPlayerBase::RightMouseCompleted()
{
	StopAimingSettings();
}

void ARTSPlayerBase::UseControlRotation()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ARTSPlayerBase::UseMovementRotation()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ARTSPlayerBase::BeginAimingSettings()
{
	bAiming = true;
	UseControlRotation();
	SpringArmComponent->bEnableCameraLag = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	AimingTimeline.Play();
}

void ARTSPlayerBase::StopAimingSettings()
{
	bAiming = false;
	UseMovementRotation();
	SpringArmComponent->bEnableCameraLag = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	AimingTimeline.Reverse();
}

void ARTSPlayerBase::AimingUpdate(float Alpha) const
{
	float CurrentSpringArmLength = FMath::Lerp(DefaultSpringArmLength, AimingSpringArmLength, Alpha);
	FVector CurrentCameraPosition = FMath::Lerp(DefaultCameraPosition, AimingCameraPosition, Alpha);

	SpringArmComponent->TargetArmLength = CurrentSpringArmLength;
	CameraComponent->SetRelativeLocation(CurrentCameraPosition);

	UE_LOG(LogTemp, Warning, TEXT("AimingUpdate"));
}