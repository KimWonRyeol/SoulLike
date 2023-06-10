// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/LoglikeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/ABAnimInstance.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "Character/ABCharacterWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Character/ABWeapon.h"
#include "Particles/ParticleSystem.h"
#include "Props/Door.h"
#include "UI/WeaponEffect.h"
#include "UI/ArtifactEffect.h"
#include "Character/ABCharacterStatComponent.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Character/ABPlayerState.h"
#include "GameFramework/ABSaveGame.h"


//////////////////////////////////////////////////////////////////////////
// ALoglikeCharacter

ALoglikeCharacter::ALoglikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	//CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//공격중인지 판별하는 변수
	IsAttacking = false;
	//공격 최대 콤보 수
	MaxCombo = 3;
	//공격 콤보 초기화
	AttackEndComboState();
	//콜리전 채널 등 기본 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("LoglikeCharacter"));
	
	//캐릭터 공격 콜리전 범위(양옆)
	AttackRadius = 50.0f;
	//캐릭터 공격 콜리전 범위(앞뒤)
	AttackRange = 100.0f;

	//캐릭터 스텟 클래스 객체 설정
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	//캐릭터 죽음 유무
	IsDead = false;

	//무기 변경 유무(소켓 때문에)
	IsChangeWeapon = false;

	// Tick 처리 가능 + 시작하자마자 Tick 처리를 원할 때
	PrimaryActorTick.bCanEverTick = true;

	//사용할 HP Bar HUD 클래스 객체 생성
	/*static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	HUDWidgetClass = HealthBarObj.Class;*/

	//맞았을 때 파티클 효과 가져오기
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitImpactParticle(TEXT("/Script/Engine.ParticleSystem'/Game/UndeadPack/Blood/FX/P_HitImpact_2.P_HitImpact_2'"));
	if (HitImpactParticle.Succeeded())
		HitImpactP = HitImpactParticle.Object;

	CurrentCheat = false;
}


void ALoglikeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	
	SetWeapon(CurWeapon);

	//생성한 위젯 클래스 객체 확인
	if (HUDWidgetClass != nullptr)
	{
		//위젯 객체 생성
		CurrentWidget = CreateWidget<UABCharacterWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// Input

//캐릭터 기본 입력을 관리하는 함수
void ALoglikeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALoglikeCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALoglikeCharacter::Look);

	}

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Dodge);
	PlayerInputComponent->BindAction(TEXT("Parrying"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Parrying);
	PlayerInputComponent->BindAction(TEXT("CheatMode"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::ChangeCheatMode);
	//PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Inventory);
	//PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Interact);
}

//void ALoglikeCharacter::Inventory()
//{
//	UE_LOG(LogTemp, Warning, TEXT("sdfsdfsdfsdf"));
//}
//void ALoglikeCharacter::Interact()
//{
//
//}

//캐릭터의 이동 함수
void ALoglikeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !IsAttacking && !IsParrying)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//캐릭터가 보는 방향 체크하는 함수
void ALoglikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


