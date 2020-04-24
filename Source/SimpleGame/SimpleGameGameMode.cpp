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
	PrimaryActorTick.bCanEverTick = true;  // 이 부분이 없어서 안됐던거였음. 그리고 아래에 있는 본문 코드에서 Super::Tick(DeltaSeconds); 부분 주석처리 해서 됨.



	// DecayRate 기본값 설정.
	DecayRate = 0.02f;
}


//BeginPlay()는 생성자 이후에 발행해야함. 그러므로 Tick() 앞에 작성해줌.
void ASimpleGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 모든 Spawn Volume Actor를 찾아냄.
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

	// 목표 점수 설정
	ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower())*1.25f;
		//MaximumPower = PowerToWin;
	} // 여기까지 되는거 확인함. 1.25f를 1.f로 바꿔서 되는거 확인함.

	


	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}

	}

	// 여기까지는 잘 됨.

}


// *****************************                        여기까지는 잘 됨.                     ****************************************


/*
float ASimpleGameGameMode::GetMaximumPower()
{
	
	return MaximumPower;
}
*/




void ASimpleGameGameMode::Tick(float DeltaSeconds)
{
	// *이 설정으로 부모에서 원래 실행되던 부분(어떤 부분?)을 그냥 넘어가지 않게 함.*
	// Super::Tick(DeltaSeconds);

	// SimpleGameCharacter를 사용하고 있는 것인지 확인함.
	ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		// 요구치 이상의 파워를 모으면 게임 상태를 Win으로 설정.
		/* if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(ESimpleGamePlayState::EWin);
		} // 작동 안됨.
		*/ 
		// 캐릭터의 파워가 0보다 크면,
		/*else */if (MyCharacter->GetCurrentPower() > 0)
		{
			if (MyCharacter->GetCurrentPower() > GetPowerToWin())
			{

				MyCharacter->SetCurrentPower(-(MyCharacter->GetCurrentPower()-GetPowerToWin()));


			 }



			// DecayRate를 이용해서 파워를 감소시킴.
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

	// 새로운 현재 상태를 제어. Handle the new current state
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
	/* case ESimpleGamePlayState::EWin: // 작동 안됨.
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
	case ESimpleGamePlayState::EGameOver: // 마이너스가 되지 않아서 확인할 수가 없음.
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