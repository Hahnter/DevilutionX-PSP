# DevilutionX for PlayStation Portable

A working port of DevilutionX to the PlayStation Portable (PSP), tested on PSP 3000 hardware and the PPSSPP emulator.

**Status**: ✅ Playable | **Performance**: ~30 FPS average on PSP 3000 | **Version**: v1.5.0-psp

---

## Quick Start

### On PSP Hardware

1. Install homebrew on your PSP (if not already done)
2. Download the latest release (EBOOT.PBP and assets folder)
3. Create folder: `ms0:/PSP/GAME/DevilutionX/`
4. Copy EBOOT.PBP and assets/ folder to that directory
5. Copy DIABDAT.MPQ from your Diablo CD or GOG to the same folder
6. Launch from PSP Games menu

### On PPSSPP Emulator

1. Download the latest release
2. Extract EBOOT.PBP and assets/ folder
3. Create a game folder and place files inside
4. Add DIABDAT.MPQ to the same folder
5. Open EBOOT.PBP in PPSSPP

### File Structure

```
PSP/GAME/DevilutionX/
├── EBOOT.PBP          (game executable)
├── DIABDAT.MPQ        (game data - from your Diablo)
├── assets/            (game resources)
│   ├── gui/
│   ├── music/
│   ├── sfx/
│   └── textures/
└── diablo.ini         (config - created automatically)
```

**Important**: DIABDAT.MPQ must be provided by you (from Diablo CD or GOG purchase).

---

## Features

✅ **Full Diablo gameplay**
- All dungeons (Cathedral, Catacombs, Caves, Hell)
- All character classes (Warrior, Rogue, Sorcerer)
- Item management and inventory
- Spell system and abilities
- Multiplayer support (over PSP network) — implemented but not extensively tested; treat as experimental

✅ **Performance optimized for PSP**
- ~30 FPS average on PSP 3000
- Texture splitting for PSP GPU limits
- Resolution scaling (640x480 → 480x272)
- Native pixel format for fast rendering

✅ **Quality of life improvements**
- Auto-pillar support
- Widescreen mode
- Customizable controls
- Save/load system

---

## Performance

**Average across gameplay on PSP 3000**: ~30 FPS, a significant improvement over the original PSP port attempt (~7 FPS).

Frame rate varies with scene complexity — town and low-enemy areas run faster, dense combat and heavy-effect areas (e.g. Hell) run slower. No per-scene FPS numbers are published here since they weren't measured with a fixed benchmarking method; treat any specific figure you see elsewhere as a rough impression, not a spec.

### Hardware Tested

- ✅ PSP 3000 (hardware) — confirmed working, ~30 FPS average
- ✅ PPSSPP emulator — confirmed working
- ⚠️ PSP 1000, PSP 2000, PSP Go — **not tested**. These share the same CPU/GPU architecture as the PSP 3000, so they should work, but this hasn't been verified on real hardware.

---

## Technical Details

### Resolution Handling

The PSP has a physical screen of 480x272 pixels, but Diablo's UI was designed for larger screens. This port:

- Renders game at 640x480 (logical resolution)
- Automatically scales to PSP's 480x272 physical display
- Uses two GPU textures (PSP GPU limit is 512px width)
- Transparent to the player - everything works normally

### Graphics Options (PSP Optimized)

These settings are forced for PSP compatibility:

```
Upscaling: ENABLED
  (Required to fit 640x480 onto 480x272 display)

Integer Scaling: DISABLED
  (Incompatible with downscaling)

Scaling Quality: NEAREST
  (Best for sprite-based graphics)

VSync: ENABLED
  (Prevents screen tearing)

FPS Limit: 30
  (Matches PSP capability, saves battery)
```

Users cannot change these settings - they're optimized for PSP hardware.

---

## Save System

### How Saves Work

- Saves are stored as `.sav` files in `PSP/GAME/DevilutionX/`
- Each character can be saved and loaded
- Multiple save files are supported
- Saves are compatible between hardware and PPSSPP

### Save Corruption

If a save file becomes corrupted (rare):

**Cause:**
- Game crashes during save operation
- Power loss while saving
- Unexpected shutdown

**This is normal** - same behavior as original Diablo on any platform.

**Solution:**
1. Delete the corrupted `.sav` file
2. Start a new game
3. Future saves will work normally

This is not a bug - it's expected behavior when a save write is interrupted.

---

## Controls

