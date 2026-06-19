// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchCharacterNotify.h"

#include "GameFramework/Character.h"

void ULaunchCharacterNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	
	if (!MeshComp)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	FVector WorldLaunchVelocity;
	if (auto* Controller = Character->GetController(); bIsControllerRelative && Controller)
	{
		FRotator ControlYaw = Controller->GetControlRotation();
		ControlYaw.Pitch = ControlYaw.Roll = 0.f;
		Character->SetActorRotation(ControlYaw); // Make character face launch direction
		
		WorldLaunchVelocity = ControlYaw.RotateVector(LaunchVelocity);
		
	}
	else
	{
		WorldLaunchVelocity = Character->GetActorTransform().TransformVectorNoScale(LaunchVelocity);
	}

	Character->LaunchCharacter(WorldLaunchVelocity, bXYOverride, bZOverride);
	
	Super::Notify(MeshComp, Animation, EventReference);
}
