// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <SimpleGame.h>
#include <Engine.h>
#include "SimpleGameGameMode.generated.h"


// 현재 게임 플레이 상태를 저장하는 Enum. (Enum to store the current state of gameplay.)
UENUM(BlueprintType)
enum class ESimpleGamePlayState : uint8
{
	EPlaying,
	EGameOver,
	EWin,
	EUnknown

};


UCLASS(minimalapi)
class ASimpleGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASimpleGameGameMode();

	virtual void Tick(float Seconds) override;


	// 게임 승리에 필요한 파워를 알려줌. HUD에 필요. (Returns power needed to win.- Need for the HUD.)
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetPowerToWin() const;

	virtual void BeginPlay() override;


	// 현재 플레이 상태로 돌아감.
	UFUNCTION(BlueprintPure, Category = "Power")
	ESimpleGamePlayState GetCurrentState() const;

	// 새 플레이 상태 설정.
	void SetCurrentState(ESimpleGamePlayState NewState);


//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	//	float GetMaximumPower() const;




	//UPROPERTY(EditDefaultsOnly, Category = Anim)
	//	UAnimMontage* Death_Montage;
	


protected:
	// 캐릭터의 파워가 줄어드는 정도.
	// 캐릭터나 다른 클래스 대신에 '게임모드'에 설정을 하는 이유는 '게임모드'에서 게임의 룰을 정하기 때문.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float DecayRate;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float PowerToWin;
	


	// The Widget class to use for our HUD screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// HUD의 인스턴스.
	UPROPERTY() // 이렇게만 해서 Garbage collection 될 수 있게 만들어줌.
	class UUserWidget* CurrentWidget;



private:

	// 현재 플레이 상태를 추적함. (Keeps track of current playing state.
	ESimpleGamePlayState CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;


	// 게임 플레이 상태에 영향을 주는 모든 함수의 호출을 담당.(Handle any function calls that rely upon changing the playing state of our game.)
	void HandleNewState(ESimpleGamePlayState NewState);



};



