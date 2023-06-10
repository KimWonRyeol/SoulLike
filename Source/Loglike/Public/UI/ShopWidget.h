// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include <Character/ABCharacterStatComponent.h>
#include "GameFramework/LoglikeGameInstance.h"
#include "ShopWidget.generated.h"


/**
 *
 */
UCLASS()
class LOGLIKE_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UShopWidget(const FObjectInitializer& ObjectInitializer);

	ULoglikeGameInstance* ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quit")
	class UButton* Quit;

	UFUNCTION(BlueprintCallable)
		void BuyArtifactKey();
	UFUNCTION(BlueprintCallable)
		void BuyWeaponKey();

	UFUNCTION(BlueprintCallable)
		void BuyHpStat();
	UFUNCTION(BlueprintCallable)
		void BuyDamageStat();
	UFUNCTION(BlueprintCallable)
		void BuyLuckStat();

	UPROPERTY(BlueprintReadOnly)
	FString CPPWeaponKeyText;

	UPROPERTY(BlueprintReadOnly)
	FString CPPArtifactKeyText;

	UPROPERTY(BlueprintReadOnly)
	FString CPPHpStatText;

	UPROPERTY(BlueprintReadOnly)
	FString CPPDamageStatText;

	UPROPERTY(BlueprintReadOnly)
	FString CPPLuckStatText;

	UPROPERTY(BlueprintReadOnly)
	FString CPPTokenText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundBase* BuySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundBase* DoorSound;
};




