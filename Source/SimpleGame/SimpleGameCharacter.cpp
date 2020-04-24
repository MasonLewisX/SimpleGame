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


	// CollectionSphere ����.
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere -> AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	// ĳ������ �⺻ �Ŀ� �ʱⰪ�� ������.
	InitialPower = 2000.f;

	CharacterPower = InitialPower;

	//ASimpleGameGameMode* SimpleGameGameModeReference;
	//SimpleGameGameModeReference = CreateDefaultSubobject<ASimpleGameGameMode>(TEXT("GameModeReference"));
	//MaximumPower = SimpleGameGameModeReference->GetPowerToWin();

	// �Ŀ� ������ ���� �ӵ��� ������. (Set the dependence of the speed on the power level)
	SpeedFactor = 0.5f;
	BaseSpeed = 10.0f;



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ASimpleGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents(); // "�Լ����� �߰�����. ������ �Լ��� ������ �� ���� �θ� �ִ� �۵� ��ĵ� ǥ���մϴ�."

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
	// CollectionSphere�� ������ �Ǵ� ��� ���͸� �迭�� �޾ƿ�.
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors); // 'GetOverlappingActors()' �Լ��� Primitive Component Ŭ������ �⺻������ �ִ� �Լ�.

	// ���� �������� �Ŀ��� ��� ������ ����.
	float CollectedPower = 0;


	/*ASimpleGameGameMode* ASimpleGameCharacter::GetMaximumPower() const
	{
		
		return GetPowerToWin();
	}
	*/


	// �츮�� ������ ������ ���Ϳ���
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		// ���͸� APickup���� ����ȯ
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);
	
		// �� ��ȯ�� �����Ͽ�����, �׸��� �������� ��ȿ�ϰ� Ȱ��ȭ�Ǿ�������
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			// �ش� �������� WasCollected �Լ��� ȣ���մϴ�. 
			TestPickup->WasCollected(); // �̰� ���Ŀ� �κ��丮�� ���� ��. ���͸��� �Ʒ� �״�� �ٷ� �������ٿ� ä�쵵�� ��.
			
			// Pickup �� ���� Battery���� Ȯ���ϱ� ���ؼ� üũ��. (���������� �������� ���͸� ������ ����ȯ���� Ȯ���Ѵٰ� ��.
			ABatteryPickup* const TestBattery = Cast<ABatteryPickup>(TestPickup);
				if (TestBattery) // �������� Ȱ�� ��������, PendingKill ����Ʈ�� ������ ���� �̹� Ȯ���Ͽ��� ������, �� ��ġ������ �ٽ� Ȯ������ �ʾƵ� ��.
				{
						// ������ �� �Ŀ����� ĳ���Ϳ� �����ֱ�.
						CollectedPower += TestBattery->GetPower(); // �̷��� �ؼ� �ӿ� �� �Ŀ����� ������.
						
				}
			// �������� ��Ȱ��ȭ ��Ŵ
			TestPickup->SetActive(false);


		}
					
	}

	if (CollectedPower > 0)
	{
		UpdatePower(CollectedPower);
	}

}


//////// ***********************    �ٸ� Ŭ������ �Լ� ��������    ********************************* //////////////////////////


/*
float ASimpleGameCharacter::GetPowerToWin()
{
	ASimpleGameGameMode* GameMode = Cast<ASimpleGameCharacter>(ASimpleGameGameMode::PowerToWin);
	return GameMode->GetPowerToWin();
}
//////// ***********************    �ٸ� Ŭ������ �Լ� ��������    ********************************* //////////////////////////
*/



// �Ŀ��� �ʱⰪ�� ��ȯ��.
float ASimpleGameCharacter::GetInitialPower()
{
	return InitialPower;
}

// ���� �Ŀ����� ��ȯ��.
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


// �Ŀ��� �ðų� �پ�� �� ȣ���.
void ASimpleGameCharacter::UpdatePower(float CollectedPowerToAdd) // PowerChange�� �Ķ���ͷ� ����.
{
	// �Ŀ� ����
	CharacterPower = CharacterPower + CollectedPowerToAdd; // ������ �ʿ� �ִ� ��ġ�� ������.

	//ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	// ASimpleGameGameMode* SimpleGameGameModeReference = Cast<ASimpleGameGameMode>(MyCharacter );
	
	// MaximumPower = SimpleGameGameModeReference->GetMaximumPower();
	/*
	if (CharacterPower > SimpleGameGameModeReference->GetPowerToWin())
	{
		CharacterPower = 2000;//SimpleGameGameModeReference->GetPowerToWin();
	}
	*/
	// �Ŀ��� ���� �ӵ��� ������.
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor * CharacterPower;

	// Call Visual Effect
	PowerChangeEffect();

}

