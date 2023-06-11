// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageWidgetActor.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "UI/DamageWidget.h"

// Sets default values
ADamageWidgetActor::ADamageWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetSimulatePhysics(true);
	RootComponent = StaticMeshComponent;
	DamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DAMAGEWIDGET"));
	DamageWidget->SetupAttachment(RootComponent);
	DamageWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void ADamageWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	FVector RandomVec;
	RandomVec.X = FMath::RandRange(-400.f,400.f);
	RandomVec.Y = FMath::RandRange(-400.f,400.f);
	RandomVec.Z = FMath::RandRange(400.f,800.f);
	StaticMeshComponent->AddImpulse(RandomVec);
}

// Called every frame
void ADamageWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageWidgetActor::SetDamageValue(int32 DamageValue)
{
	if(DamageWidget == nullptr) return;
	if(!Cast<UDamageWidget>(DamageWidget->GetUserWidgetObject())) return;
	Cast<UDamageWidget>(DamageWidget->GetUserWidgetObject())->SetDamageNumberText(DamageValue);
}

