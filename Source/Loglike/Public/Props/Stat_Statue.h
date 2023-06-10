// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stat_Statue.generated.h"

UCLASS()
class LOGLIKE_API AStat_Statue : public AActor
{
	GENERATED_BODY()
	
public:	
	AStat_Statue();
	virtual void BeginPlay() override;
	UFUNCTION()
	void SetCollisionOn();

	UFUNCTION(BlueprintCallable, Category = "Stat Statue")
		void Stat_UP();
};
