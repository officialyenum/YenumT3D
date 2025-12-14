// ============================================================================
// Copyright (c) 2025 Chukwuyenum Opone
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author: Chukwuyenum Opone
// GitHub: https://github.com/officialyenum
// Email: oponechukwuyenum@gmail.com
// ============================================================================

#include "FuntionLibrary/GeneralSettingsBFL.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/SettingsSubsystem.h"

/**
 * UGeneralSettingsBFL
 *
 * Blueprint Function Library that provides UI-friendly access to the
 * USettingsSubsystem. This allows UMG widgets to query display and
 * graphics settings without directly interacting with subsystems.
 *
 * Intended Usage:
 * - UI widgets (Settings menus, Main menu, Pause menu)
 * - Blueprint-only projects that still want subsystem-driven logic
 *
 * This library does NOT store or apply settings itself.
 * It forwards all logic to USettingsSubsystem.
 */

/**
 * Retrieves the active USettingsSubsystem from the current game instance.
 *
 * @param WorldContextObject
 *      Any UObject that can provide a valid UWorld context.
 *      Typically:
 *          - UUserWidget (Self)
 *          - AActor
 *          - PlayerController
 *
 * @return
 *      Pointer to the USettingsSubsystem if found.
 *      Returns nullptr if:
 *          - WorldContextObject is invalid
 *          - World is not available
 *          - GameInstance is not initialized
 *
 * @usage
 *      Internal helper function.
 *      Not exposed to Blueprint.
 */
USettingsSubsystem* UGeneralSettingsBFL::GetSettingsSubsystem(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	UGameInstance* GI = World->GetGameInstance();
	if (!GI)
	{
		return nullptr;
	}

	return GI->GetSubsystem<USettingsSubsystem>();
}

void UGeneralSettingsBFL::InitSettings(const UObject* WorldContextObject, USoundMix* SoundMix)
{
	UGameplayStatics::SetBaseSoundMix(WorldContextObject, SoundMix);
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		FSubsystemCollection<USettingsSubsystem> Collection;
		Sub->Initialize(Collection);
	}
}

/**
 * Queues a visual-related setting change without immediately applying it.
 *
 * This function updates the pending visual settings stored in the
 * USettingsSubsystem. The change will NOT take effect until
 * SaveSettings() is called.
 *
 * Supported visual settings:
 * - Graphics Quality
 * - Screen Resolution
 * - Window Mode
 *
 * @param WorldContextObject
 *      Any UObject that provides a valid world context.
 *      Typically the calling UUserWidget (Self).
 *
 * @param Type
 *      Specifies which visual setting to update.
 *      Values come from EVisualSettingsType:
 *          - EVST_GraphicsQuality → Overall scalability level
 *          - EVST_Resolution      → Screen resolution index
 *          - EVST_WindowMode      → Window mode index
 *
 * @param Level
 *      Integer value representing the selected option.
 *      Meaning depends on the setting type:
 *          - Graphics Quality → Scalability level (0–3 or 0–4)
 *          - Resolution       → Index from GetAvailableResolutions()
 *          - Window Mode      → 0 = Fullscreen, 1 = Windowed, 2 = Borderless
 *
 * @note
 *      This function does NOT apply or save settings.
 *      Call SaveSettings() to apply changes.
 *
 * @usage (Blueprint)
 *      OnSelectionChanged (ComboBox)
 *      → UpdateVisualSettings
 */
void UGeneralSettingsBFL::UpdateVisualSettings(const UObject* WorldContextObject, EVisualSettingsType Type, int32 Level)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		switch (Type) {
		case EVisualSettingsType::EVST_GraphicsQuality:
			Sub->QueueGraphicsQuality(Level);
			break;
		case EVisualSettingsType::EVST_Resolution:
			Sub->QueueResolution(Level);
			break;
		case EVisualSettingsType::EVST_WindowMode:
			Sub->QueueWindowMode(Level);
			break;
		default: ;
		}
	}
}

