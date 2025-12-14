// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SettingsSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SettingsSubsystem.generated.h"


UENUM(BlueprintType)
enum class EVolumeType : uint8
{
	EVT_Master UMETA(DisplayName = "Master"),
	EVT_SFX UMETA(DisplayName = "SFX"),
	EVT_Dialogue UMETA(DisplayName = "Dialogue"),
	
	EVT_MAX UMETA(DisplayName = "DefaultMax"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsQueued);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsInitialized, FPendingSettings, InitSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsApplied, USettingsSaveGame*, AppliedSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundSettingsApplied, EVolumeType, SoundType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsReverted, FPendingSettings, RevertSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsResetToDefaults, FPendingSettings, DefaultSettings);

/**
 * 
 */
UCLASS()
class SSPCORE_API USettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// EVENTS
	/** Fired when any setting is queued but not yet applied */
	UPROPERTY(BlueprintAssignable, Category="Settings|Events")
	FOnSettingsQueued OnSettingsQueued;
	
	/** Fired when any setting is retrieved from save game or initialized */
	UPROPERTY(BlueprintAssignable, Category="Settings|Events")
	FOnSettingsInitialized OnSettingsInitialized;
	
	/** Fired after settings are applied and saved */
	UPROPERTY(BlueprintAssignable, Category="Settings|Events")
	FOnSettingsApplied OnSettingsApplied;
	
	/** Fired when sound setting is applied */
	UPROPERTY(BlueprintAssignable, Category="Settings|Events")
	FOnSoundSettingsApplied OnSoundSettingsApplied;

	/** Fired when pending settings are discarded */
	UPROPERTY(BlueprintAssignable, Category="Settings|Events")
	FOnSettingsReverted OnSettingsReverted;

	/** Fired when settings are reset to default values */
	UPROPERTY(BlueprintAssignable, Category="Settings|Events")
	FOnSettingsResetToDefaults OnSettingsResetToDefaults;
	
	// Will be used by UI but not saved until apply
	// Graphics
	void QueueGraphicsQuality(int32 Level);
	void QueueResolution(int32 Index);
	void QueueWindowMode(int32 Index);
	//Volume - Master, SFX & Dialogue
	void QueueVolume(float Value, EVolumeType VolumeType);
	// Brightness
	void QueueBrightness(float Value);

	// Apply pending changes
	UFUNCTION()
	void ApplySettings();
	UFUNCTION()
	void ApplySoundSettings(USoundMix* SoundMix, USoundClass* SoundClass, EVolumeType SoundType) const;

	// Cancel pending changes (restore current applied settings)
	UFUNCTION()
	void RevertPendingSettings();
	UFUNCTION()
	void ResetToDefaults();

	
	// For UI initial values
	UFUNCTION()
	static TArray<FString> GetAvailableResolutions();
	UFUNCTION()
	TArray<FString> GetWindowModes() const;
	UFUNCTION()
	int32 GetCurrentResolutionIndex() const;
	UFUNCTION()
	static int32 GetCurrentGraphics();
	UFUNCTION()
	static int32 GetCurrentWindowMode();
	UFUNCTION()
	FPendingSettings GetCurrentSettingsData() const;

	
	UFUNCTION()
	FORCEINLINE FUserSettingsData GetDefaultSettings() const { return DefaultSettings; };
	UFUNCTION()
	FORCEINLINE FPendingSettings GetPendingSettings() const { return PendingSettings; };
	UFUNCTION()
	FORCEINLINE USettingsSaveGame* GetSaveGame() const { return SaveGame; };
private:

	UPROPERTY(VisibleAnywhere)
	bool bHasPendingChanges = false;
	// Save Settings to Data Store
	const FUserSettingsData DefaultSettings;
	FPendingSettings PendingSettings;

	UPROPERTY(VisibleAnywhere)
	USettingsSaveGame* SaveGame;
};
