// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSCharacterPlayer.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectRTS/ProjectRTS.h"
#include "ProjectRTS/RTSGameplayTag.h"
#include "ProjectRTS/Physics/RTSCollisionChannel.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySet.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Animation/RTSAnimInstance.h"
#include "ProjectRTS/ProjectRTSGAS/Attribute/RTSAmmoSet.h"
#include "ProjectRTS/ProjectRTSGAS/Input/RTSInputComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Player/RTSPlayerController.h"
#include "ProjectRTS/ProjectRTSGAS/Player/RTSPlayerState.h"
#include "ProjectRTS/ProjectRTSGAS/Weapon/FireEffect.h"
#include "ProjectRTS/ProjectRTSGAS/Weapon/RTSWeaponContext.h"

ARTSCharacterPlayer::ARTSCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg Mesh"));
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));
	BackMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Back Mesh"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));

	ArmMesh->SetupAttachment(GetMesh());
	LegMesh->SetupAttachment(GetMesh());
	HeadMesh->SetupAttachment(GetMesh());
	BackMesh->SetupAttachment(GetMesh());
	WeaponMesh->SetupAttachment(GetMesh());

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
		WeaponMesh->SetSkeletalMesh(DefaultGunMesh.Object);
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
				GameplayTagPropertyMap.Initialize(this, RTSASC);
			}
			
			const URTSAmmoSet* RTSAmmoSet = RTSASC->GetSet<URTSAmmoSet>();
			if (RTSAmmoSet && IsLocallyControlled())
			{
				RTSAmmoSet->OnAmmoChanged.AddUObject(this, &ARTSCharacterPlayer::OnAmmoChangedCallback);
			}
		}

		// 디버그 바로 보이게 설정.
		ARTSPlayerController* RTSPlayerController = CastChecked<ARTSPlayerController>(NewController);
		if (RTSPlayerController)
		{
			//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
		}
		
		// WeaponInitialize
		InitializeWeapon();
		
		// IMC등록 및 BindAction처리.
		InitializePlayerInput(RTSPlayerController->InputComponent);

		// AbilitySet에 등록된 Ability들 GiveAbility 처리.
		RegisterAbilitySet();
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

	if (RecoilTimelineCurveFloat)
	{
		OnRecoilTimelineFloat.BindUFunction(this, FName(TEXT("RecoilUpdate")));
		FOnTimelineEvent RecoilFinishEvent;
		RecoilFinishEvent.BindUFunction(this, FName(TEXT("RecoilFinish")));

		RecoilTimeline.AddInterpFloat(RecoilTimelineCurveFloat, OnRecoilTimelineFloat);
		RecoilTimeline.SetTimelineFinishedFunc(RecoilFinishEvent);
		RecoilTimeline.SetTimelineLength(RecoilTimelineLength);
		RecoilTimeline.SetLooping(false);
	}

	
}

void ARTSCharacterPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSCharacterPlayer, CurrentWeaponType);
}

void ARTSCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimingTimeline.TickTimeline(DeltaTime);
	RecoilTimeline.TickTimeline(DeltaTime);
}

void ARTSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
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

		// Weapon Swap Action Bind.
		RTSInputComponent->BindWeaponSwapAction(InputDataAsset, this, &ARTSCharacterPlayer::Input_WeaponSwap);
	}
}

