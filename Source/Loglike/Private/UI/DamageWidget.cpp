// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageWidget.h"

#include "Components/TextBlock.h"

void UDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UDamageWidget::SetDamageNumberText(int32 NewNum)
{
	if(DamageNumberText == nullptr) return;
	FText NewText = FText::FromString(FString::FromInt(NewNum));
	DamageNumberText->SetText(NewText);
	
}
