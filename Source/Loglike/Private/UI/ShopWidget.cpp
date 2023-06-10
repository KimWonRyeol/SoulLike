// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/ShopWidget.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include <Character/LoglikeCharacter.h>
#include "Sound/SoundBase.h"




UShopWidget::UShopWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	if (ABGameInstance != nullptr)
	{
		ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		UABCharacterStatComponent* Stat = PlayerCharacter->CharacterStat;

		CPPWeaponKeyText = FString::Printf(TEXT("Stock: %d / 1"), (Stat->GetWeaponKeyRemain()));
		CPPArtifactKeyText = FString::Printf(TEXT("Stock: %d / 1"), (Stat->GetArtifactKeyRemain()));
		CPPHpStatText = FString::Printf(TEXT("Stock: %d / 10"), (Stat->GetHpStatRemain()));
		CPPDamageStatText = FString::Printf(TEXT("Stock: %d / 10"), (Stat->GetDamageStatRemain()));
		CPPLuckStatText = FString::Printf(TEXT("Stock: %d / 10"), (Stat->GetLuckStatRemain()));
		CPPTokenText = FString::Printf(TEXT("%d"), (Stat->GetToken()));

		// BuySound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/RPG_Skill_SFX/Wav/Additional_SFX/Item_Coin_01_Metal_Short_crispy_03.Item_Coin_01_Metal_Short_crispy_03'"));
		// DoorSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/RPG_Skill_SFX/Wav/Additional_SFX/Door_Stonel_Gate_01_Open_Slow_Heavy_Debris_01.Door_Stonel_Gate_01_Open_Slow_Heavy_Debris_01"));
	}
}

void UShopWidget::BuyArtifactKey()
{
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UABCharacterStatComponent* Stat = PlayerCharacter->CharacterStat;

	//SaveFile에서 Remain 가져오기
	int32 remain = Stat->GetArtifactKeyRemain();

	//재고 없을 때
	if (remain < 1)
	{
		printf("you can not buy");
		return;
	}
	//토큰 없을 때
	if (PlayerCharacter->CharacterStat->GetToken() < 50)
	{
		UE_LOG(LogTemp, Log, TEXT("You Can Not Buy"));
		return;
	}

	Stat->SetArtifactKey(true);
	CPPArtifactKeyText = FString::Printf(TEXT("Stock: %d / 1"), --(remain));
	Stat->SetArtifactKeyRemain(remain);
	PlayerCharacter->CharacterStat->AddToken_(-50);

	
	if (BuySound)
	{
		UUserWidget::PlaySound(BuySound);
	}

	//Token TEXT UPDATE
	CPPTokenText = FString::Printf(TEXT("%d"), (Stat->GetToken()));

	//문 무너지는 소리 재생
	if (DoorSound)
	{
		UUserWidget::PlaySound(DoorSound);
	}

	PlayerCharacter->SetUpdateStat();
}

void UShopWidget::BuyWeaponKey()
{
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UABCharacterStatComponent* Stat = PlayerCharacter->CharacterStat;

	//SaveFile에서 Remain 가져오기
	int32 remain = Stat->GetWeaponKeyRemain();

	//재고 없을 때
	if (remain < 1)
	{
		printf("you can not buy");
		return;
	}
	//토큰 없을 때
	if (PlayerCharacter->CharacterStat->GetToken() < 50)
	{
		UE_LOG(LogTemp, Log, TEXT("You Can Not Buy"));
		return;
	}

	//열쇠 구매 세이브 파일 적용
	Stat->SetWeaponKey(true);
	CPPWeaponKeyText = FString::Printf(TEXT("Stock: %d / 1"), --(remain));
	Stat->SetWeaponKeyRemain(remain);
	PlayerCharacter->CharacterStat->AddToken_(-50);

	if (BuySound)
	{
		UUserWidget::PlaySound(BuySound);
	}

	//Token TEXT UPDATE
	CPPTokenText = FString::Printf(TEXT("%d"), (Stat->GetToken()));

	//문 무너지는 소리 재생

	if (DoorSound)
	{
		UUserWidget::PlaySound(DoorSound);
	}

	//HUD UPDATE
	PlayerCharacter->SetUpdateStat();
}

void UShopWidget::BuyHpStat()
{
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UABCharacterStatComponent* Stat = PlayerCharacter->CharacterStat;

	//SaveFile에서 Remain 가져오기
	int32 remain = Stat->GetHpStatRemain();

	//재고 없을 때
	if (remain < 1)
	{
		printf("you can not buy");
		return;
	}
	//토큰 없을 때
	if (PlayerCharacter->CharacterStat->GetToken() < 10)
	{
		UE_LOG(LogTemp, Log, TEXT("You Can Not Buy"));
		return;
	}

	//BUY
	CPPHpStatText = FString::Printf(TEXT("Stock: %d / 10"), --(remain));
	Stat->AddHP_(1);
	Stat->SetHpStatRemain(remain);
	PlayerCharacter->CharacterStat->AddToken_(-10);

	if (BuySound)
	{
		UUserWidget::PlaySound(BuySound);
	}

	//Token TEXT UPDATE
	CPPTokenText = FString::Printf(TEXT("%d"), (Stat->GetToken()));

	ABGameInstance->CurrentHealth = 100.0f + Stat->GetMaxHealth() * 5;

	//HUD UPDATE
	PlayerCharacter->SetUpdateStat();
}

void UShopWidget::BuyDamageStat()
{
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UABCharacterStatComponent* Stat = PlayerCharacter->CharacterStat;

	//SaveFile에서 Remain 가져오기
	int32 remain = Stat->GetDamageStatRemain();

	//재고 없을 때
	if (remain < 1)
	{
		printf("you can not buy");
		return;
	}
	//토큰 없을 때
	if (PlayerCharacter->CharacterStat->GetToken() < 10)
	{
		UE_LOG(LogTemp, Log, TEXT("You Can Not Buy"));
		return;
	}

	//BUY
	CPPDamageStatText = FString::Printf(TEXT("Stock: %d / 10"), --remain);
	Stat->AddDamage_(1);
	Stat->SetDamageStatRemain(remain);
	PlayerCharacter->CharacterStat->AddToken_(-10);

	if (BuySound)
	{
		UUserWidget::PlaySound(BuySound);
	}

	//Token TEXT UPDATE
	CPPTokenText = FString::Printf(TEXT("%d"), (Stat->GetToken()));

	//HUD UPDATE
	PlayerCharacter->SetUpdateStat();
}

void UShopWidget::BuyLuckStat()
{
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UABCharacterStatComponent* Stat = PlayerCharacter->CharacterStat;

	//SaveFile에서 Remain 가져오기
	int32 remain = Stat->GetLuckStatRemain();

	//재고 없을 때
	if (remain < 1)
	{
		printf("you can not buy");
		return;
	}
	//토큰 없을 때
	if (PlayerCharacter->CharacterStat->GetToken() < 10)
	{
		UE_LOG(LogTemp, Log, TEXT("You Can Not Buy"));
		return;
	}
	
	//BUY
	CPPLuckStatText = FString::Printf(TEXT("Stock: %d / 10"), --remain);
	Stat->AddLuck_(1);
	Stat->SetLuckStatRemain(remain);
	PlayerCharacter->CharacterStat->AddToken_(-10);

	if (BuySound)
	{
		UUserWidget::PlaySound(BuySound);
	}

	//Token TEXT UPDATE
	CPPTokenText = FString::Printf(TEXT("%d"), (Stat->GetToken()));

	//HUD UPDATE
	PlayerCharacter->SetUpdateStat();
}
