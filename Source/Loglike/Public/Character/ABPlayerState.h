// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

UCLASS()
class LOGLIKE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AABPlayerState();
	float GetMaxHealth();
	float GetDefaultDamage();
	float GetDefaultLuck();
	float GetDefaultAttackSpeed();
	int32 GetToken();

	void InitPlayerData();
	void SavePlayerData();

	FString SaveSlotName;
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

private:
	//void SetCharacterLevel(int32 NewName);
	struct FABCharacterData* CurrentStatData;

protected:
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float DefaultDamage;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float DefaultLuck;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float DefaultAttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 Token;

	UPROPERTY(VIsibleAnywhere, Category = "Stat")
	class UABCharacterStatComponent* CharacterStat;
	
};
