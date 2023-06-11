// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void SetDamageNumberText(int32 NewNum);
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Damage",Meta = (BindWidget))
	class UTextBlock* DamageNumberText;
};