void ARTSCharacterPlayer::InitializeWeapon()
{
	if (WeaponContexts.Num() == 0)
	{
		RTS_LOG(LogRTS, Log, TEXT("WeaponContexts is empty!"));
		return;
	}
	

	URTSWeaponContext* NewWeaponContext = WeaponContexts[0];
	if (NewWeaponContext)
	{
		// NewWeaponContext의 GA부여.
		FGameplayAbilitySpec NewAbilitySpec(NewWeaponContext->WeaponGA);
		NewAbilitySpec.DynamicAbilityTags.AddTag(NewWeaponContext->WeaponInputTag);
		RTSASC->GiveAbility(NewAbilitySpec);	

		// Weapon SkeletalMesh 변경.
		if (NewWeaponContext->WeaponMesh)
		{
			WeaponMesh->SetSkeletalMesh(NewWeaponContext->WeaponMesh);
		}

		// LeaderPose 다시 설정.
		SetLeaderPoseComponent();

		// 무기 Enum 변경, Context갱신.
		CurrentWeaponContext = NewWeaponContext;
		CurrentWeaponType = NewWeaponContext->WeaponType;

		// RTSAmmoSet 세팅.
		RTSASC->SetNumericAttributeBase(URTSAmmoSet::GetMaxAmmoAttribute(), CurrentWeaponContext->MaxAmmo);
		RTSASC->SetNumericAttributeBase(URTSAmmoSet::GetCurrentAmmoAttribute(), CurrentWeaponContext->MaxAmmo);
	}
}

// bool ARTSCharacterPlayer::TraceToCrosshair(FHitResult& OutHitResult, float InTraceDistance, ECollisionChannel InTraceChannel, bool bUseShotSpread)
// {
// 	APlayerController* PlayerController = Cast<APlayerController>(GetController());
// 	if (PlayerController)
// 	{
// 		FVector2D ViewportSize;
// 		if (GEngine && GEngine->GameViewport)
// 		{
// 			GEngine->GameViewport->GetViewportSize(ViewportSize);
// 		}
// 		
// 		FVector2D CrosshairLocation2D = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
// 		FVector CrosshairWorldLocation;
// 		FVector CrosshairWorldRotation;
//
// 		if (bUseShotSpread)
// 		{
// 			float SpreadAngle;
// 			float SpeedSquared = GetVelocity().SizeSquared2D();
// 			float MaxSpeedSquared = RunSpeed * RunSpeed;
//
// 			if (SpeedSquared > (MaxSpeedSquared * 0.5f))
// 			{
// 				SpreadAngle = MaxSpreadAngle;
// 			}
// 			else
// 			{
// 				SpreadAngle = MinSpreadAngle;
// 			}
// 			
// 			CrosshairWorldRotation = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(CrosshairWorldRotation, SpreadAngle);
// 		}
// 		
// 		if (PlayerController->DeprojectScreenPositionToWorld(CrosshairLocation2D.X, CrosshairLocation2D.Y, CrosshairWorldLocation, CrosshairWorldRotation))
// 		{
// 			const FVector TraceStartLocation = CrosshairWorldLocation;
// 			const FVector TraceEndLocation = CrosshairWorldLocation + CrosshairWorldRotation * InTraceDistance;
//
// #if WITH_EDITOR
// 			DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Green, false, 3.0f, 0, 5.0f);
// #endif
// 			
// 			return GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStartLocation, TraceEndLocation, InTraceChannel);
// 		}
// 	}
// 	return false;
// }

