// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABatteryPickup();

	// WasCollected 함수를 오버라이딩 함. - 임플멘테이션을 사용. 블루프린트 네이티브 이벤트이기 때문.
	void WasCollected_Implementation() override;


	// 캐릭터 등에서 값을 확인할 수 있도록 접근 함수를 만들어 줌. 퍼블릭 형으로, 어디에서든 배터리의 파워값을 얻을 수 있음.
	float GetPower();




protected:
	// 배터리가 캐릭터로 보내는 파워의 양.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true")) // C++ 상속관계에서만 편집 가능하고, 이를 상속받는 블루프린트에서는 변경이 불가능해짐. 하지만, 블루프린트에 protected 옵션을 떼고 전달할 수 있다. 즉, 상속받은 블루프린트에서도 값을 변경할 수 있게 되는 것.
	float BatteryPower;


};
