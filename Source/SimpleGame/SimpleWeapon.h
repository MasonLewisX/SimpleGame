// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PublicCharacter.h" // PublicCharacter의 객체를 만들어 쓰기 위해서 인클루드 시킴.
#include "SimpleWeapon.generated.h"  // 여기 이 generated.h 파일은 인클루드 헤더 목록에서 가장 마지막에 있어야 오류가 안남.

UCLASS()
class SIMPLEGAME_API ASimpleWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleWeapon(const class FObjectInitializer& ObjectInitializer); // 클래스가 생성될 때마다 기본적인 값을 넣을 생성자 함수가 있는데, 웨폰 메쉬를 생성해주기 위해서 FObjectInitializer 클래스가 필요함. 인수를 넣어서 선언.

	void SetPawnOwner(APublicCharacter* NewOwner); // Weapon을 부착하기 전에, Weapon을 소유하고 있는 캐릭터를 설정해주는 함수를 선언. PublicCharacter에서 가져가서 쓰기 때문에, Public에서 선언함.

	void AttachMeshToPawn();

	void OnEquip(const ASimpleWeapon* SelectedWeapon); // 여러가지 무기들 중에서, 선택한 무기를 붙이기 위해서 구현하는 함수

	void OnUnEquip();

	void StartAttack();
	void StopAttack();


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


private:
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon) // 아... 이 여자 자꾸 위폼이래....
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class USphereComponent* WeaponCollision;

	bool DuringAttack;     // 이 부분이 공격하지 않았을때 데미지 충돌 일어나지 않게 하는 부분


protected:
	class APublicCharacter* MyPawn;   // PublicCharacter 클래스의 모든 속성을 이어받아서 쓰기 위해 'PublicCharacter'의 객체로 아예 하나 만들어 씀. 그게 MyPawn. 그리고 이를 위해서는 그 클래스 헤더파일을 여기에다가 인클루드 시켜야됨.

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnim;
};

