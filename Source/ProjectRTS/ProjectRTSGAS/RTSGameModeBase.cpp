// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameModeBase.h"

#include "Player/RTSPlayerState.h"

ARTSGameModeBase::ARTSGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/ProjectRTSGAS/Character/BP_RTSPlayer.BP_RTSPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/ProjectRTSGAS/Player/BP_RTSPlayerController.BP_RTSPlayerController_C'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	PlayerStateClass = ARTSPlayerState::StaticClass();
}

void ARTSGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* ARTSGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	
	return NewPlayerController;
}

void ARTSGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ARTSGameModeBase::StartPlay()
{
	Super::StartPlay();
}
