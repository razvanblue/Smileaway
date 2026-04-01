// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchProjectileNotify.h"

#include "Smileaway/Character/SmileawayCharacter.h"
#include "Smileaway/Combat/ProjectileBase.h"

void ULaunchProjectileNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	UWorld* World = MeshComp->GetWorld();
	if (!World || !ProjectileClass)
	{
		return;
	}
	
	
	AActor* Owner = MeshComp->GetOwner();
	if (const auto Character = Cast<ASmileawayCharacter>(Owner))
	{
		FTransform SocketTransform;
		if (SocketName != NAME_None && MeshComp->DoesSocketExist(SocketName))
		{
			SocketTransform = MeshComp->GetSocketTransform(SocketName, RTS_World);
		}
		else
		{
			SocketTransform = MeshComp->GetComponentTransform();
		}
		
		FVector SpawnLocation = SocketTransform.GetLocation() + SocketTransform.TransformVector(LocationOffset);
		FRotator SpawnRotation = Owner->GetActorRotation() + RotationOffset;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;
		SpawnParams.Instigator = Character;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}
