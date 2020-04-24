// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include <SimpleGame.h>
#include <SimpleGameGameMode.h>
#include "PublicCharacter.h" // 여기까지 구현한 코드를 가져다 쓰기 위해 "GameFramework/Character.h" 를 PubulicCharacter.h 를 부모 클래스로 변경함.
#include <GameFramework/Character.h>
#include <Engine.h>
#include "SimpleGameCharacter.generated.h"

UCLASS(config = Game)
class ASimpleGameCharacter : public APublicCharacter // 부모 클래스 역시 PublicCharacter로 바꿔줌.(ACharacter에서)
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	// 아이템 탐지용 구체 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;




	
public:
	ASimpleGameCharacter();

	virtual void PostInitializeComponents() override; // 마지막으로 PostInitializeComponents() 함수의 SpawnDefaultInventory 함수를 추가하고, 게임이 시작하면 웨폰을 가지고 있도록 설정함.

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// Called every frame
	// virtual void Tick(float DeltaSeconds) override;

	
		

	//UPROPERTY(BlueprintReadOnly, Category = "Power")
	//	ASimpleGameGameMode* SimpleGameGameModeReference;

	//UPROPERTY(VisibleAnywhere, Category = "Power")
	//	float MaximumPower;
	

	// InitialPower값 얻어오는 함수.
	UFUNCTION(BlueprintPure, Category = "Power") // BlueprintPure는 블루프린트 함수에서 '실행 순서 핀이 없는' 노드임. 값을 변경하지 않고 현재 InitialPower 값만을 반환하기 때문.
		float GetInitialPower();

	// CurrentPower 값 얻어오는 함수.
	UFUNCTION(BlueprintPure, Category = "Power") // BlueprintPure는 블루프린트 함수에서 '실행 순서 핀이 없는' 노드임. 값을 변경하지 않고 현재 InitialPower 값만을 반환하기 때문.
		float GetCurrentPower();




	UFUNCTION(BlueprintCallable, Category = "Power")
		void SetCurrentPower(float CollectedPowerToAdd);


	// 캐릭터의 현재 파워값을 업데이트 함.
	// @param PowerChange는 현재 파워에 더할 값으로, 값은 양수, 또는 음수일 수도 있음. 이를 더해주는 방법으로는 함수 한개만을 이용해서 값을 증가/감소 시킬 수 있음.
	UFUNCTION(BlueprintCallable, Category = "Power")
		void UpdatePower(float PowerChange); // 플로트형 PowerChange를 파라미터로 받음.



protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


	//이 함수는 누르면 동작함. CollectionSphere 속에 들어있는 아이템을 수거함.
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	// 캐릭터 초기 파워값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float InitialPower;

	
	// 캐릭터의 속도에 곱해지는 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float SpeedFactor;

	// 파워 레벨이 0일때의 캐릭터 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float BaseSpeed;



	UFUNCTION(BlueprintImplementableEvent, Category = "Power")
		void PowerChangeEffect();



private:
	// 캐릭터의 현재 파워값
	UPROPERTY(VisibleAnywhere, Category = "Power")
	float CharacterPower;

	class ASimpleGameGameMode* SimpleGameGameModeReference;




public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// CollectionSphere Subobject를 반환해줌.
	FORCEINLINE class USphereComponent* GetCollectionShpere() const { return CollectionSphere; }
};

