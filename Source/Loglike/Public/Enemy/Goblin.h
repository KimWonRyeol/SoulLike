// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeMonsterBase.h"
#include "Goblin.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API AGoblin : public AMeleeMonsterBase
{
	GENERATED_BODY()
public:
	AGoblin();
	virtual void BeginPlay() override;
	virtual void AttackStart() override;
	virtual void AttackEnd() override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Fist", Meta = (AllowPrivateAccess = true))
		class UBoxComponent* LeftBox;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Fist", Meta = (AllowPrivateAccess = true))
		class UBoxComponent* RightBox;

};
