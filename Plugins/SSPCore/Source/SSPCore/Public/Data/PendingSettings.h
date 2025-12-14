// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PendingSettings.generated.h"

USTRUCT(BlueprintType)
struct FPendingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GraphicsQuality = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterVolume = -1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SFXVolume = -1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DialogueVolume = -1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Brightness = -1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResolutionIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WindowMode = -1;
	/** Returns true if at least one setting has been modified */
	bool IsValid() const
	{
		return GraphicsQuality >= 0
			|| Brightness >= 0.0f
			|| MasterVolume >= 0.0f
			|| SFXVolume >= 0.0f
			|| DialogueVolume >= 0.0f
			|| ResolutionIndex >= 0
			|| WindowMode >= 0;
	}

	/** Resets all values back to "unset" */
	void Reset()
	{
		*this = FPendingSettings{};
	}
};

