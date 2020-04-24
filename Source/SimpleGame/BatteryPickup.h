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

	// WasCollected �Լ��� �������̵� ��. - ���ø����̼��� ���. �������Ʈ ����Ƽ�� �̺�Ʈ�̱� ����.
	void WasCollected_Implementation() override;


	// ĳ���� ��� ���� Ȯ���� �� �ֵ��� ���� �Լ��� ����� ��. �ۺ� ������, ��𿡼��� ���͸��� �Ŀ����� ���� �� ����.
	float GetPower();




protected:
	// ���͸��� ĳ���ͷ� ������ �Ŀ��� ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true")) // C++ ��Ӱ��迡���� ���� �����ϰ�, �̸� ��ӹ޴� �������Ʈ������ ������ �Ұ�������. ������, �������Ʈ�� protected �ɼ��� ���� ������ �� �ִ�. ��, ��ӹ��� �������Ʈ������ ���� ������ �� �ְ� �Ǵ� ��.
	float BatteryPower;


};
