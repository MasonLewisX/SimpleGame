// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PublicCharacter.generated.h"

UCLASS()
class SIMPLEGAME_API APublicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APublicCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functrionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	   	 

	

	USkeletalMeshComponent* GetSpecificPawnMesh() const; // 웨폰 메쉬를 캐릭터의 메쉬에 붙이기 위한 소켓을 만드는 과정. 소스파일로 가서 GetMesh()를 리턴해줌. 이걸 왜 리턴해주는지 모르겠음.

	FName GetWeaponAttachPoint() const; // 소켓 이름을 Weapon Class에서 사용할 수 있게 함수화하여 리턴해줌.

	void EquipWeapon(class ASimpleWeapon* Weapon); // 사용자가 원하는 웨폰을 넣어주기 위해서 인수를 넣어서 EquipWeapon() 함수를 만듦.

	void OnChangeWeapon();

	void WeaponAttack();

	   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float Health;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;








protected:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName WeaponAttachPoint;

	TArray<class ASimpleWeapon*> Inventory; // 인벤토리에 무기가 망치와 총이 있는데 이 중에서 무엇을 골라서 쓸 것인지를 배열로 해서 정함.

	class ASimpleWeapon* CurrentWeapon;

	void AddWeapon(class ASimpleWeapon* Weapon);

	void SetCurrentWeapon(class ASimpleWeapon* NewWeapon, class ASimpleWeapon* LastWeapon);

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class ASimpleWeapon>> DefaultInventoryClasses; // 웨폰 리스트에 넣어둔 웨폰들을 모두 스폰 시켜 얻은 웨폰 함수로 웨폰들을 추가시켜주겠습니다.

	void SpawnDefaultInventory();




	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* BehitAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* DeathAnim;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);
	//					     데미지 수치,							데미지 이벤트,			  데미지를 받는 폰,			데미지를 주는 액터
	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	void OnDieAnimationEnd();

};