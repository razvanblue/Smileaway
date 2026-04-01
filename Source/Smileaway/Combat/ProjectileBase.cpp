// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Smileaway/Interfaces/HitInterface.h"


AProjectileBase::AProjectileBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);
	Collision->SetNotifyRigidBodyCollision(true);
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
	Collision->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = Collision;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->ProjectileGravityScale = 0.5f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(300, 0, 300));
}


void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBase::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	Explode();
	
	ProjectileMovement->StopMovementImmediately();
	Destroy();
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
	
	ProjectileMovement->StopMovementImmediately();
	Destroy();
}

void AProjectileBase::Explode()
{
	const FVector Location = GetActorLocation();
	const FRotator Rotation = {0.f, GetActorRotation().Yaw, 0.f};

	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), ExplosionEffect, Location, Rotation);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, Location);
	}
	
	TriggerHitbox();
}

void AProjectileBase::TriggerHitbox()
{
	const auto Location = GetActorLocation();
	const auto Forward = GetActorForwardVector();
	
	FCollisionShape Box = FCollisionShape::MakeBox(FVector{0.f, AttackData.HitboxHalfWidth, AttackData.HitboxHalfHeight});
	TArray<FHitResult> Hits;
	// GetWorld()->SweepMultiByChannel(
	// 	Hits,
	// 	Location + Forward * AttackData.RangeMin,
	// 	Location + Forward * AttackData.RangeMax,
	// 	GetActorRotation().Quaternion(),
	// 	ECC_GameTraceChannel1,
	// 	Box
	// );
	TArray<AActor*> ActorsToIgnore = {this, GetOwner()};
	UKismetSystemLibrary::BoxTraceMulti(
		this,
		Location + Forward * AttackData.RangeMin,
		Location + Forward * AttackData.RangeMax,
		Box.GetBox(),
		GetActorRotation(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Hits,
		true
	);
	
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("Hit something!"));
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UHitInterface::StaticClass()))
		{
			IHitInterface::Execute_GetHit(HitActor, Hit.ImpactPoint, AttackData.AttackMultiplier, GetOwner());
		}
	}
}