void ARTSCharacterPlayer::SetLeaderPoseComponent()
{
	if (USkeletalMeshComponent* Body = GetMesh())
	{
		ArmMesh->SetLeaderPoseComponent(Body);
		LegMesh->SetLeaderPoseComponent(Body);
		HeadMesh->SetLeaderPoseComponent(Body);
		BackMesh->SetLeaderPoseComponent(Body);
		WeaponMesh->SetLeaderPoseComponent(Body);
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

void ARTSCharacterPlayer::Input_WeaponSwap(uint8 WeaponIndex)
{
	if ((uint8)CurrentWeaponType - 1 == WeaponIndex)
	{
		return;
	}
	
	FGameplayEventData PayLoadData;
	PayLoadData.Target = this;
	PayLoadData.EventMagnitude = WeaponIndex;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, WeaponSwapInputTag, PayLoadData);
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

void ARTSCharacterPlayer::SetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ARTSCharacterPlayer::SetRunSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ARTSCharacterPlayer::StartAiming()
{
	UseControlRotation();
	SetWalkSpeed();
	SpringArmComponent->bEnableCameraLag = false;

	AimingTimeline.Play();
}

void ARTSCharacterPlayer::StopAiming()
{
	UseMovementRotation();
	SetRunSpeed();
	SpringArmComponent->bEnableCameraLag = true;

	AimingTimeline.Reverse();
}

FVector ARTSCharacterPlayer::GetMuzzleSocketLocation() const
{
	if (CurrentWeaponContext)
	{
		const FVector MuzzleSocketLocation = GetMesh()->GetSocketLocation(CurrentWeaponContext->MuzzleSocketName);
		return MuzzleSocketLocation;
	}

	return GetActorLocation();
}

void ARTSCharacterPlayer::SpawnFireEffectActor(TArray<FVector>& InImpactPositions, TArray<FVector>& InImpactNormals)
{
	UWorld* World = GetWorld();
	if (World && World->IsNetMode(ENetMode::NM_DedicatedServer))
	{
		return;
	}

	if (!CurrentWeaponContext)
	{
		return;
	}
	
	TArray<FVector> ImpactPositions = InImpactPositions;
	TArray<FVector> ImpactNormals = InImpactNormals;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), false, this);

	// Fake추가 여부 검사.
	if (CurrentWeaponContext->bNeedFakeData)
	{
		// Fake 추가.
		const FVector FirstImpactPosition = ImpactPositions[0];
		const FVector SocketLocation = GetMuzzleSocketLocation();
		FVector TraceDir = FirstImpactPosition - SocketLocation;
		TraceDir.Normalize();
		
		for (uint8 ix = 0; ix < CurrentWeaponContext->NumberOfFakeData; ++ix)
		{
			FVector FakeTracerDir = VRandConeNormalDistribution(TraceDir, 5.0f, 0.5f);
			
			const FVector FakeTraceEnd = FakeTracerDir * 50000.0f + SocketLocation;

			FHitResult HitResult;
			World->LineTraceSingleByChannel(HitResult, SocketLocation, FakeTraceEnd, RTS_TraceChannel_PlayerTraceEnemyMesh, TraceParams);

			if (HitResult.bBlockingHit)
			{
				ImpactPositions.Add(HitResult.Location);
				ImpactNormals.Add(HitResult.Normal);
			}
			else
			{
				ImpactPositions.Add(FakeTraceEnd);
				ImpactNormals.Add(FVector::ZeroVector);
			}
		}
	}

	// FireEffectActor 생성 (3초마다 사용안했으면 재생성).
	if (!IsValid(FireEffectActor))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		TSubclassOf<AFireEffect> FireEffectActorClass = CurrentWeaponContext->FireEffectActorClass;
		FireEffectActor = World->SpawnActor<AFireEffect>(FireEffectActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (FireEffectActor)
		{
			FireEffectActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
			
			// Niagara 데이터 넘기기.
			FireEffectActor->MuzzleNiagaraSystem = CurrentWeaponContext->MuzzleNiagara;
			FireEffectActor->TracerNiagaraSystem = CurrentWeaponContext->TracerNiagara;
			FireEffectActor->ImpactNiagaraSystem = CurrentWeaponContext->ImpactNiagara;
		}
	}

	if (IsValid(FireEffectActor))
	{
		// MuzzlePosition 넘기기.
		FireEffectActor->MuzzlePosition = GetMuzzleSocketLocation();
		// Impact 데이터 넘기기.
		FireEffectActor->ImpactPositions = ImpactPositions;
		FireEffectActor->ImpactNormals = ImpactNormals;

		// 실행.
		FireEffectActor->Fire();
	}
	
}

FVector ARTSCharacterPlayer::VRandConeNormalDistribution(const FVector& Direction, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.0f)
	{
		// 0~1 의 랜덤값에 ^ Exponent.
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleRad;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Direction.Rotation();
		FQuat DirectionQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f, 0.0f, AngleAround));
		FQuat FinalDirectionQuat = DirectionQuat * AroundQuat * FromCenterQuat;
		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}

	return Direction.GetSafeNormal();
}

