// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "MeleeMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API AMeleeMonsterBase : public AMonsterBase
{
	GENERATED_BODY()
public:
	AMeleeMonsterBase();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	/**근거리몬스터의 근접공격을 담당하는 함수*/
	void ComboAttack();
	/**PivotActor To 몬스터 방향으로 튕겨져 나가는 함수*/
	void PushBack(AActor* PivotActor);
	/**스턴상태의 montage를 실행하고 BT의 상태를 Stun으로 돌림*/
	void Stun();
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool GetIsAttacking() { return IsAttacking; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UTextRenderComponent* TextRenderComponent;
private:
	int32 ComboNum;
	bool IsAttacking;
};
