// Fill out your copyright notice in the Description page of Project Settings.


#include "HitInterface.h"
#include "GenericTeamAgentInterface.h"
// Add default functionality here for any IHitInterface functions that are not pure virtual.

bool IHitInterface::CanDamage(AActor* Source, AActor* Target)
{
    if (!Target || Source == Target) // Assume Source is valid
        return false;

    const IGenericTeamAgentInterface* SourceTeam = Cast<IGenericTeamAgentInterface>(Source);
    const IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(Target);

    if (SourceTeam && TargetTeam)
    {
        return SourceTeam->GetGenericTeamId() != TargetTeam->GetGenericTeamId();
    }

    // If no team info
    return true;
}
