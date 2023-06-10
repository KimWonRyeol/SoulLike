// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossHPBar.h"
#include "Enemy/Boss.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBossHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	CurrentText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHP")));
	MaxText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHP")));

	if (MaxText != nullptr)
	{
		MaxText->SetText(FText::AsNumber(MaxNum));
	}
	if (CurrentText != nullptr)
	{
		CurrentText->SetText(FText::AsNumber(MaxNum));
	}
}

void UBossHPBar::UpdateHPWidget(float CurrentNum)
{
	HPProgressBar->SetPercent(CurrentNum / MaxNum);
	CurrentText->SetText(FText::AsNumber(CurrentNum));
}
