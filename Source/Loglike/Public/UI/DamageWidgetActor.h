// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageWidgetActor.generated.h"

UCLASS()
class LOGLIKE_API ADamageWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageWidgetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetDamageValue(int32 DamageValue);
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class UWidgetComponent* DamageWidget;
};
