// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PublicCharacter.h" // PublicCharacter�� ��ü�� ����� ���� ���ؼ� ��Ŭ��� ��Ŵ.
#include "SimpleWeapon.generated.h"  // ���� �� generated.h ������ ��Ŭ��� ��� ��Ͽ��� ���� �������� �־�� ������ �ȳ�.

UCLASS()
class SIMPLEGAME_API ASimpleWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleWeapon(const class FObjectInitializer& ObjectInitializer); // Ŭ������ ������ ������ �⺻���� ���� ���� ������ �Լ��� �ִµ�, ���� �޽��� �������ֱ� ���ؼ� FObjectInitializer Ŭ������ �ʿ���. �μ��� �־ ����.

	void SetPawnOwner(APublicCharacter* NewOwner); // Weapon�� �����ϱ� ����, Weapon�� �����ϰ� �ִ� ĳ���͸� �������ִ� �Լ��� ����. PublicCharacter���� �������� ���� ������, Public���� ������.

	void AttachMeshToPawn();

	void OnEquip(const ASimpleWeapon* SelectedWeapon); // �������� ����� �߿���, ������ ���⸦ ���̱� ���ؼ� �����ϴ� �Լ�

	void OnUnEquip();

	void StartAttack();
	void StopAttack();


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


private:
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon) // ��... �� ���� �ڲ� �����̷�....
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class USphereComponent* WeaponCollision;

	bool DuringAttack;     // �� �κ��� �������� �ʾ����� ������ �浹 �Ͼ�� �ʰ� �ϴ� �κ�


protected:
	class APublicCharacter* MyPawn;   // PublicCharacter Ŭ������ ��� �Ӽ��� �̾�޾Ƽ� ���� ���� 'PublicCharacter'�� ��ü�� �ƿ� �ϳ� ����� ��. �װ� MyPawn. �׸��� �̸� ���ؼ��� �� Ŭ���� ��������� ���⿡�ٰ� ��Ŭ��� ���Ѿߵ�.

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnim;
};

