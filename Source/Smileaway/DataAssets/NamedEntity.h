#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NamedEntity.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API UNamedEntity : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Display")
	FText Name;

	UPROPERTY(EditDefaultsOnly, Category = "Display")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, Category = "Display")
	TObjectPtr<UTexture2D> Icon;
};
