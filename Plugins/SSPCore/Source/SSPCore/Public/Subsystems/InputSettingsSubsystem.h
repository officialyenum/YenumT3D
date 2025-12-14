// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputSettingsSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FBindingData
{
	GENERATED_BODY()

};
/**
 * 
 */
UCLASS()
class SSPCORE_API UInputSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
};
