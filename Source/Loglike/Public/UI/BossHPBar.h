// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPBar.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBossHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdateHPWidget(float CurrentNum);

private:
	UPROPERTY()
	class ABoss* SpawnedBoss;

	UPROPERTY()
	class UProgressBar* HPProgressBar;

	UPROPERTY()
	class UTextBlock* CurrentText;

	UPROPERTY()
	class UTextBlock* MaxText;

	UPROPERTY()
	float CurrentHitPoint;
	float MaxNum = 500;
};
