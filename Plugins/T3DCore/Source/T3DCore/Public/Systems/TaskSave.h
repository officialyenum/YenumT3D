// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TaskSave.generated.h"

/**
 * 
 */
UCLASS()
class T3DCORE_API UTaskSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName SavedTaskID = NAME_None;

	UPROPERTY()
	int32 SavedCurrentObjectiveIndex = 0;

	UPROPERTY()
	int32 SavedCurrentObjectiveCount = 0;
};
