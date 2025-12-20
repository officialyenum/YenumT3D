# 🎮 UE5 Game Jam Settings Suite

A **designer-friendly Unreal Engine 5 game jam template and plugin suite** that exposes **Audio, Visual, and Input settings** entirely through Blueprints.

No engine digging. No C++. Just plug, build, play, and ship.


## 🧭 Choose How You Want to Use It

This project is designed to work in **three different ways** depending on your needs:

## Option 1 — Full Game Jam Template (Recommended)

**Best for:**
- Game jams
- Prototypes
- Small teams
- Designers who want everything ready

### Includes:
- ✅ Complete UE5 project
- ✅ Settings menu structure
- ✅ Audio & visual settings
- ✅ Input rebinding system
- ✅ Save / apply / reset workflows
- ✅ Clean UI-ready Blueprint APIs

### How to use:
1. Clone or download the repository
2. Open the `.uproject`
3. Start building your game

---

## Option 2 — General Settings Plugin (Audio + Visual)

**Best for:**
- Existing projects
- Teams that already have UI
- Designers who only need display & sound control

### Features:
- Graphics quality
- Resolution
- Window mode
- Brightness
- Audio volume (Master, SFX, Dialogue)
- Save & reset support

### Install as Plugin:
```

YourProject/
└── Plugins/
└── GeneralSettingsPlugin/

```

Enable the plugin and restart the editor.


## Option 3 — Input Rebinding Plugin

**Best for:**
- Accessibility
- PC & controller-first games
- Custom control layouts

### Features:
- Keyboard rebinding
- Gamepad rebinding
- Player-mappable actions
- Save & restore bindings
- Blueprint-driven UI workflow

📌 *Can be used standalone or together with General Settings Plugin.*



## 🧩 Plugin Compatibility

| Plugin             | Standalone | Works Together |
|--------------------|----------- |----------------|
| General Settings   | ✅         | ✅            |
| Input Rebinding    | ✅         | ✅            |
| Game Jam Template  |            | Includes all   |

---

## 🔧 Initialization (Required)

Call once on startup (GameInstance or Main Menu):

```text
InitSettings(WorldContextObject, SoundMix)
````
<img width="624" height="463" alt="Screenshot 2025-12-20 171618" src="https://github.com/user-attachments/assets/35fe5318-795b-48df-8275-e5b7c440a89c" />


Required for:

* Audio system
* Saved settings loading

---

## 🖥 Visual Settings (Blueprint)

### Update

```text
UpdateVisualSettings
```
<img width="1012" height="567" alt="Screenshot 2025-12-20 171815" src="https://github.com/user-attachments/assets/33b6fe7a-fcdc-4d62-b809-265819b90d72" />

**Types**

* GraphicsQuality
* Resolution
* WindowMode


### Populate UI

```text
GetAvailableResolutions
GetGraphics
GetWindowModes
```


### Get Current Values

```text
GetCurrentResolutionIndex
GetCurrentGraphicsIndex
GetCurrentWindowMode
```
<img width="1142" height="356" alt="Screenshot 2025-12-20 172008" src="https://github.com/user-attachments/assets/aa1daf38-4b85-436d-a0a6-39ee970024b7" />
<img width="1057" height="400" alt="Screenshot 2025-12-20 172001" src="https://github.com/user-attachments/assets/631bdd93-9e3e-46b5-b20a-1bd1117ea5a3" />
<img width="944" height="380" alt="Screenshot 2025-12-20 171954" src="https://github.com/user-attachments/assets/5944fe22-e0f6-48fc-87a2-2094b1fa6668" />

---

## 🔊 Audio Settings

### Update Volume

```text
UpdateVolumeSettings
```

### Save Sound

```text
SaveSoundSettings
```
<img width="818" height="676" alt="Screenshot 2025-12-20 172402" src="https://github.com/user-attachments/assets/16a5ac11-9bfa-4daf-8cac-4e02582fb961" />

Supports:

* Master
* SFX
* Dialogue

---

## 🎮 Input Rebinding (Plugin)

### Typical UI Flow

1. Display current binding
2. Listen for input
3. Apply mapping
4. Save input profile

All exposed via **Blueprint events and functions**.

---

## 💾 Save & Reset

```text
SaveSettings
ResetSettingsToDefault
```

Works across:

* Audio
* Visuals
* Input bindings

---

## 🧠 Designed for Designers

✔ No engine subsystems exposed
✔ No console commands
✔ No C++ required
✔ UMG-ready APIs

---

## 📦 Requirements

* Unreal Engine **5.6**
* SoundMix & SoundClass assets (for audio)
* Enhanced Input (for rebinding plugin)

---

## 🚀 Roadmap

* Preset profiles
* Accessibility presets
* Platform-specific defaults
* UI example widgets

---

## 📄 License

MIT License
© 2025 Chukwuyenum Opone
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  
 Author: Chukwuyenum Opone
 GitHub: https://github.com/officialyenum
 Email: oponechukwuyenum@gmail.com

---

## 🛒 Marketplace / Fab

This project is packaged as:

* ✔ Full Project Template
* ✔ Individual Plugins
* ✔ Bundle-friendly

Simply add to your project and enable.

---

Happy shipping! 🚢

---

# 3️⃣ Fab / Marketplace Readiness Checklist

### ✅ Required for Plugins
- `uplugin` description filled
- Version number
- Engine version compatibility
- No project-only dependencies
- Clean `Public / Private` separation
- No unused assets

### ✅ Required for Project Template
- Clear `README`
- Default map set
- No missing references
- Plugins disabled unless required

---

# 4️⃣ Repo Structure

```text
GameJamSettingsSuite/
├── GameJamTemplate/
│   └── GameJamTemplate.uproject
│
├── Plugins/
│   ├── GeneralSettingsPlugin/
│   └── InputRebindingPlugin/
│
├── Docs/
│   ├── Screenshots/
│   └── Blueprints/
│
└── README.md
````
📣 Feedback & Collaboration
This is a personal development project, but i am always open to feedback, ideas, or potential collaboration. If you're into Unreal games, feel free to open an issue, star or fork the project!
