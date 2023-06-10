// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeInOutWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UFadeInOutWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void PlayFadeIn();
	UFUNCTION()
		void PlayFadeOut();

public:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* FadeIn;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* FadeOut;
};
