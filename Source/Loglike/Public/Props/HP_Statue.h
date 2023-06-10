// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HP_Statue.generated.h"

UCLASS()
class LOGLIKE_API AHP_Statue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHP_Statue();
	virtual void BeginPlay() override;
	UFUNCTION()
	void SetCollisionOn();

	UFUNCTION(BlueprintCallable, Category = "Stat Statue")
	void HP_UP();

};
