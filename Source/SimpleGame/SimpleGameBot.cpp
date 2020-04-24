// Fill out your copyright notice in the Description page of Project Settings.



#include "SimpleGameBot.h"
#include <SimpleGame.h>
#include <SimpleGameCharacter.h>
#include <SimpleGameAIController.h>



ASimpleGameBot::ASimpleGameBot()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionShpere"));
	CollisionSphere->InitSphereRadius(10.0f);
	CollisionSphere->AttachTo(GetMesh(), "WeaponPoint");
	
	bUseControllerRotationYaw = true;

	DuringAttack = false;

	AIControllerClass = ASimpleGameAIController::StaticClass();



}




void ASimpleGameBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ASimpleGameCharacter::StaticClass()) && DuringAttack == true)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, NULL, this, UDamageType::StaticClass());

	}
}


void ASimpleGameBot::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}


void ASimpleGameBot::PlayMeleeAnim()
{
	if (!DuringAttack)
	{
		DuringAttack = true;

		float AnimDuration = PlayAnimMontage(MeleeAnim);

	}
	

	GetWorldTimerManager().SetTimer(TimerHandle_EnemyStopAttack, this, &ASimpleGameBot::StopMeleeAnim, 1, false);
	
}


void ASimpleGameBot::StopMeleeAnim()
{
	if (DuringAttack)
	{
		DuringAttack = true;

		StopAnimMontage(MeleeAnim);
	}


}