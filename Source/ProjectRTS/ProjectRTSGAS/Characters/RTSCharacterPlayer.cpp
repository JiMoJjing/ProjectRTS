// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectRTS/ProjectRTS.h"
#include "ProjectRTS/RTSGameplayTag.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySet.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Animation/RTSAnimInstance.h"
#include "ProjectRTS/ProjectRTSGAS/Input/RTSInputComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Player/RTSPlayerState.h"

ARTSCharacterPlayer::ARTSCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg Mesh"));
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));
	BackMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Back Mesh"));
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));

	ArmMesh->SetupAttachment(GetMesh());
	LegMesh->SetupAttachment(GetMesh());
	HeadMesh->SetupAttachment(GetMesh());
	BackMesh->SetupAttachment(GetMesh());
	GunMesh->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultBodyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectRTS/Characters/Player/Mesh/ModularParts/SK_Body_01.SK_Body_01'"));
	if (DefaultBodyMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(DefaultBodyMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultArmMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectRTS/Characters/Player/Mesh/ModularParts/SK_Arm_01.SK_Arm_01'"));
	if (DefaultArmMesh.Object)
	{
		ArmMesh->SetSkeletalMesh(DefaultArmMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultLegMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectRTS/Characters/Player/Mesh/ModularParts/SK_Leg_01.SK_Leg_01'"));
	if (DefaultLegMesh.Object)
	{
		LegMesh->SetSkeletalMesh(DefaultLegMesh.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultBackMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectRTS/Characters/Player/Mesh/ModularParts/SK_Back_01.SK_Back_01'"));
	if (DefaultBackMesh.Object)
	{
		BackMesh->SetSkeletalMesh(DefaultBackMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultHeadMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectRTS/Characters/Player/Mesh/ModularParts/SK_Head_01.SK_Head_01'"));
	if (DefaultHeadMesh.Object)
	{
		HeadMesh->SetSkeletalMesh(DefaultHeadMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectRTS/Characters/Player/Mesh/ModularParts/SK_Gun_AR.SK_Gun_AR'"));
	if (DefaultGunMesh.Object)
	{
		GunMesh->SetSkeletalMesh(DefaultGunMesh.Object);
	}
	
	// SetLeaderPoseComponent.
	SetLeaderPoseComponent();
	
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

void ARTSCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ARTSPlayerState* RTSPlayerState = Cast<ARTSPlayerState>(GetPlayerState());
	if (RTSPlayerState)
	{
		// ASC 받아서 저장.
		RTSASC = RTSPlayerState->GetRTSAbilitySystemComponent();
		// InitAbilityActorInfo 처리.
		if (RTSASC)
		{
			RTSASC->InitAbilityActorInfo(RTSPlayerState, this);

			// RTSAnimInstance에 ASC 세팅.
			if (URTSAnimInstance* RTSAnimInstance = Cast<URTSAnimInstance>(GetMesh()->GetAnimInstance()))
			{
				RTSAnimInstance->InitializeWithAbilitySystem(RTSASC);
			}		
		}

		// 디버그 바로 보이게 설정.
		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		if (PlayerController)
		{
			//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
		}
	}
}

UAbilitySystemComponent* ARTSCharacterPlayer::GetAbilitySystemComponent() const
{
	return RTSASC;
}

void ARTSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Timeline Settings.
	if (AimingTimelineCurveFloat)
	{
		OnAimingTimelineFloat.BindUFunction(this, FName(TEXT("AimingUpdate")));
		FOnTimelineEvent FinishEvent;
		
		AimingTimeline.AddInterpFloat(AimingTimelineCurveFloat, OnAimingTimelineFloat);
		AimingTimeline.SetTimelineFinishedFunc(FinishEvent);
		AimingTimeline.SetTimelineLength(AimingTimelineLength);
		AimingTimeline.SetLooping(false);
	}
}

void ARTSCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimingTimeline.TickTimeline(DeltaTime);
}

void ARTSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// IMC등록 및 BindAction처리.
	InitializePlayerInput(PlayerInputComponent);

	// AbilitySet에 등록된 Ability들 GiveAbility 처리.
	RegisterAbilitySet();
}

void ARTSCharacterPlayer::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	if (RTSASC == nullptr)
	{
		RTS_LOG(LogRTS, Log, TEXT("%s"), TEXT("ASC is nullptr"));
		return;
	}

	if (InputDataAsset == nullptr)
	{
		RTS_LOG(LogRTS, Log, TEXT("%s"), TEXT("InputDataAsset is nullptr"));
		return;
	}

	if (InputMappingContext == nullptr)
	{
		RTS_LOG(LogRTS, Log, TEXT("%s"), TEXT("InputMappingContext is nullptr"));
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	if (URTSInputComponent* RTSInputComponent =  Cast<URTSInputComponent>(PlayerInputComponent))
	{
		// Ability Action Bind.
		RTSInputComponent->BindAbilityAction(InputDataAsset, this, &ARTSCharacterPlayer::Input_AbilityInputTagPressed, &ARTSCharacterPlayer::Input_AbilityInputTagReleased);
		
		// Native Action Bind.
		RTSInputComponent->BindNativeAction(InputDataAsset, RTSGameplayTag::InputTag_Move, ETriggerEvent::Triggered, this, &ARTSCharacterPlayer::Input_Move);
		RTSInputComponent->BindNativeAction(InputDataAsset, RTSGameplayTag::InputTag_Look, ETriggerEvent::Triggered, this, &ARTSCharacterPlayer::Input_Look);
	}
}

bool ARTSCharacterPlayer::TraceToCrosshair(FHitResult& OutHitResult, float InTraceDistance, ECollisionChannel InTraceChannel, bool bUseShotSpread)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}
		
		FVector2D CrosshairLocation2D = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
		FVector CrosshairWorldLocation;
		FVector CrosshairWorldRotation;

		if (bUseShotSpread)
		{
			float SpreadAngle;
			float SpeedSquared = GetVelocity().SizeSquared2D();
			float MaxSpeedSquared = RunSpeed * RunSpeed;

			if (SpeedSquared > (MaxSpeedSquared * 0.5f))
			{
				SpreadAngle = MaxSpreadAngle;
			}
			else
			{
				SpreadAngle = MinSpreadAngle;
			}
			
			CrosshairWorldRotation = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(CrosshairWorldRotation, SpreadAngle);
		}
		
		if (PlayerController->DeprojectScreenPositionToWorld(CrosshairLocation2D.X, CrosshairLocation2D.Y, CrosshairWorldLocation, CrosshairWorldRotation))
		{
			const FVector TraceStartLocation = CrosshairWorldLocation;
			const FVector TraceEndLocation = CrosshairWorldLocation + CrosshairWorldRotation * InTraceDistance;

#if WITH_EDITOR
			DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Green, false, 3.0f, 0, 5.0f);
#endif
			
			return GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStartLocation, TraceEndLocation, InTraceChannel);
		}
	}
	return false;
}

void ARTSCharacterPlayer::SetLeaderPoseComponent()
{
	if (USkeletalMeshComponent* Body = GetMesh())
	{
		ArmMesh->SetLeaderPoseComponent(Body);
		LegMesh->SetLeaderPoseComponent(Body);
		HeadMesh->SetLeaderPoseComponent(Body);
		BackMesh->SetLeaderPoseComponent(Body);
		GunMesh->SetLeaderPoseComponent(Body);
	}
}

void ARTSCharacterPlayer::Input_Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	const FRotator YawRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.Y);
	AddMovementInput(RightDirection, InputValue.X);
}

