// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class LOGLIKE_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();
	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION() /**공격을 시작할 때 동작*/
		virtual void AttackStart() PURE_VIRTUAL(AMonsterBase::AttackStart, );
	UFUNCTION() /**공격을 마칠 때 동작*/
		virtual void AttackEnd() PURE_VIRTUAL(AMonsterBase::AttackEnd, );
	UFUNCTION()/**공격 실행*/
		virtual void Attack();
	float GetAttackSpeed() { return AttackSpeed; };
	/**해당 몬스터를 피격할 때 호출합니다.*/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/**해당 몬스터의 상태를 죽음으로 변경합니다.*/
	void Dead();
	/**Mesh의 불투명도를 변경*/
	void ChangeOpacity(float DeltaTime);
	UFUNCTION() /**해당 몬스터의 공격이 성공할 때 실행됩니다.*/
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void GenDamageWidget(float DamageValue);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthPoint", meta = (AllowPrivateAccess = true))
	float HealthPoint;
	float Damage;
	float MovementSpeed;
	float AttackSpeed;
	
	class UMonsterAnimBase* MonsterAnim;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh", meta = (AllowPrivateAccess = true))
		TArray<USkeletalMeshComponent*> OpacitySkeletalMesh;	//Opacity변경할 Meshes

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = true))
	UParticleSystem* HitImpactP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = true))
	class UNiagaraSystem* SparkN;
	float MatOpacity;
	bool IsDead;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Damage", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class ADamageWidgetActor> DamageWidgetClass;
};
