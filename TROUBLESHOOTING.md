# DevilutionX PSP - Troubleshooting Guide

Common issues and solutions for running DevilutionX on PSP.

---

## Save File Issues

### Problem: "Save file corrupted" or Save won't load

**Symptoms:**
- Game saved successfully before
- Now loading that save causes crash
- New games still work fine

**Cause:**
Save file can become corrupted if:
- Game crashes during save operation
- Power loss while saving
- Unexpected shutdown

This is **normal behavior** - same as original Diablo and other PSP games.

**Solution:**
1. Delete the corrupted save file
   - On PSP: Navigate to `PSP/GAME/DevilutionX/` and delete the `.sav` file
   - On PPSSPP: Find the save in the Memory Stick folder and delete it
2. Start a new game
3. Future saves will work normally

### Problem: Can't save at all

**Symptoms:**
- Game won't let you save
- Or save button does nothing

**Possible Causes:**
1. Storage is full
   - Check PSP memory stick available space
   - Need at least 1MB free

2. Wrong folder location
   - Saves must be in: `PSP/GAME/DevilutionX/`
   - Not in root or other directories

3. File permissions
   - On PSP: Memory stick might be corrupted
   - Solution: Plug into PC, check for errors

**Solution:**
- Free up space on memory stick
- Verify `PSP/GAME/DevilutionX/` folder exists
- Try saving with a simple name (no special characters)

---

## Performance Issues

### Problem: Game is slow / Low FPS

**Symptoms:**
- Game runs slower than expected
- Choppy or laggy gameplay

**Note:** Average FPS is 30 on PSP 3000, which is normal for this hardware.

**If below 30 FPS:**

1. Check graphics settings
   ```
   Graphics > Upscale: ON
   Graphics > Integer Scaling: OFF
   Graphics > Scaling Quality: Nearest
   Graphics > VSync: ON
   Graphics > FPS Limit: 30
   ```

2. Close other background apps (if on PSP)

3. Try different scenes - performance varies by area

4. Complex scenes (many enemies + effects) will be slower

---

## Game Won't Start

### Problem: Game crashes on startup

**Symptoms:**
- Black screen after DevilutionX splash
- Returns to PSP menu with error

**Solutions:**

1. **Check DIABDAT.MPQ exists**
   - File must be in: `PSP/GAME/DevilutionX/DIABDAT.MPQ`
   - Case sensitive on some systems
   - File must be complete and valid

2. **Memory stick not formatted correctly**
   - On PSP: Settings > System Settings > Format Memory Stick
   - Warning: This deletes everything!

3. **Try a different build**
   - If you have an older version, try that
   - Might be a specific build issue

4. **Check for disk errors (PPSSPP)**
   - In PPSSPP settings, enable "Disk check" mode

---

## Controls Not Working

### Problem: Buttons don't respond

**Symptoms:**
- Pressed button but nothing happens
- Some buttons work, some don't

**Solutions:**

1. **Check game settings**
   - Go to: Options > Game > Controls
   - Verify buttons are mapped correctly

2. **PSP buttons might need remapping**
   - Touch button might not work in menu
   - Try D-Pad or other buttons

3. **Controller not connected (PPSSPP)**
   - In PPSSPP: Settings > Controls
   - Verify controller is selected and mapped

---

## Graphics Issues

### Problem: Graphics glitches / Texture problems

**Symptoms:**
- Graphical artifacts
- Missing textures
- Color issues

**Note:** This is rare with the current build. If you encounter it:

**Solutions:**

1. **Disable upscaling temporarily**
   - Graphics > Upscale: OFF
   - See if it helps

2. **Reinitialize graphics**
   - Go back to main menu (loses current game)
   - Load again

3. **Restart game completely**
   - Close and reopen DevilutionX

4. **Report it!**
   - Post issue with:
     - What happened
     - Where in game it occurred
     - PSP model or PPSSPP version

---

## Audio Issues

### Problem: No sound or crackling audio

**Symptoms:**
- Music/sound effects don't play
- Audio is distorted or crackling

**Solutions:**

1. **Check volume**
   - PSP system volume is low
   - Game volume in options might be off

2. **Disable audio if it helps**
   - Options > Game > Sound Volume: 0
   - Continue without audio

3. **Audio quality on PPSSPP**
   - PPSSPP Settings > Audio
   - Try different audio settings

---

## Installation Issues

### Problem: Can't get files onto PSP

**Symptoms:**
- EBOOT.PBP won't copy
- Files won't show up on memory stick

**Solutions:**

1. **Check USB connection**
   - PSP in USB mode
   - Computer recognizes the memory stick

2. **Right folder structure**
   - Correct: `PSP/GAME/DevilutionX/EBOOT.PBP`
   - Incorrect: `PSP/GAME/EBOOT.PBP` (missing folder)
   - Incorrect: `EBOOT.PBP` (at root)

3. **File isn't corrupted**
   - Re-download EBOOT.PBP
   - Make sure download completed

4. **Memory stick issue**
   - Try formatting on PSP
   - Or use different memory stick

---

## PPSSPP-Specific Issues

### Problem: Game runs differently in PPSSPP vs PSP hardware

**Symptoms:**
- Works great in PPSSPP, crashes on PSP
- Or vice versa
- Performance different

**Solutions:**

PPSSPP is an emulator, not exact hardware. Differences are normal.

1. **GPU Backend (PPSSPP)**
   - Try: Settings > Graphics > Backend
   - Vulkan vs OpenGL might differ
   - Try both

2. **CPU Settings (PPSSPP)**
   - Settings > System > CPU Core
   - Some games prefer specific cores

3. **Accuracy vs Speed (PPSSPP)**
   - Settings > System > Accuracy
   - "Compatibility" works better for most games

---

## Still Having Issues?

### Before Reporting

1. Try the solutions above
2. Delete and reinstall if corrupted
3. Test on both PSP and PPSSPP (if possible)
4. Check that files are in correct location

### When Reporting

Include:
- **What happened** (be specific)
- **When it happens** (menu, gameplay, specific action)
- **PSP model** or **PPSSPP version**
- **What you tried** to fix it
- **Error code** if shown (like 80020148)

This helps fix the issue faster!

---

## Quick Checklist

Before assuming it's a bug:

- [ ] DIABDAT.MPQ is in correct folder
- [ ] Graphics settings are correct
- [ ] Enough space on memory stick
- [ ] Game is up to date version
- [ ] Tried restarting game
- [ ] (For PSP) Memory stick not corrupted
- [ ] (For PPSSPP) GPU settings adjusted

Most issues are environmental, not bugs!
