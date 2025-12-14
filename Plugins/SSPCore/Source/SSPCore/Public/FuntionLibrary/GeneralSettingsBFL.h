// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeneralSettingsBFL.generated.h"

struct FPendingSettings;
struct FUserSettingsData;
enum class EVolumeType : uint8;
class USettingsSubsystem;
UENUM(BlueprintType)
enum class EVisualSettingsType : uint8
{
	EVST_GraphicsQuality UMETA(DisplayName = "GraphicsQuality"),
	EVST_Resolution UMETA(DisplayName = "Resolution"),
	EVST_WindowMode UMETA(DisplayName = "WindowMode"),
	
	EGST_MAX UMETA(DisplayName = "DefaultMax"),
};
/**
 * 
 */
UCLASS()
class SSPCORE_API UGeneralSettingsBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static USettingsSubsystem* GetSettingsSubsystem(const UObject* WorldContextObject);
	// -------------------- Init --------------------
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings", meta=(WorldContext="WorldContextObject"))
	static void InitSettings(const UObject* WorldContextObject, USoundMix* SoundMix);
	// -------------------- Resolution --------------------
	
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Display", meta=(WorldContext="WorldContextObject"))
	static void UpdateVisualSettings(const UObject* WorldContextObject, EVisualSettingsType Type, int32 Level);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Volume", meta=(WorldContext="WorldContextObject"))
	static void UpdateVolumeSettings(const UObject* WorldContextObject, EVolumeType Type, float Level);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Brightness", meta=(WorldContext="WorldContextObject"))
	static void UpdateBrightnessSettings(const UObject* WorldContextObject, float Value);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings", meta=(WorldContext="WorldContextObject"))
	static void SaveSettings(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings", meta=(WorldContext="WorldContextObject"))
	static void SaveSoundSettings(const UObject* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClass, EVolumeType SoundType);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings", meta=(WorldContext="WorldContextObject"))
	static void ResetSettingsToDefault(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Display", meta=(WorldContext="WorldContextObject"))
	static TArray<FString> GetAvailableResolutions(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Display")
	static TArray<FString> GetGraphics();
	
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Display", meta=(WorldContext="WorldContextObject"))
	static int32 GetCurrentResolutionIndex(const UObject* WorldContextObject);

	// -------------------- Window Mode --------------------

	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Display")
	static TArray<FString> GetWindowModes();

	UFUNCTION(BlueprintCallable, Category="Yenum|Settings|Display")
	static int32 GetCurrentWindowMode();

	
	// -------------------- Data --------------------
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings", meta=(WorldContext="WorldContextObject"))
	static FUserSettingsData GetDefaultSettingsData(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category="Yenum|Settings", meta=(WorldContext="WorldContextObject"))
	static FPendingSettings GetPendingSettingsData(const UObject* WorldContextObject);
	
};
