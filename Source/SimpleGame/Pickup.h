// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


UCLASS()
class SIMPLEGAME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	/** Return the mesh for the pickup */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	/** Return whether or not the pickup is active */
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	/** Allows other classes to safely change whether or not pickup is active */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);

	// 아이템을 먹었을 때 호출할 함수
	UFUNCTION(BlueprintNativeEvent) //블루프린트 네이티브이기 때문에, 임플멘테이션의 정의를 해놓아야함. 버추얼 함수여야하고, 자손 클래스에서 오버라이딩 가능해야함.
	void WasCollected();
	virtual void WasCollected_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** True when the pickup can be used, and false when pickup is deactivatd */
	bool bIsActive;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	/** Static mesh to represent the pickup in the level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PickupMesh;

};
