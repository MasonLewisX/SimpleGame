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


	   	 

	

	USkeletalMeshComponent* GetSpecificPawnMesh() const; // ���� �޽��� ĳ������ �޽��� ���̱� ���� ������ ����� ����. �ҽ����Ϸ� ���� GetMesh()�� ��������. �̰� �� �������ִ��� �𸣰���.

	FName GetWeaponAttachPoint() const; // ���� �̸��� Weapon Class���� ����� �� �ְ� �Լ�ȭ�Ͽ� ��������.

	void EquipWeapon(class ASimpleWeapon* Weapon); // ����ڰ� ���ϴ� ������ �־��ֱ� ���ؼ� �μ��� �־ EquipWeapon() �Լ��� ����.

	void OnChangeWeapon();

	void WeaponAttack();

	   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float Health;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;








protected:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName WeaponAttachPoint;

	TArray<class ASimpleWeapon*> Inventory; // �κ��丮�� ���Ⱑ ��ġ�� ���� �ִµ� �� �߿��� ������ ��� �� �������� �迭�� �ؼ� ����.

	class ASimpleWeapon* CurrentWeapon;

	void AddWeapon(class ASimpleWeapon* Weapon);

	void SetCurrentWeapon(class ASimpleWeapon* NewWeapon, class ASimpleWeapon* LastWeapon);

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class ASimpleWeapon>> DefaultInventoryClasses; // ���� ����Ʈ�� �־�� �������� ��� ���� ���� ���� ���� �Լ��� �������� �߰������ְڽ��ϴ�.

	void SpawnDefaultInventory();




	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* BehitAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* DeathAnim;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);
	//					     ������ ��ġ,							������ �̺�Ʈ,			  �������� �޴� ��,			�������� �ִ� ����
	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	void OnDieAnimationEnd();

};