void ARTSCharacterPlayer::Input_Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
	UENUM(BlueprintType)
	enum class EWeaponClass : uint8
	{
		Punch UMETA(DisplayName = "Punch"),
		Gun UMETA(DisplayName = "Gun"),
		Knife UMETA(DisplayName = "Knife")
	};
}

void ARTSCharacterPlayer::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (RTSASC)
	{
		RTSASC->AbilityInputTagPressed(InputTag);
	}
}

void ARTSCharacterPlayer::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (RTSASC)
	{
		RTSASC->AbilityInputTagReleased(InputTag);
	}
}

void ARTSCharacterPlayer::UseControlRotation()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ARTSCharacterPlayer::UseMovementRotation()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ARTSCharacterPlayer::StartAiming()
{
	UseControlRotation();
	SpringArmComponent->bEnableCameraLag = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	AimingTimeline.Play();
}

void ARTSCharacterPlayer::StopAiming()
{
	UseMovementRotation();
	SpringArmComponent->bEnableCameraLag = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	AimingTimeline.Reverse();
}

void ARTSCharacterPlayer::RegisterAbilitySet()
{
	if (AbilitySet && RTSASC)
	{
		AbilitySet->GiveToAbilitySystem(RTSASC);
	}
}

void ARTSCharacterPlayer::AimingUpdate(float Alpha) const
{
	float CurrentSpringArmLength = FMath::Lerp(DefaultSpringArmLength, AimingSpringArmLength, Alpha);
	FVector CurrentCameraPosition = FMath::Lerp(DefaultCameraPosition, AimingCameraPosition, Alpha);

	SpringArmComponent->TargetArmLength = CurrentSpringArmLength;
	CameraComponent->SetRelativeLocation(CurrentCameraPosition);
}
