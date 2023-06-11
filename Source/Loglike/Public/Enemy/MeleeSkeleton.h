// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeMonsterBase.h"
#include "MeleeSkeleton.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API AMeleeSkeleton : public AMeleeMonsterBase
{
	GENERATED_BODY()
public:
	AMeleeSkeleton();
	virtual void BeginPlay() override;
	virtual void AttackStart() override;
	virtual void AttackEnd() override;
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Sword", Meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Sword;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Sword", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* SwordCollision;
};