/**
 * Queues an audio volume change without immediately applying it.
 *
 * This function updates the pending volume settings in the
 * USettingsSubsystem. Changes will not affect audio output
 * until SaveSettings() is called.
 *
 * @param WorldContextObject
 *      Any UObject that provides a valid world context.
 *      Usually the calling UUserWidget (Self).
 *
 * @param Type
 *      Specifies which volume channel to update.
 *      Values come from EVolumeType:
 *          - EVT_Master   → Master volume
 *          - EVT_SFX      → Sound effects volume
 *          - EVT_Dialogue → Dialogue volume
 *
 * @param Level
 *      Volume value in the range 0.0 – 1.0.
 *      Values outside this range may be clamped internally.
 *
 * @note
 *      Audio changes are queued only.
 *      Call SaveSettings() to apply and persist them.
 *
 * @usage (Blueprint)
 *      OnValueChanged (Slider)
 *      → UpdateVolumeSettings
 */
void UGeneralSettingsBFL::UpdateVolumeSettings(const UObject* WorldContextObject, EVolumeType Type, float Level)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		Sub->QueueVolume(Level, Type);
	}
}

/**
 * Queues a brightness (gamma) change without immediately applying it.
 *
 * Internally, brightness is applied using Unreal Engine's gamma
 * correction system. The value will not take effect until
 * SaveSettings() is called.
 *
 * @param WorldContextObject
 *      Any UObject that provides a valid world context.
 *
 * @param Value
 *      Brightness value (gamma multiplier).
 *      Recommended range:
 *          0.5 – 3.0
 *
 * @note
 *      This function only queues the brightness change.
 *      Call SaveSettings() to apply it.
 *
 * @usage (Blueprint)
 *      OnValueChanged (Slider)
 *      → UpdateBrightnessSettings
 */
void UGeneralSettingsBFL::UpdateBrightnessSettings(const UObject* WorldContextObject, float Value)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		Sub->QueueBrightness(Value);
	}
}

/**
 * Applies and saves all queued settings.
 *
 * This function commits all pending changes stored in the
 * USettingsSubsystem and applies them to the engine.
 * Settings are also saved so they persist between game sessions.
 *
 * Affected systems:
 * - Graphics quality
 * - Resolution & window mode
 * - Brightness (gamma)
 * - Audio volumes
 *
 * @param WorldContextObject
 *      Any UObject that provides a valid world context.
 *
 * @note
 *      This should typically be called when the user presses
 *      an "Apply" or "Save" button in the settings menu.
 *
 * @usage (Blueprint)
 *      OnClicked (Apply Button)
 *      → SaveSettings
 */
void UGeneralSettingsBFL::SaveSettings(const UObject* WorldContextObject)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		Sub->ApplySettings();
	}
}

void UGeneralSettingsBFL::SaveSoundSettings(const UObject* WorldContextObject, USoundMix* SoundMix,
	USoundClass* SoundClass, EVolumeType SoundType)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		Sub->ApplySoundSettings(SoundMix, SoundClass, SoundType);
	}
}

/**
 * Resets all settings to their default values and applies them immediately.
 *
 * This function restores predefined default settings
 * (graphics, audio, brightness, etc.), applies them to the engine,
 * and saves them for future sessions.
 *
 * @param WorldContextObject
 *      Any UObject that provides a valid world context.
 *
 * @note
 *      This action overrides any queued changes.
 *      Defaults are defined in USettingsSubsystem.
 *
 * @usage (Blueprint)
 *      OnClicked (Reset Button)
 *      → ResetSettingsToDefault
 */
void UGeneralSettingsBFL::ResetSettingsToDefault(const UObject* WorldContextObject)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		Sub->ResetToDefaults();
	}
}

