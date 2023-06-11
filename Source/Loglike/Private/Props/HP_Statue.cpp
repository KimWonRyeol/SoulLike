// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/HP_Statue.h"
#include <GameFramework/LoglikeGameInstance.h>
#include <Character/LoglikeCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <DungeonGameMode.h>

// Sets default values
AHP_Statue::AHP_Statue()
{
	
}

void AHP_Statue::BeginPlay()
{
	Super::BeginPlay();
	//SetActorEnableCollision(false);
	ULoglikeGameInstance* GameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (int8(GameInstance->CurrentStageType) != 2)
	{
		Destroy();
		return;
	}


	ADungeonGameMode* GameMode = Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnStageClear.AddDynamic(this, &AHP_Statue::SetCollisionOn);
}

void AHP_Statue::SetCollisionOn()
{
	SetActorEnableCollision(true);
}

void AHP_Statue::HP_UP()
{
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	ABGameInstance->SetCurrentHealth(0.3);
	PlayerCharacter->SetUpdateStat();
}



