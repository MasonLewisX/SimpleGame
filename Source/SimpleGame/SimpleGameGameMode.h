// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <SimpleGame.h>
#include <Engine.h>
#include "SimpleGameGameMode.generated.h"


// ���� ���� �÷��� ���¸� �����ϴ� Enum. (Enum to store the current state of gameplay.)
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


	// ���� �¸��� �ʿ��� �Ŀ��� �˷���. HUD�� �ʿ�. (Returns power needed to win.- Need for the HUD.)
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetPowerToWin() const;

	virtual void BeginPlay() override;


	// ���� �÷��� ���·� ���ư�.
	UFUNCTION(BlueprintPure, Category = "Power")
	ESimpleGamePlayState GetCurrentState() const;

	// �� �÷��� ���� ����.
	void SetCurrentState(ESimpleGamePlayState NewState);


//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	//	float GetMaximumPower() const;




	//UPROPERTY(EditDefaultsOnly, Category = Anim)
	//	UAnimMontage* Death_Montage;
	


protected:
	// ĳ������ �Ŀ��� �پ��� ����.
	// ĳ���ͳ� �ٸ� Ŭ���� ��ſ� '���Ӹ��'�� ������ �ϴ� ������ '���Ӹ��'���� ������ ���� ���ϱ� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float DecayRate;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float PowerToWin;
	


	// The Widget class to use for our HUD screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// HUD�� �ν��Ͻ�.
	UPROPERTY() // �̷��Ը� �ؼ� Garbage collection �� �� �ְ� �������.
	class UUserWidget* CurrentWidget;



private:

	// ���� �÷��� ���¸� ������. (Keeps track of current playing state.
	ESimpleGamePlayState CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;


	// ���� �÷��� ���¿� ������ �ִ� ��� �Լ��� ȣ���� ���.(Handle any function calls that rely upon changing the playing state of our game.)
	void HandleNewState(ESimpleGamePlayState NewState);



};



