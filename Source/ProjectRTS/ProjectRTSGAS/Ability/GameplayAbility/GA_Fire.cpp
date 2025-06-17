// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Fire.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayCueFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ProjectRTS/Physics/RTSCollisionChannel.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

UGA_Fire::UGA_Fire(const FObjectInitializer& ObjectInitializer)
{

}

bool UGA_Fire::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	if (IsLocallyControlled())
	{
		StartFiring();
	}
	
	// Bind TargetData Callback.
	OnTargetDataReadyCallbackDelegateHandle = ASC->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(this, &UGA_Fire::OnTargetDataReadyCallback);

	// Montage Task.
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, FireMontage, 1.0f, NAME_None);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Fire::FireComplete);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_Fire::FireComplete);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGA_Fire::FireComplete);
	PlayMontageAndWait->ReadyForActivation();

	// Fire Delay Timer.
	FTimerHandle FireTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UGA_Fire::FireComplete, FireDelayTime, false);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Fire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		// SCopeLockCount는 활성화 된 예측 창의 갯수라고 한다.
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
			return;
		}

		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
		check(ASC);

		// 델리게이트 제거.
		ASC->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).Remove(OnTargetDataReadyCallbackDelegateHandle);

		// TargetData 소모.
		ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
		
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UGA_Fire::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo();
	if (RTSCharacter)
	{
		RTSCharacter->UseControlRotation();
		RTSCharacter->SetWalkSpeed();
	}
}

void UGA_Fire::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo();
	URTSAbilitySystemComponent* ASC = GetRTSAbilitySystemComponentFromActorInfo();

	
	if (RTSCharacter && ASC)
	{
		if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Character.State.Aiming")))))
		{
			RTSCharacter->UseMovementRotation();
			RTSCharacter->SetRunSpeed();
		}
	}
}

void UGA_Fire::StartFiring()
{
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	AController* Controller = GetControllerFromActorInfo();
	check(Controller);

	// WeaponStateComponent는 HitMarker용 이므로 생략.

	// 예측키는 서버에서 클라이언트의 GA실행 취소 시 해당 키로 관련된 모든 것들을 취소한다고 한다.
	FScopedPredictionWindow ScopedPrediction(ASC, CurrentActivationInfo.GetActivationPredictionKey());

	// FoundHits 채우는 로직.
	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);
	
	// TargetDataHandle에 TargetData_SingleTargetHit 채워넣기.
	FGameplayAbilityTargetDataHandle TargetData;
	if (FoundHits.Num() > 0)
	{
		for (const FHitResult& FoundHit : FoundHits)
		{
			FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;

			TargetData.Add(NewTargetData);
		}
	}

	// TargetData 처리.
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}

void UGA_Fire::PerformLocalTargeting(OUT TArray<FHitResult>& OutHits)
{
	APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	
	if (AvatarPawn && AvatarPawn->IsLocallyControlled())
	{
		// Aim시작 위치와 방향 구하는 로직.
		FFiringInput InputData;
		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn);
		
		InputData.AimDirection = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.TraceStart = TargetTransform.GetTranslation();
		
		InputData.TraceEnd = InputData.TraceStart + InputData.AimDirection * MaxRange;

		// 한 탄창의 총알 수에 해당하는 Trace 진행.
		TraceBulletsInCartridge(InputData, OutHits);
	}
}

FTransform UGA_Fire::GetTargetingTransform(APawn* SourcePawn)
{
	check(SourcePawn);

	AController* Controller = SourcePawn->GetController();
	const FVector ActorLocation = SourcePawn->GetActorLocation();
	double FocalDistance = 1024.0f;
	FVector FocalLocation;
	FVector CameraLocation;
	FRotator CameraRotation;
	
	if (Controller)
	{
		// 카메라 위치, 회전 구하기.
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC)
		{
			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		}

		FVector AimDirection = CameraRotation.Vector().GetSafeNormal();
		FocalLocation = CameraLocation + (AimDirection * FocalDistance);

		if (PC)
		{
			// CameraLocation을 Pawn에 가깝게 보정.
			CameraLocation = FocalLocation + ((ActorLocation - FocalLocation) | AimDirection) * AimDirection;
		}
	}

	return FTransform(CameraRotation, CameraLocation);
}