//컴포넌트 초기화
void ALoglikeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	ABAnim->OnMontageEnded.AddDynamic(this, &ALoglikeCharacter::OnAttackMontageEnded);
	ABAnim->OnMontageEnded.AddDynamic(this, &ALoglikeCharacter::OnDodgeMontageEnded);
	ABAnim->OnMontageEnded.AddDynamic(this, &ALoglikeCharacter::OnParryingMontageEnded);

	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	ABAnim->OnAttackHitCheck.AddUObject(this, &ALoglikeCharacter::ToggleAttackMode);

	//세이브 데이터 로드 (스탯 컴포넌트에서 최신 데이터를 가져오지 못해서 여기서 로드했음.... 나중에 고칠 예정)
	UABSaveGame* ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));
	ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot("Player1", 0));
	if (nullptr == ABSaveGame)
	{
		CharacterStat->SetMaxHealth(0.0f);
		CharacterStat->SetDefaultDamage(0.0f);
		CharacterStat->SetDefaultLuck(0.0f);
		CharacterStat->SetDefaultToken(0);
		CharacterStat->SetWeaponKey(false);
		CharacterStat->SetArtifactKey(false);
		CharacterStat->SetArtifactKeyRemain(1);
		CharacterStat->SetWeaponKeyRemain(1);
		CharacterStat->SetHpStatRemain(10);
		CharacterStat->SetLuckStatRemain(10);
		CharacterStat->SetDamageStatRemain(10);
	}
	else 
	{
		CharacterStat->SetMaxHealth(ABSaveGame->MaxHealth);
		CharacterStat->SetDefaultDamage(ABSaveGame->DefaultDamage);
		CharacterStat->SetDefaultLuck(ABSaveGame->DefaultLuck);
		CharacterStat->SetDefaultToken(ABSaveGame->Token);
		CharacterStat->SetWeaponKey(ABSaveGame->WeaponKey);
		CharacterStat->SetArtifactKey(ABSaveGame->ArtifactKey);
		CharacterStat->SetArtifactKeyRemain(ABSaveGame->ArtifactKeyRemain);
		CharacterStat->SetWeaponKeyRemain(ABSaveGame->WeaponKeyRemain);
		CharacterStat->SetHpStatRemain(ABSaveGame->HpStatRemain);
		CharacterStat->SetLuckStatRemain(ABSaveGame->LuckStatRemain);
		CharacterStat->SetDamageStatRemain(ABSaveGame->DamageStatRemain);
	}

	//게임 인스턴스 객체 생성
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	if (ABGameInstance == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ErrorErrorErrorErrorErrorError"));
	}
	else
	{
		

		//Ruined Crown 장착했을 때의 효과(스테이지 이동시 체력 회복)
		if (ArtifactEffect.Recovery == true)
		{
			ABGameInstance->SetCurrentHealth(ABGameInstance->ArtifactEffect_Recovery);
			//CurrentHealth = ABGameInstance->GetCurrentHealth();
		}

		//다른 곳에서 스탯 변화를 주고 HUD에 보이도록 업데이트 해주는 함수
		SetUpdateStat();

		//추가 토큰 수 변수 초기화
		ABGameInstance->SetAddToken(-ABGameInstance->GetAddToken());
	}
}

//캐릭터 스테이스 상태 설정 함수 (필요 없을려나...)
void ALoglikeCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;
	switch (CurrentState)
	{
		case ECharacterState::LOADING: 
		{
			break;
		}
		case ECharacterState::READY:
		{
			break;
		}
		case ECharacterState::DEAD:
		{
			break;
		}
	}
}

//캐릭터 상태 반환 함수
ECharacterState ALoglikeCharacter::GetCharacterState() const
{
	return ECharacterState();
}

//공격 애님 실행 함수
void ALoglikeCharacter::Attack()
{
	if (IsDodge) return;
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	UE_LOG(LogTemp, Warning, TEXT("Character Damage : %f"), CharacterDamage);
}

//공격 몽타주 끝 체크 함수
void ALoglikeCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();
}

//콤보 공격 시작 상태 체크 함수
void ALoglikeCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

//콤보 공격 끝 상태 체크 함수
void ALoglikeCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

//닷지 애님 실행 함수
void ALoglikeCharacter::Dodge()
{
	if (!IsDodge && !DodgeDelay) {
		ABAnim->PlayDodgeMontage();
		IsDodge = true;
		DodgeDelay = true;
		GetWorld()->GetTimerManager().SetTimer(Dodge_Timer, this, &ALoglikeCharacter::Delay1, 2.0f, false);
	}
}

//닷지용 딜레이 함수
void ALoglikeCharacter::Delay1()
{
	DodgeDelay = false;
}

//닷지 몽타주 끝 체크 함수
void ALoglikeCharacter::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsDodge = false;
}

//패링 애님 실행 함수
void ALoglikeCharacter::Parrying()
{
	//기본 패링 시간
	float parryingduration = 1.0f;

	if (!IsParrying && !ParryingDelay) {
		//패링 시간을 늘려주는 무기를 가지면 패링시간 증가
		if (WeaponEffect.ParryingDuration) parryingduration += 1;

		ABAnim->PlayParryingMontage(parryingduration);
		IsParrying = true;
		ParryingDelay = true;
		GetWorld()->GetTimerManager().SetTimer(Parrying_Timer, this, &ALoglikeCharacter::Delay2, 2.0f, false); // ������
	}
}

