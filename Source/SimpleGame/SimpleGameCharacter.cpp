// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SimpleGameCharacter.h"
#include <Engine.h>
//#include <SimpleGame.h>
#include <SimpleGameGameMode.h>
#include <PublicCharacter.h>
#include <Pickup.h>
#include <BatteryPickup.h>
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"



//////////////////////////////////////////////////////////////////////////
// ASimpleGameCharacter

ASimpleGameCharacter::ASimpleGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;


	// CollectionSphere 생성.
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere -> AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	// 캐릭터의 기본 파워 초기값을 설정함.
	InitialPower = 2000.f;

	CharacterPower = InitialPower;

	//ASimpleGameGameMode* SimpleGameGameModeReference;
	//SimpleGameGameModeReference = CreateDefaultSubobject<ASimpleGameGameMode>(TEXT("GameModeReference"));
	//MaximumPower = SimpleGameGameModeReference->GetPowerToWin();

	// 파워 레벨에 따라 속도를 설정함. (Set the dependence of the speed on the power level)
	SpeedFactor = 0.5f;
	BaseSpeed = 10.0f;



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ASimpleGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents(); // "함수성을 추가해줌. 언제나 함수를 덮었을 때 보통 부모에 있는 작동 방식도 표현합니다."

	SpawnDefaultInventory();

}




//////////////////////////////////////////////////////////////////////////
// Input

void ASimpleGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &APublicCharacter::OnChangeWeapon);


	InputComponent->BindAction("Collect", IE_Pressed, this, &ASimpleGameCharacter::CollectPickups);

	PlayerInputComponent->BindAction("StartAttack", IE_Pressed, this, &APublicCharacter::WeaponAttack);



	PlayerInputComponent->BindAxis("MoveForward", this, &ASimpleGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASimpleGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASimpleGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASimpleGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASimpleGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASimpleGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASimpleGameCharacter::OnResetVR);
}


void ASimpleGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASimpleGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ASimpleGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ASimpleGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASimpleGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASimpleGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASimpleGameCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASimpleGameCharacter::CollectPickups()
{
	// CollectionSphere와 오버랩 되는 모든 액터를 배열로 받아옴.
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors); // 'GetOverlappingActors()' 함수는 Primitive Component 클래스에 기본적으로 있는 함수.

	// 먹은 아이템의 파워를 계속 추적할 변수.
	float CollectedPower = 0;


	/*ASimpleGameGameMode* ASimpleGameCharacter::GetMaximumPower() const
	{
		
		return GetPowerToWin();
	}
	*/


	// 우리가 수집한 각각의 액터에서
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		// 액터를 APickup으로 형변환
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);
	
		// 형 변환이 성공하였을때, 그리고 아이템이 유효하고 활성화되어있을때
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			// 해당 아이템의 WasCollected 함수를 호출합니다. 
			TestPickup->WasCollected(); // 이거 이후에 인벤토리에 들어가게 함. 배터리는 아래 그대로 바로 에너지바에 채우도록 함.
			
			// Pickup 이 또한 Battery인지 확인하기 위해서 체크함. (번역에서는 아이템이 배터리 형인지 형변환으로 확인한다고 함.
			ABatteryPickup* const TestBattery = Cast<ABatteryPickup>(TestPickup);
				if (TestBattery) // 아이템이 활성 상태인지, PendingKill 리스트에 없는지 등을 이미 확인하였기 때문에, 이 위치에서는 다시 확인하지 않아도 됨.
				{
						// 아이템 속 파워값을 캐릭터에 더해주기.
						CollectedPower += TestBattery->GetPower(); // 이렇게 해서 속에 든 파워값을 가져옴.
						
				}
			// 아이템을 비활성화 시킴
			TestPickup->SetActive(false);


		}
					
	}

	if (CollectedPower > 0)
	{
		UpdatePower(CollectedPower);
	}

}


//////// ***********************    다른 클래스의 함수 가져오기    ********************************* //////////////////////////


/*
float ASimpleGameCharacter::GetPowerToWin()
{
	ASimpleGameGameMode* GameMode = Cast<ASimpleGameCharacter>(ASimpleGameGameMode::PowerToWin);
	return GameMode->GetPowerToWin();
}
//////// ***********************    다른 클래스의 함수 가져오기    ********************************* //////////////////////////
*/



// 파워의 초기값을 반환함.
float ASimpleGameCharacter::GetInitialPower()
{
	return InitialPower;
}

// 현재 파워값을 반환함.
float ASimpleGameCharacter::GetCurrentPower()
{
	return CharacterPower;
}


void ASimpleGameCharacter::SetCurrentPower(float CollectedPowerToAdd)
{
	CharacterPower = CharacterPower + CollectedPowerToAdd;

			
}

/*
float ASimpleGameCharacter::GetMaximumPower()
{
	return SimpleGameGameModeReference->GetPowerToWin();
}
*/


// 파워가 늘거나 줄어들 때 호출됨.
void ASimpleGameCharacter::UpdatePower(float CollectedPowerToAdd) // PowerChange를 파라미터로 갖음.
{
	// 파워 변경
	CharacterPower = CharacterPower + CollectedPowerToAdd; // 생성자 쪽에 있는 수치를 변경함.

	//ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	// ASimpleGameGameMode* SimpleGameGameModeReference = Cast<ASimpleGameGameMode>(MyCharacter );
	
	// MaximumPower = SimpleGameGameModeReference->GetMaximumPower();
	/*
	if (CharacterPower > SimpleGameGameModeReference->GetPowerToWin())
	{
		CharacterPower = 2000;//SimpleGameGameModeReference->GetPowerToWin();
	}
	*/
	// 파워에 따라 속도를 변경함.
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor * CharacterPower;

	// Call Visual Effect
	PowerChangeEffect();

}