void UGA_Fire::TraceBulletsInCartridge(const FFiringInput& InputData, TArray<FHitResult>& OutHits)
{
	// 한 탄약통에 든 총알 갯수만큼 for문 반복 ( 샷건의 경우 여러개, 나머지는 1개 ).
	for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
	{
		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(SpreadAngle * 0.5f);

		// 탄 퍼짐 적용.
		const FVector BulletDir = VRandConeNormalDistribution(InputData.AimDirection, HalfSpreadAngleInRadians, SpreadExponent);
		const FVector EndTrace = InputData.TraceStart + (BulletDir * MaxRange);

		// Trace 진행.
		FHitResult Impact = DoSingleBulletTrace(InputData.TraceStart, EndTrace, BulletTraceSweepRadius);

		// 결과가 Blocking이 아니면.
		if (!Impact.bBlockingHit)
		{
			Impact.TraceStart = GetMuzzleLocation();
			Impact.Location = EndTrace;
			Impact.ImpactPoint = EndTrace;
		}

		OutHits.Add(Impact);
	}
}

FHitResult UGA_Fire::DoSingleBulletTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), false, GetAvatarActorFromActorInfo());

	// 1차 / LineTraceSingleByChannel ( TraceStart to TraceEnd ).
	FHitResult FirstImpact;
	bool bFirstImpactHit = GetWorld()->LineTraceSingleByChannel(FirstImpact, TraceStart, TraceEnd, RTS_TraceChannel_PlayerTraceEnemyMesh, TraceParams);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bFirstImpactHit ? FColor::Green : FColor::Red, false, 5.0f, 0, 1.0f);

	if (bFirstImpactHit)
	{
		// 2차 / SweepSingleByChannel ( SocketLocation(Muzzle) to FirstImpact.ImpactPoint ).
		FHitResult FinalImpact;
		const FVector MuzzleLocation = GetMuzzleLocation();
		FVector SweepEnd = MuzzleLocation + ((FirstImpact.ImpactPoint - MuzzleLocation).GetSafeNormal()) * FirstImpact.Distance * 1.1f;
		bool bFinalImpactHit = GetWorld()->SweepSingleByChannel(FinalImpact, MuzzleLocation, SweepEnd, FQuat::Identity, RTS_TraceChannel_PlayerTraceEnemyMesh, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
		if (bDrawDebug)
		{
			DrawDebugLine(GetWorld(), MuzzleLocation, SweepEnd, bFinalImpactHit ? FColor::Green : FColor::Red, false, 5.0f, 0, 1.0f);
		}

		if (bFinalImpactHit)
		{
			return FinalImpact;
		}
	}
	else
	{
		return FirstImpact;
	}

	return FirstImpact;
}

FVector UGA_Fire::VRandConeNormalDistribution(const FVector& Direction, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.0f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		// 0~1 의 랜덤값에 ^ Exponent.
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
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

const FVector UGA_Fire::GetMuzzleLocation()
{
	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterFromActorInfo();
	FVector SocketLocation;
	
	if (RTSCharacter)
	{
		SocketLocation = RTSCharacter->GetMuzzleSocketLocation();
	}
	else
	{
		AActor* AvatarActor = GetAvatarActorFromActorInfo();
		SocketLocation = AvatarActor->GetActorLocation();
	}

	return SocketLocation;
}

void UGA_Fire::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	if (const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FScopedPredictionWindow ScopedPrediction(ASC);

		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority();
		if (bShouldNotifyServer)
		{
			ASC->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), LocalTargetDataHandle, ApplicationTag, ASC->ScopedPredictionKey);
		}

		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			// TargetData로 GE, GC 처리하기.
			OnTargetDataReady(LocalTargetDataHandle);
		}
	}
}

void UGA_Fire::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (!IsLocallyControlled())
	{
		return;
	}
	
	URTSAbilitySystemComponent* ASC = GetRTSAbilitySystemComponentFromActorInfo();
	check(ASC);

	// GC_Fire 호출.
	FHitResult FirstHitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);
	FGameplayCueParameters CueParameters = UGameplayCueFunctionLibrary::MakeGameplayCueParametersFromHitResult(FirstHitResult);
	ASC->ExecuteGameplayCue(GameplayCue_Fire, CueParameters);

	// Damage GE처리(TargetData에 담긴 Data를 한번에 처리).
	if (HasAuthority(&CurrentActivationInfo) && DamageEffectClass)
	{
		BP_ApplyGameplayEffectToTarget(TargetData, DamageEffectClass, 1, 1);
	}
	
	/*
	// Bullet Tracer, Bullet Impact 처리.
	uint8 TargetDataSize = TargetData.Num();
	
	for (uint8 ix = 0; ix < TargetDataSize; ++ix)
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, ix);
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddHitResult(HitResult);	
		FGameplayCueParameters Params;
		Params.EffectContext = EffectContext;
		
		// @Todo: GameplayCue 최적화.
		ASC->ExecuteGameplayCue(GameplayCue_BulletTracer, Params);
		ASC->ExecuteGameplayCue(GameplayCue_BulletImpact, Params);
	
		// @Todo: GameplayCue NetMulticast 해결.
	}
	*/
	
}

void UGA_Fire::FireComplete()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