//패링용 딜레이 함수
void ALoglikeCharacter::Delay2()
{
	ParryingDelay = false;
}

void ALoglikeCharacter::SuccessParrying()
{
	isParrySuccessful = true;
}

void ALoglikeCharacter::DrainEnemyHP()
{
	if (WeaponEffect.Leech == false) return;

	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	ABGameInstance->SetCurrentHealth(2);
	SetUpdateStat();
}

bool ALoglikeCharacter::GetIsParrying()
{
	return IsParrying;
}


//패링 몽타주 끝 체크 함수
void ALoglikeCharacter::OnParryingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsParrying = false;
}

//공격 콜리전을 켜서 공격했는지 체크하는 함수
void ALoglikeCharacter::AttackCheck()
{
	FHitResult HitResult;
	ActorsToIgnore.Add(this);

	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);
	
#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 0.5f;

	//콜리전이 눈에 보이도록 켜주는 함수
	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif
	//콜리전에 들어갔는지 체크
	if (bResult)
	{
		float finalDamage = CharacterDamage;
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			//마지막 콤보 공격시 2배 데미지를 주는 무기를 가지고 있을시
			if (WeaponEffect.FinalCombo)
			{
				if (CurrentCombo == 3) finalDamage = CharacterDamage * 2;
			}
			//패링 성공시 다음 공격을 2배 데미지 주는 무기를 가지고 있을시
			else if (WeaponEffect.ParryingAtk)
			{
				if (isParrySuccessful) finalDamage = CharacterDamage * 2;
				isParrySuccessful = false;
			}

			if (CurrentCheat)
			{
				finalDamage = CharacterDamage * 1000.f;
			}
			
			//데미지를 캐릭터에 적용시키는 함수 실행(기본 함수)	
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s %f"), *HitResult.GetActor()->GetName(), finalDamage);
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), finalDamage, GetController(), this, UDamageType::StaticClass());
		}
	}
}

void ALoglikeCharacter::MultiAttackCheck()
{
	TArray<FHitResult> HitResults;
	ActorsToIgnore.Add(this);

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetActorLocation();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	bool bResult = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLocation,
		EndLocation,
		10000.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3),
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	if (bResult)
	{
		float FinalDamage = CharacterDamage;

		if (WeaponEffect.FinalCombo && CurrentCombo == 3)
		{
			FinalDamage *= 2.0f;
		}
		else if (WeaponEffect.ParryingAtk && isParrySuccessful)
		{
			FinalDamage *= 2.0f;
			isParrySuccessful = false;
		}

		if (CurrentCheat)
		{
			FinalDamage *= 1000.0f;
		}

		for (auto HitResult : HitResults)
		{
			if (HitResult.GetActor() && HitResult.GetActor()->IsValidLowLevel())
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name: %s %f"), *HitResult.GetActor()->GetName(), FinalDamage);
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), FinalDamage, GetController(), this, UDamageType::StaticClass());
			}
		}
	}
}


//캐릭터 데이터 저장 함수
void ALoglikeCharacter::SaveCharacter()
{
	//게임 인스턴스 객체 생성
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());

	UABSaveGame* ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));

	ABSaveGame->MaxHealth = CharacterStat->GetMaxHealth();
	ABSaveGame->DefaultDamage = CharacterStat->GetDefaultDamage();
	ABSaveGame->DefaultLuck = CharacterStat->GetDefaultLuck();
	ABSaveGame->DefaultAttackSpeed = CharacterStat->GetDefaultAttackSpeed();
	ABSaveGame->Token = CharacterToken;
	ABSaveGame->ArtifactKey = CharacterStat->GetArtifactKey();
	ABSaveGame->WeaponKey = CharacterStat->GetWeaponKey();
	ABSaveGame->ArtifactKeyRemain = CharacterStat->GetArtifactKeyRemain();
	ABSaveGame->WeaponKeyRemain = CharacterStat->GetWeaponKeyRemain();
	ABSaveGame->HpStatRemain = CharacterStat->GetHpStatRemain();
	ABSaveGame->DamageStatRemain= CharacterStat->GetDamageStatRemain();
	ABSaveGame->LuckStatRemain= CharacterStat->GetLuckStatRemain();

	if (!UGameplayStatics::SaveGameToSlot(ABSaveGame, "Player1", 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Savefile Load Error!!!!"));
	}
}

