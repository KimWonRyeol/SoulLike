// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABPlayerState.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "GameFramework/ABSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ABCharacterStatComponent.h"

AABPlayerState::AABPlayerState()
{
	//저장할 디폴트 데이터 선언할 것!!!!!!!!!!!!
	MaxHealth = 100.0f;
	DefaultDamage = 0.0f;
	DefaultLuck = 0.0f;
	DefaultAttackSpeed = 1.0;
	Token = 0;
	SaveSlotName = TEXT("Player1");
	
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
}

float AABPlayerState::GetMaxHealth()
{
	return MaxHealth;
}

float AABPlayerState::GetDefaultDamage()
{
	return DefaultDamage;
}

float AABPlayerState::GetDefaultLuck()
{
	return DefaultLuck;
}

float AABPlayerState::GetDefaultAttackSpeed()
{
	return DefaultAttackSpeed;
}

int32 AABPlayerState::GetToken()
{
	return Token;
}

void AABPlayerState::InitPlayerData()
{
	UABSaveGame* ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));
	
	ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == ABSaveGame) 
		ABSaveGame = GetMutableDefault<UABSaveGame>();

	//SetCharacterLevel(1);

	MaxHealth = ABSaveGame->MaxHealth;
	CharacterStat->SetMaxHealth(MaxHealth);

	DefaultDamage = ABSaveGame->DefaultDamage;
	CharacterStat->SetDefaultDamage(DefaultDamage);

	DefaultLuck = ABSaveGame->DefaultLuck;
	CharacterStat->SetDefaultLuck(DefaultLuck);

	DefaultAttackSpeed = ABSaveGame->DefaultAttackSpeed;
	CharacterStat->SetDefaultAttackSpeed(DefaultAttackSpeed);

	Token = ABSaveGame->Token;
	CharacterStat->SetDefaultToken(Token);
	//SavePlayerData();
}

void AABPlayerState::SavePlayerData()
{
	UABSaveGame* NewPlayerData = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));

	NewPlayerData->MaxHealth = CharacterStat->GetMaxHealth();
	NewPlayerData->DefaultDamage = CharacterStat->GetDefaultDamage();
	NewPlayerData->DefaultLuck = CharacterStat->GetDefaultLuck();
	NewPlayerData->DefaultAttackSpeed = CharacterStat->GetDefaultAttackSpeed();
	NewPlayerData->Token = CharacterStat->GetToken();
	
	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Savefile Load Error!!!!"));
	}
}

//void AABPlayerState::SetCharacterLevel(int32 NewName)
//{
//	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
//	CurrentStatData = ABGameInstance->GetABCharacterData(NewName);
//}