### Default PSP Controls

| Button | Action |
|--------|--------|
| **D-Pad** | Move character |
| **Triangle** | Cast spell / Use item |
| **Circle** | Attack |
| **X** | Interact / Pick up |
| **Square** | Open inventory |
| **L** | Previous spell |
| **R** | Next spell |
| **Start** | Open menu |
| **Select** | Open character info |

### Customizing Controls

In-game:
1. Go to: Options > Game > Controls
2. Select a button to remap
3. Press the PSP button you want to use
4. Changes are saved to diablo.ini

---

## Known Limitations

- **No Hellfire expansion** - Optional DLC not implemented
- **Network multiplayer** - Implemented but not extensively tested; may not work on all connections
- **Only PSP 3000 verified on hardware** - PSP 1000/2000/Go are expected to work (same architecture) but haven't been tested
- **No controller rebinding for menus** - Use touch or specific buttons
- **Performance varies** - Complex scenes may run below the ~30 FPS average

These are hardware/software limitations, not bugs.

---

## Battery Life

Not independently measured for this port. Expect battery life in the same range as other PSP homebrew of similar CPU/GPU load — actual runtime depends on your PSP model, battery condition, and screen brightness. If you have real-world numbers from your own testing, feel free to contribute them.

---

## Troubleshooting

**Game won't start:**
- Verify DIABDAT.MPQ is in correct folder
- Check file is not corrupted
- Try different memory stick (if PSP)

**Low FPS:**
- Some slowdown in complex scenes is expected on PSP hardware
- Try different areas to see typical FPS

**Save won't load:**
- See "Save Corruption" section above
- Delete the corrupted save and start fresh

**Can't hear audio:**
- Check PSP system volume
- Check game volume in Options

**Full troubleshooting guide**: See TROUBLESHOOTING.md

---

## Compatibility

### PSP Models

| Model | Status |
|-------|--------|
| PSP 1000 | ⚠️ Untested — expected to work (same architecture) |
| PSP 2000 | ⚠️ Untested — expected to work (same architecture) |
| PSP 3000 | ✅ Tested & Working |
| PSP Go | ⚠️ Untested — expected to work (same architecture) |

### Memory Requirements

- **Storage**: ~200MB (game + saves) — approximate, depends on assets and save count
- **RAM**: Uses PSP's 32MB (shared)
- **Free space needed**: At least 1MB for saves

### Emulators

- **PPSSPP**: ✅ Tested and working
- Other PSP emulators: Untested

---

## Customization

### diablo.ini Configuration

The game creates `diablo.ini` automatically. You can customize:

```ini
[Game]
Difficulty=Normal
Sound Volume=80
Music Volume=80

[Graphics]
Upscale=true
IntegerScaling=false
ScalingQuality=nearest
VSync=on
FpsLimit=30

[Audio]
SampleRate=22050
Channels=2
```

Edit directly to customize gameplay.

---

## Building from Source

See [docs/building.md](docs/building.md) for the current PSP build instructions and toolchain requirements.

---

## Contributing

Found a bug or have an improvement?

1. Test thoroughly on both PSP and PPSSPP (if possible)
2. Describe the issue clearly with:
   - What happened
   - When it happens
   - PSP model or PPSSPP version
   - Steps to reproduce
3. Post an issue or pull request

---

## Credits

- **diasurgical/DevilutionX** - Original DevilutionX project
- **dports/DevilutionX-PSP** - PSP port foundation
- **PSPDEV** - PSP development tools and SDK
- **Community** - Testing and feedback

---

## License

DevilutionX is released under the Sustainable Use License. See LICENSE.md for details.

**Non-commercial use only** - You may not charge for this or derivative works.

Diablo® is a trademark of Blizzard Entertainment, Inc. This project is not affiliated with or endorsed by Blizzard.

---

## Links

- **GitHub**: https://github.com/diasurgical/DevilutionX
- **DevilutionX Wiki**: https://github.com/diasurgical/DevilutionX/wiki
- **PSP Homebrew**: https://psp.brewology.com/

---

## Version History

**v1.5.0-psp** (Current)
- Fixed texture splitting for PSP GPU limits
- Implemented resolution scaling (640x480 → 480x272)
- Fixed graphics reinitialization
- Configuration validation for PSP
- Tested on PSP 3000 hardware and PPSSPP
- ~30 FPS average performance on PSP 3000

---

**Happy gaming!**
