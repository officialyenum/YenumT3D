// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PendingSettings.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FUserSettingsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GraphicsQuality = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Brightness = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SFXVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DialogueVolume = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WindowMode = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResolutionIndex = 0;

	FPendingSettings GetAsPendingSettings() const
	{
		FPendingSettings PendingData;
		PendingData.GraphicsQuality = GraphicsQuality;
		PendingData.Brightness = Brightness;
		PendingData.MasterVolume = MasterVolume;
		PendingData.SFXVolume = SFXVolume;
		PendingData.DialogueVolume = DialogueVolume;
		PendingData.WindowMode = WindowMode;
		PendingData.ResolutionIndex = ResolutionIndex;
		return PendingData;
	};
};
/**
 * 
 */
UCLASS()
class SSPCORE_API USettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUserSettingsData Data;

	void SaveData(const FPendingSettings& PendingSettingsData);
};
