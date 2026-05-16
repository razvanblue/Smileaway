#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireZone.generated.h"

class UNiagaraComponent;

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
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireDensity = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float BaseExpansionRate = 0.01f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float TargetProgress = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TObjectPtr<UNiagaraComponent> FireEffect;

private:
	
	float CurrentProgress = 0.0f;
	
	float RealExpansionRate = 1.f;
};