void ARTSCharacterPlayer::StartFireRecoil()
{
	if (!IsLocallyControlled())
	{
		return;
	}
	
	if (!CurrentWeaponContext)
	{
		return;
	}

	FFireRecoil FireRecoil = CurrentWeaponContext->FireRecoil;

	float AddYaw = FMath::FRandRange(FireRecoil.YawMin, FireRecoil.YawMax);
	float AddPitch = FMath::FRandRange(FireRecoil.PitchMin, FireRecoil.PitchMax);
	float DeltaTime = GetWorld()->DeltaTimeSeconds;

	RecoilAddYaw = AddYaw * DeltaTime;
	RecoilAddPitch = AddPitch * DeltaTime;

	RecoilTimeline.PlayFromStart();
}

void ARTSCharacterPlayer::RegisterAbilitySet()
{
	if (AbilitySet && RTSASC)
	{
		AbilitySet->GiveToAbilitySystem(RTSASC);
	}
}

void ARTSCharacterPlayer::WeaponSwapAbilitySuccess(uint8 InWeaponIndex)
{
	check(WeaponContexts.Contains(InWeaponIndex));
	check(RTSASC);
	if (!IsLocallyControlled())
	{
		return;
	}
	
	URTSWeaponContext* NewWeaponContext = WeaponContexts[InWeaponIndex];
	if (NewWeaponContext == nullptr)
	{
		RTS_LOG(LogRTS, Log, TEXT("NewWeaponContext(Index = %d) is nullptr"), InWeaponIndex);
		return;
	}
	if (NewWeaponContext == CurrentWeaponContext)
	{
		RTS_LOG(LogRTS, Log, TEXT("NewWeaponContext == CurrentWeaponContext"));
		return;
	}

	// CurrentWeaponContext의 GA제거.
	if (CurrentWeaponContext)
	{
		FGameplayAbilitySpec* CurrentAbilitySpec = RTSASC->FindAbilitySpecFromClass(CurrentWeaponContext->WeaponGA);
		if (CurrentAbilitySpec)
		{
			if (CurrentAbilitySpec->IsActive())
			{
				RTSASC->CancelAbilityHandle(CurrentAbilitySpec->Handle);
			}
		
			RTSASC->ClearAbility(CurrentAbilitySpec->Handle);
		}
	}

	// NewWeaponContext의 GA부여.
	FGameplayAbilitySpec NewAbilitySpec(NewWeaponContext->WeaponGA);
	NewAbilitySpec.DynamicAbilityTags.AddTag(NewWeaponContext->WeaponInputTag);
	RTSASC->GiveAbility(NewAbilitySpec);	

	// Weapon SkeletalMesh 변경.
	if (NewWeaponContext->WeaponMesh)
	{
		WeaponMesh->SetSkeletalMesh(NewWeaponContext->WeaponMesh);
	}

	// LeaderPose 다시 설정.
	SetLeaderPoseComponent();

	// 처리 끝났으면 CurrentWeapon 갱신.
	CurrentWeaponContext = NewWeaponContext;

	// 무기 Enum 변경.
	CurrentWeaponType = NewWeaponContext->WeaponType;
	OnWeaponTypeChanged.Broadcast(CurrentWeaponType);

	DefaultCameraPosition = CurrentWeaponContext->DefaultCameraPosition;
	DefaultSpringArmLength = CurrentWeaponContext->DefaultSpringArmLength;
	DefaultFOV = CurrentWeaponContext->DefaultFOV;

	AimingCameraPosition = CurrentWeaponContext->AimingCameraPosition;
	AimingSpringArmLength = CurrentWeaponContext->AimingSpringArmLength;
	AimingFOV = CurrentWeaponContext->AimingFOV;

	// RTSAmmoSet 세팅.
	RTSASC->SetNumericAttributeBase(URTSAmmoSet::GetMaxAmmoAttribute(), CurrentWeaponContext->MaxAmmo);
	RTSASC->SetNumericAttributeBase(URTSAmmoSet::GetCurrentAmmoAttribute(), CurrentWeaponContext->MaxAmmo);
}

