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


	// WasCollected 함수를 오버라이딩 함. - 임플멘테이션을 사용. 블루프린트 네이티브 이벤트이기 때문.
	void WasCollected_Implementation() override;



};