//받는 데미지 처리 함수
float ALoglikeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//최종 계산된 데미지를 담을 변수
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	//게임 인스턴스 객체 생성
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());

	//구르기 중일 때는 피해x
	if (IsDodge == false && IsParrying == false) 
	{
		CurrentHealth -= FinalDamage;
		if (CurrentHealth < 0) CurrentHealth = 0;
		ABGameInstance->SetCurrentHealth(-FinalDamage);

		//HUD에 피격시 효과 실행
		CurrentWidget->PlayWidgetAnimationByName();

		//blood particle 생성
		if (HitImpactP != nullptr)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpactP, GetActorLocation());
	}

	//델리게이트에 바인딩된 함수를 호출
	//OnHPChanged.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("Character Max Health : %f "), MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Character Current Health : %f "), CurrentHealth);
	if (CurrentHealth <= 0.0f)
	{
		this->DisableInput(Cast<APlayerController>(GetController()));	//죽으면 입력 비활성화
		ABAnim->SetDeadAnim();//죽는 애니메이션 실행하도록 함수
		SetActorEnableCollision(false); //죽을 때는 콜리전 해제
		IsDead = true;

		FTimerHandle GravityTimerHandle;
		float GravityTime = 3.0f;

		GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));

			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
	}
	return FinalDamage;
}

//무기 장착이 가능한지를 반환하는 함수
bool ALoglikeCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}
//무기 장착 함수
void ALoglikeCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	//무기 소켓 장착
	//게임 인스턴스에서 현재 착용하고 있는 무기가 기본 무기인지 이름을 통해서 소켓 구분
	if (IsChangeWeapon == false)
	{
		WeaponSocket = FName(TEXT("RightHand_Weapon"));
	}
	else
	{
		WeaponSocket = FName(TEXT("RightHand_Weapon0"));
	}
	
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

//블루프린트에서 체력 비율을 가져갈 때 사용할 함수
float ALoglikeCharacter::GetHealthRatio()
{
	//게임 인스턴스 객체 생성
	/*auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());*/

	return (CurrentHealth < KINDA_SMALL_NUMBER ? 0.0f : (CurrentHealth / MaxHealth));
}

//블루프린트에서 캐릭터의 체력 스탯을 가져갈 때 사용할 함수
float ALoglikeCharacter::GetCharacterHealth()
{
	return HealthStat;
}

//블루프린트에서 캐릭터의 공격력 스탯을 가져갈 때 사용할 함수
float ALoglikeCharacter::GetCharacterDamage()
{
	return DamageStat;
}

//블루프린트에서 캐릭터의 행운 스탯을 가져갈 때 사용할 함수
float ALoglikeCharacter::GetCharacterLuck()
{
	return CharacterLuck;
}

//블루프린트에서 캐릭터의 토큰을 가져갈 때 사용할 함수
int ALoglikeCharacter::GetCharacterToken()
{
	return CharacterToken;
}

float ALoglikeCharacter::GetCharacterMaxHealth()
{
	return MaxHealth;
}

float ALoglikeCharacter::GetCharacterCurrentHealth()
{
	return CurrentHealth;
}