void ARTSCharacterPlayer::OnRep_CurrentWeaponType()
{
	uint8 CurrentWeaponTypeIndex = static_cast<uint8>(CurrentWeaponType) - 1;
	if(!WeaponContexts.Contains(CurrentWeaponTypeIndex))
	{
		return;
	}
	
	URTSWeaponContext* NewWeaponContext = WeaponContexts[CurrentWeaponTypeIndex];
	if (!NewWeaponContext)
	{
		return;
	}

	// Mesh 바꾸기.
	// Weapon SkeletalMesh 변경.
	if (NewWeaponContext->WeaponMesh)
	{
		WeaponMesh->SetSkeletalMesh(NewWeaponContext->WeaponMesh);
	}

	// LeaderPose 다시 설정.
	SetLeaderPoseComponent();

	// 처리 끝났으면 CurrentWeapon 갱신.
	CurrentWeaponContext = NewWeaponContext;
}

URTSWeaponContext* ARTSCharacterPlayer::GetWeaponContextByIndex(uint8 Index) const
{
	if (WeaponContexts.Contains(Index))
	{
		return WeaponContexts[Index];
	}

	return nullptr;
}

void ARTSCharacterPlayer::OnAmmoChangedCallback(float InCurrentAmmo, float InMaxAmmo)
{
	if (IsLocallyControlled() && InCurrentAmmo == 0.0f)
	{
		// 탄창이 1 -> 0이 될 때 CommitAbility는 통과됨, 그 순간에 Reload 실행 -> GA_Fire는 중간에 안끊기고 계속 실행됨 -> 결과적으로 Tag 설정등 되어있으나 이미 GA_Fire의 ActivateAbility내부이므로 GA_Fire 계속 실행됨.
		// GA_Fire의 CommitAbility의 순서를 뒤로 미루거나, Reload자체를 다음 틱에 실행하도록 하는 방법 이렇게 2가지가 있는데.
		// CommitAbility의 순서변경은 가독성도 안좋고 GA_Fire을 비정상적으로 캔슬시키는 것과 다름 없으므로.
		// 구조를 바꾸지 않는 이상 다음 Tick에 실행하는것이 올바르다고 판단했다.
		GetWorldTimerManager().SetTimerForNextTick([this]()
		{
			FGameplayEventData PayLoadData;
			PayLoadData.Target = this;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, ReloadInputTag, PayLoadData);
		});
		
		// @Study: ExecuteOnGameThread.
		// Ticker? 를 사용하는건데 공부해두면 좋을 것 같다.
	}
}

void ARTSCharacterPlayer::AimingUpdate(float Alpha) const
{
	float CurrentSpringArmLength = FMath::Lerp(DefaultSpringArmLength, AimingSpringArmLength, Alpha);
	FVector CurrentCameraPosition = FMath::Lerp(DefaultCameraPosition, AimingCameraPosition, Alpha);
	float CurrentFOV = FMath::Lerp(DefaultFOV, AimingFOV, Alpha);

	SpringArmComponent->TargetArmLength = CurrentSpringArmLength;
	CameraComponent->SetRelativeLocation(CurrentCameraPosition);
	CameraComponent->FieldOfView = CurrentFOV;
}

void ARTSCharacterPlayer::RecoilUpdate(float Alpha)
{
	AddControllerYawInput(RecoilAddYaw);
	AddControllerPitchInput(RecoilAddPitch);
}

void ARTSCharacterPlayer::RecoilFinish()
{
	RecoilAddYaw = 0.0f;
	RecoilAddPitch = 0.0f;
}
