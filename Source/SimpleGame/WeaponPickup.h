// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API AWeaponPickup : public APickup
{
	GENERATED_BODY()
	

public:
	AWeaponPickup();


	// WasCollected �Լ��� �������̵� ��. - ���ø����̼��� ���. �������Ʈ ����Ƽ�� �̺�Ʈ�̱� ����.
	void WasCollected_Implementation() override;



};