//다른 곳에서 스탯 변화를 주고 HUD에 보이도록 업데이트 해주는 함수
void ALoglikeCharacter::SetUpdateStat()
{
	//게임 인스턴스 객체 생성
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());

	//무기 효과를 가져옴 
	WeaponEffect = ABGameInstance->GetWeaponEffect();

	//아티팩트 효과를 가져옴
	ArtifactEffect = ABGameInstance->GetArtifactEffect();

	//캐릭터 체력 스탯 설정
	HealthStat = CharacterStat->GetMaxHealth() + ABGameInstance->GetAddHealth();

	//Gem Of HP 장착했을 때의 효과(HP 스탯 증가)
	if (ArtifactEffect.Stat_UP_HP == true)
	{
		HealthStat += ABGameInstance->ArtifactEffect_Stat_HP;
	}

	//캐릭터 최대 체력 세팅
	MaxHealth = 100.0f + HealthStat * 5;

	//Ruby Amulet 장착했을 때의 효과(HP 증가)
	if (ArtifactEffect.HP_UP == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("222222"));
		MaxHealth += ABGameInstance->ArtifactEffect_MaxHealth;
	}

	//GlassCannon 무기를 가질 때 최대체력의 90% 감소
	if (WeaponEffect.GlassCannon) MaxHealth *= 0.1;

	ABGameInstance->SetMaxHealth(100.0f + HealthStat * 5);
	
	//캐릭터 현재 체력 세팅
	CurrentHealth = ABGameInstance->GetCurrentHealth() + ABGameInstance->ArtifactEffect_MaxHealth;


	//Gem Of HP 장착했을 때의 효과(HP 스탯 증가)
	if (ArtifactEffect.Stat_UP_HP == true)
	{
		CurrentHealth += ABGameInstance->ArtifactEffect_Stat_HP * 5;
	}

	//GlassCannon 무기를 가질 때 현재체력을 최대체력에 맞게 Clamp
	if (WeaponEffect.GlassCannon)
	{
		if (CurrentHealth > MaxHealth) CurrentHealth = MaxHealth;
	}
	UE_LOG(LogTemp, Warning, TEXT("EEEEEEEEEE : %f %f"), CurrentHealth, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("QQQQQQQQQQ : %f %f"), ABGameInstance->GetCurrentHealth(), ABGameInstance->MaxHealth);

	//캐릭터 공격력 세팅 (기본무기 공격력 7 + 스탯 공격력)
	DamageStat = CharacterStat->GetDefaultDamage() + ABGameInstance->GetAddDamage();

	//Gem Of ATK 장착했을 때의 효과(공격력 스탯 증가)
	if (ArtifactEffect.Stat_UP_Atk == true)
	{
		DamageStat += ABGameInstance->ArtifactEffect_Stat_Damage;
	}

	//GlassCannon 무기를 가질 때 공격력 스탯 20 상승
	if(WeaponEffect.GlassCannon) DamageStat += 20;

	CharacterDamage = ABGameInstance->GetWeaponDamage() + DamageStat * 4;

	//Rage Helm 장착했을 때의 효과(Damage 증가)
	if (ArtifactEffect.Damage_UP == true)
	{
		if (CharacterDamage * ABGameInstance->ArtifactEffect_Damage < 1.0)
			CharacterDamage += 1.0;
		else
			CharacterDamage = 1.0 + CharacterDamage + CharacterDamage * ABGameInstance->ArtifactEffect_Damage;
	}

	//캐릭터 행운 세팅
	CharacterLuck = CharacterStat->GetDefaultLuck() + ABGameInstance->GetAddLuck();

	//Gem Of LUK 장착했을 때의 효과(행운 스탯 증가)
	if (ArtifactEffect.Stat_UP_Luck == true)
	{
		CharacterLuck += ABGameInstance->ArtifactEffect_Stat_Luck;
	}

	//캐릭터 토큰 세팅
	CharacterToken = CharacterStat->GetToken();

	//ABGameInstance->SetCurrentHealth(CurrentHealth);

	SaveCharacter();
}

bool ALoglikeCharacter::GetWeaponKey()
{
	return CharacterStat->GetWeaponKey();
}

bool ALoglikeCharacter::GetArtifactKey()
{
	return CharacterStat->GetArtifactKey();
}

void ALoglikeCharacter::ChangeCheatMode()
{
	if (!CurrentCheat)
	{
		UE_LOG(LogTemp, Warning, TEXT("Now Is Not Cheat Mode. => Change!"));
		CurrentCheat = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Now Is Cheat Mode!!  ChangeTT"));
		CurrentCheat = false;
	}
}

void ALoglikeCharacter::ToggleAttackMode()
{
	if (CurrentCheat)
	{
		MultiAttackCheck();
	}
	else
	{
		AttackCheck();
	}
}