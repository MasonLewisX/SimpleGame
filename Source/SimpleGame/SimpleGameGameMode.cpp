// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SimpleGameGameMode.h"
#include "SimpleGameCharacter.h"
#include <SimpleGame.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include <Engine.h>
#include <SpawnVolume.h>
#include "UObject/ConstructorHelpers.h"


ASimpleGameGameMode::ASimpleGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;  // �� �κ��� ��� �ȵƴ��ſ���. �׸��� �Ʒ��� �ִ� ���� �ڵ忡�� Super::Tick(DeltaSeconds); �κ� �ּ�ó�� �ؼ� ��.



	// DecayRate �⺻�� ����.
	DecayRate = 0.02f;
}


//BeginPlay()�� ������ ���Ŀ� �����ؾ���. �׷��Ƿ� Tick() �տ� �ۼ�����.
void ASimpleGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ��� Spawn Volume Actor�� ã�Ƴ�.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}





	SetCurrentState(ESimpleGamePlayState::EPlaying);

	// ��ǥ ���� ����
	ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower())*1.25f;
		//MaximumPower = PowerToWin;
	} // ������� �Ǵ°� Ȯ����. 1.25f�� 1.f�� �ٲ㼭 �Ǵ°� Ȯ����.

	


	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}

	}

	// ��������� �� ��.

}


// *****************************                        ��������� �� ��.                     ****************************************


/*
float ASimpleGameGameMode::GetMaximumPower()
{
	
	return MaximumPower;
}
*/




void ASimpleGameGameMode::Tick(float DeltaSeconds)
{
	// *�� �������� �θ𿡼� ���� ����Ǵ� �κ�(� �κ�?)�� �׳� �Ѿ�� �ʰ� ��.*
	// Super::Tick(DeltaSeconds);

	// SimpleGameCharacter�� ����ϰ� �ִ� ������ Ȯ����.
	ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		// �䱸ġ �̻��� �Ŀ��� ������ ���� ���¸� Win���� ����.
		/* if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(ESimpleGamePlayState::EWin);
		} // �۵� �ȵ�.
		*/ 
		// ĳ������ �Ŀ��� 0���� ũ��,
		/*else */if (MyCharacter->GetCurrentPower() > 0)
		{
			if (MyCharacter->GetCurrentPower() > GetPowerToWin())
			{

				MyCharacter->SetCurrentPower(-(MyCharacter->GetCurrentPower()-GetPowerToWin()));


			 }



			// DecayRate�� �̿��ؼ� �Ŀ��� ���ҽ�Ŵ.
			MyCharacter->UpdatePower(-DeltaSeconds*DecayRate*(MyCharacter->GetInitialPower()));
		}

		else
		{
			SetCurrentState(ESimpleGamePlayState::EGameOver);
		}


	}
}


float ASimpleGameGameMode::GetPowerToWin() const
{
	return PowerToWin;

}


ESimpleGamePlayState ASimpleGameGameMode::GetCurrentState() const
{
	return CurrentState;
}


void ASimpleGameGameMode::SetCurrentState(ESimpleGamePlayState NewState)
{
	// Set current state
	CurrentState = NewState;

	// ���ο� ���� ���¸� ����. Handle the new current state
	HandleNewState(CurrentState);


}



void ASimpleGameGameMode::HandleNewState(ESimpleGamePlayState NewState)
{
	switch (NewState)
	{
		// If the game is Playing
	case ESimpleGamePlayState::EPlaying:
	{
		// Spawn volumes active
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(true);
		}

	}
	break;

	// If we've won the game
	/* case ESimpleGamePlayState::EWin: // �۵� �ȵ�.
	{
		// Spawn volumes inactive
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}

	}
	break;
	*/
	// If we've lost the game
	case ESimpleGamePlayState::EGameOver: // ���̳ʽ��� ���� �ʾƼ� Ȯ���� ���� ����.
	{
		// Spawn volumes inactive
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}

		// Block player input
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

		// Ragdoll the character
		ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (MyCharacter)
		{
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			MyCharacter->GetMovementComponent()->MovementState.bCanWalk = false;
			// MyCharacter->InputComponent->bIsActive = false;
		
			
			//MyCharacter->PlayAnimMontage(Death_Montage, 1.0f, NAME_None);
		}

	}
	break;

	// Unknown/Default state
	default:
	case ESimpleGamePlayState::EUnknown:
	{
		//

	}
	break;
	}


}