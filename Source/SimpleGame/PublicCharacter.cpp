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
APublicCharacter::APublicCharacter() // �÷��̾� ĳ����(��)�� �� ĳ���Ϳ� ���������� ���� �Լ��� ���� ���� ���� Ŭ����. (��� �θ� Ŭ������� ��.)
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








USkeletalMeshComponent* APublicCharacter::GetSpecificPawnMesh() const //    �޽��� �����ͼ� �о ���⸸ �Ұ��̱� ������ const�� �ٿ���.
{
	return GetMesh();	//���� �޽��� ĳ������ �޽��� ���̱� ���� ������ ����� �����̶�� ��. GetMesh()�� ��������.�̰� �� �������ִ��� �𸣰���.
}



FName APublicCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}


void APublicCharacter::AddWeapon(ASimpleWeapon* Weapon)
{
	if (Weapon)
	{
		Inventory.AddUnique(Weapon); // AddUnique()�� �迭���� �������� �߰��� �� ����.
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
		LocalOldWeapon->OnUnEquip(); // ������ ����� ������ ����ִ� �۾�.
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
	int32 NumWeaponClasses = DefaultInventoryClasses.Num()/* �׳� Array ��ȣ*/; // DefaultInventoryClasses ����Ʈ�� ������ ������ ������ �������.

	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo; // ���� ���Ϳ� �Լ��� �ʿ��� �������� ������ �����߽��ϴ�. ���� ������ �Լ����� �ι�° �μ��� ���� ���Դϴ�.

			ASimpleWeapon* NewWeapon = GetWorld()->SpawnActor<ASimpleWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon); // ������ ������ AddWeapon() �Լ��� �־���.

		}
	}
	if (Inventory.Num() > 0) // ù��° ����Ʈ�� �ִ� ������ ������ �ֵ��� ������.
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
	if (CurrentWeapon) // "���� ���Ⱑ �����Ǿ� �ִٸ�,"  
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