// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include <SimpleGame.h>
#include <SimpleGameGameMode.h>
#include "PublicCharacter.h" // ������� ������ �ڵ带 ������ ���� ���� "GameFramework/Character.h" �� PubulicCharacter.h �� �θ� Ŭ������ ������.
#include <GameFramework/Character.h>
#include <Engine.h>
#include "SimpleGameCharacter.generated.h"

UCLASS(config = Game)
class ASimpleGameCharacter : public APublicCharacter // �θ� Ŭ���� ���� PublicCharacter�� �ٲ���.(ACharacter����)
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	// ������ Ž���� ��ü �ݸ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;




	
public:
	ASimpleGameCharacter();

	virtual void PostInitializeComponents() override; // ���������� PostInitializeComponents() �Լ��� SpawnDefaultInventory �Լ��� �߰��ϰ�, ������ �����ϸ� ������ ������ �ֵ��� ������.

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
	

	// InitialPower�� ������ �Լ�.
	UFUNCTION(BlueprintPure, Category = "Power") // BlueprintPure�� �������Ʈ �Լ����� '���� ���� ���� ����' �����. ���� �������� �ʰ� ���� InitialPower ������ ��ȯ�ϱ� ����.
		float GetInitialPower();

	// CurrentPower �� ������ �Լ�.
	UFUNCTION(BlueprintPure, Category = "Power") // BlueprintPure�� �������Ʈ �Լ����� '���� ���� ���� ����' �����. ���� �������� �ʰ� ���� InitialPower ������ ��ȯ�ϱ� ����.
		float GetCurrentPower();




	UFUNCTION(BlueprintCallable, Category = "Power")
		void SetCurrentPower(float CollectedPowerToAdd);


	// ĳ������ ���� �Ŀ����� ������Ʈ ��.
	// @param PowerChange�� ���� �Ŀ��� ���� ������, ���� ���, �Ǵ� ������ ���� ����. �̸� �����ִ� ������δ� �Լ� �Ѱ����� �̿��ؼ� ���� ����/���� ��ų �� ����.
	UFUNCTION(BlueprintCallable, Category = "Power")
		void UpdatePower(float PowerChange); // �÷�Ʈ�� PowerChange�� �Ķ���ͷ� ����.



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


	//�� �Լ��� ������ ������. CollectionSphere �ӿ� ����ִ� �������� ������.
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	// ĳ���� �ʱ� �Ŀ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float InitialPower;

	
	// ĳ������ �ӵ��� �������� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float SpeedFactor;

	// �Ŀ� ������ 0�϶��� ĳ���� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float BaseSpeed;



	UFUNCTION(BlueprintImplementableEvent, Category = "Power")
		void PowerChangeEffect();



private:
	// ĳ������ ���� �Ŀ���
	UPROPERTY(VisibleAnywhere, Category = "Power")
	float CharacterPower;

	class ASimpleGameGameMode* SimpleGameGameModeReference;




public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// CollectionSphere Subobject�� ��ȯ����.
	FORCEINLINE class USphereComponent* GetCollectionShpere() const { return CollectionSphere; }
};

