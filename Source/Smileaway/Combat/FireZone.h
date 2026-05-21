#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireZone.generated.h"

class ASmileawayCharacter;
class UNiagaraComponent;
class USoundBase;

UCLASS()
class SMILEAWAY_API AFireZone : public AActor
{
	GENERATED_BODY()

public:
	
	AFireZone();

	virtual void Tick(float DeltaTime) override;
	
	void SetTargetProgress(float InTargetProgress);

protected:
	
	virtual void BeginPlay() override;

	/**
	 * @brief Initialize parameters of fire effect to match actor parameters
	 * @note To be called in the actor blueprint's construction script
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeFireEffect();
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float DamagePerTick = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float OuterRadius = 9600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float InnerRadius = 7250.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireDensity = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float BaseExpansionRate = 0.01f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float TargetProgress = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TObjectPtr<UNiagaraComponent> FireEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TObjectPtr<USoundBase> FireSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
	TObjectPtr<USceneComponent> AudioRotationRoot;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire", meta = (ClampMin = "1.0"))
	float AudioDegreesSpacing = 15.f;
	
private:

	void DamageOverTime();
	
	void InitializeAudioRing();

	void UpdateAudioExpansion();
	
	float CurrentProgress = 0.f;
	
	int32 ActiveAudioCount = 0;
	
	UPROPERTY()
	TArray<TObjectPtr<UAudioComponent>> AudioPool;
	
	UPROPERTY(Transient)
	ASmileawayCharacter* Target = nullptr;
};