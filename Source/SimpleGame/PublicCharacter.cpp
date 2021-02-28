// Fill out your copyright notice in the Description page of Project Settings.

#include <PublicCharacter.h>
#include "SimpleWeapon.h"
#include <Runtime/Engine/Classes/Engine/World.h>
#include <Runtime/Engine/Classes/GameFramework/Character.h>
#include <Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <Runtime/Engine/Classes/GameFramework/Actor.h>
#include <Runtime/Engine/Public/TimerManager.h>
#include <Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <Runtime/Engine/Classes/GameFramework/Controller.h>
#include <Engine.h>
#include "PublicCharacter.h"


// Sets default values
APublicCharacter::APublicCharacter() // 플레이어 캐릭터(나)와 적 캐릭터에 공통적으로 쓰일 함수를 만들어서 쓰기 위한 클래스. (얘는 부모 클래스라고 함.)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 10000000.f;

}


// Called when the game starts or when spawned
void APublicCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APublicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APublicCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}








USkeletalMeshComponent* APublicCharacter::GetSpecificPawnMesh() const //    메쉬를 가져와서 읽어서 쓰기만 할것이기 때문에 const를 붙여줌.
{
	return GetMesh();	//웨폰 메쉬를 캐릭터의 메쉬에 붙이기 위한 소켓을 만드는 과정이라고 함. GetMesh()를 리턴해줌.이걸 왜 리턴해주는지 모르겠음.
}



FName APublicCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}


void APublicCharacter::AddWeapon(ASimpleWeapon* Weapon)
{
	if (Weapon)
	{
		Inventory.AddUnique(Weapon); // AddUnique()는 배열에서 아이템을 추가할 때 쓰임.
	}

}

void APublicCharacter::SetCurrentWeapon(class ASimpleWeapon* NewWeapon, class ASimpleWeapon* OldWeapon)
{
	ASimpleWeapon* LocalOldWeapon = NULL;

	if (OldWeapon != NULL)
	{
		LocalOldWeapon = OldWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalOldWeapon = CurrentWeapon;
	}

	if (LocalOldWeapon)
	{
		LocalOldWeapon->OnUnEquip(); // 이전에 사용한 웨폰을 떼어내주는 작업.
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetPawnOwner(this);
		NewWeapon->OnEquip(OldWeapon);
	}

}

void APublicCharacter::EquipWeapon(ASimpleWeapon* Weapon)
{
	if (Weapon)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
}

void APublicCharacter::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = DefaultInventoryClasses.Num()/* 그냥 Array 번호*/; // DefaultInventoryClasses 리스트를 가져와 정수형 변수를 만들었음.

	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo; // 스폰 액터와 함수에 필요한 정보들을 변수로 선언했습니다. 스폰 엑스터 함수에서 두번째 인수로 사용될 것입니다.

			ASimpleWeapon* NewWeapon = GetWorld()->SpawnActor<ASimpleWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon); // 스폰된 웨폰을 AddWeapon() 함수로 넣어줌.

		}
	}
	if (Inventory.Num() > 0) // 첫번째 리스트에 있는 웨폰을 가지고 있도록 설정함.
	{
		EquipWeapon(Inventory[0]);

	}
}

void APublicCharacter::OnChangeWeapon()
{
	const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);

	ASimpleWeapon* NextWeapon = Inventory[(CurrentWeaponIndex + 1) % Inventory.Num()];

	EquipWeapon(NextWeapon);
}

void APublicCharacter::WeaponAttack()
{
	if (CurrentWeapon) // "현재 무기가 설정되어 있다면,"  
	{
		CurrentWeapon->StartAttack();
	}
}

void APublicCharacter::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	float BehitAnimDuration = PlayAnimMontage(BehitAnim);

	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}

}

void APublicCharacter::Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Health = FMath::Min(0.0f, Health);

	UDamageType const* const DamageType =
		DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) :
		GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	float DeathAnimDuration = PlayAnimMontage(DeathAnim);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APublicCharacter::OnDieAnimationEnd, DeathAnimDuration, false);
}


void APublicCharacter::OnDieAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

float APublicCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}


	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;

	}


	if (Health <= 0)
	{
		Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
	}

	else
	{
		OnHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return ActualDamage;


}



/*

void APublicCharacter::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	float BehitAnimDuration = PlayAnimMontage(BehitAnim);

	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);

	}
}


void APublicCharacter::Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Health = FMath::Min(0.0f, Health);

	UDamageType const* const DamageType =
		DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject)) :
		GetDefault <UDamageType>();

		Killer = GetDamageInstigator(Killer, *DamageType);

		GetWorldTimerManager().ClearAllTimersForObject(this);

		if (GetCapsuleComponent)
		{
			GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
			GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
		}

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->DisableMovement();
		}

		if (Controller != NULL)
		{
			Controller->UnPossess();
		}

		float DeathAnimDuration = PlayAnimMontage(DeathAnim);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APublicCharacter::OnDieAnimationEnd, DeathAnimDuration, false);




}


void APublicCharacter::OnDieAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}


float APublicCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
	}

	if (Health <= 0)
	{
		Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
	}

	else
	{
		OnHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return ActualDamage;

}
	
	*/