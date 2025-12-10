// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "T3DTaskData.generated.h"

UENUM(BlueprintType)
enum class ET3DTaskType : uint8
{
	ReachLocation,
	KillEnemy,
	CollectItem
};

USTRUCT(BlueprintType)
struct FT3DTask
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FName TaskName;

	UPROPERTY(EditAnywhere)
	ET3DTaskType TaskType;

	// For Kill/Collect targets: number required
	UPROPERTY(EditAnywhere)
	int32 TargetCount = 1;

	// Optional: location for ReachLocation (designer fills in editor using vector)
	UPROPERTY(EditAnywhere)
	FVector TargetLocation = FVector::ZeroVector;

	// Optional description
	UPROPERTY(EditAnywhere)
	FText Description;
};
/**
 * 
 */
UCLASS()
class T3DCORE_API UT3DTaskData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	FName TaskID;

	UPROPERTY(EditAnywhere)
	FText TaskName;

	UPROPERTY(EditAnywhere)
	TArray<FT3DTask> Tasks;
};