/**
 * Returns a list of display resolutions supported by the current system.
 *
 * This function is intended for populating resolution dropdowns
 * in UMG settings menus.
 *
 * @param WorldContextObject
 *      Any UObject that provides a valid world context.
 *      Usually the calling UUserWidget (Self).
 *
 * @return
 *      Array of formatted resolution strings.
 *      Example:
 *          ["1920 x 1080", "2560 x 1440", "3840 x 2160"]
 *
 *      Returns an empty array if the settings subsystem is unavailable.
 *
 * @usage (Blueprint)
 *      GetAvailableResolutions(Self)
 *      → ForEachLoop
 *      → AddOption (ComboBoxString)
 */
TArray<FString> UGeneralSettingsBFL::GetAvailableResolutions(const UObject* WorldContextObject)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		return Sub->GetAvailableResolutions();
	}
	return {};
}

/**
 * Returns the list of supported Graphics.
 *
 * Index mapping:
 *      0 → Low
 *      1 → Medium
 *      2 → High
 *      3 → Epic
 *
 * @return
 *      Array of graphic display strings.
 *
 * @usage (Blueprint)
 *      GetGraphics()
 *      → AddOption (ComboBoxString)
 */
TArray<FString> UGeneralSettingsBFL::GetGraphics()
{
	return {
		TEXT("Low"),
		TEXT("Medium"),
		TEXT("High"),
		TEXT("Epic")
	};
}

/**
 * Returns the index of the currently active screen resolution.
 *
 * The returned index matches the position of the resolution
 * within the array returned by GetAvailableResolutions().
 *
 * @param WorldContextObject
 *      Any UObject that provides a valid world context.
 *
 * @return
 *      Index of the active resolution.
 *      Returns INDEX_NONE (-1) if:
 *          - Resolution is not found
 *          - Settings subsystem is unavailable
 *
 * @usage (Blueprint)
 *      GetCurrentResolutionIndex(Self)
 *      → SetSelectedIndex (ComboBoxString)
 */
int32 UGeneralSettingsBFL::GetCurrentResolutionIndex(const UObject* WorldContextObject)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		return Sub->GetCurrentResolutionIndex();
	}
	return INDEX_NONE;
}

/**
 * Returns the list of supported window modes.
 *
 * Index mapping:
 *      0 → Fullscreen
 *      1 → Windowed
 *      2 → Borderless (Windowed Fullscreen)
 *
 * @return
 *      Array of window mode display strings.
 *
 * @usage (Blueprint)
 *      GetWindowModes()
 *      → AddOption (ComboBoxString)
 */
TArray<FString> UGeneralSettingsBFL::GetWindowModes()
{
	return {
		TEXT("Fullscreen"),
		TEXT("Windowed"),
		TEXT("Borderless")
	};
}

/**
 * Returns the currently active window mode as an index.
 *
 * Index mapping:
 *      0 → Fullscreen
 *      1 → Windowed
 *      2 → Borderless (Windowed Fullscreen)
 *
 * @return
 *      Index representing the active window mode.
 *      Defaults to Windowed (1) if engine settings are unavailable.
 *
 * @usage (Blueprint)
 *      GetCurrentWindowMode()
 *      → SetSelectedIndex (ComboBoxString)
 */
int32 UGeneralSettingsBFL::GetCurrentWindowMode()
{
	if (!GEngine)
	{
		return 1; // default Windowed
	}
	switch (GEngine->GetGameUserSettings()->GetFullscreenMode())
	{
	case EWindowMode::Fullscreen:
		return 0;
	case EWindowMode::Windowed:
		return 1;
	case EWindowMode::WindowedFullscreen:
		return 2;
	default:
		return 1;
	}
}

FUserSettingsData UGeneralSettingsBFL::GetDefaultSettingsData(const UObject* WorldContextObject)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		return Sub->GetDefaultSettings();
	}
	return FUserSettingsData();
}

FPendingSettings UGeneralSettingsBFL::GetPendingSettingsData(const UObject* WorldContextObject)
{
	if (USettingsSubsystem* Sub = GetSettingsSubsystem(WorldContextObject))
	{
		return Sub->GetCurrentSettingsData();
	}
	return FPendingSettings();
}
