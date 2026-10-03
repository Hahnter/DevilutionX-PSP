This release fixes the ZIP layout from v1.5.0-psp. Asset files now extract into
their required subdirectories, such as `assets/fonts/` and `assets/ui_art/`.
The EBOOT.PBP and asset file contents are identical to v1.5.0-psp; only ZIP
paths and the included installation guide changed.

Extract `devilutionx-psp-v1.5.1.zip` to `PSP/GAME/DevilutionX/`. Read
`README-PSP.txt` inside the ZIP for the required layout. If an older
`devilutionx.mpq` is already in that folder, remove it when using the packaged
`assets/` directory.

This download contains **no MPQ files**. Copy `DIABDAT.MPQ` from your own Diablo
installation; Hellfire also requires the MPQ files from your own Hellfire
installation.
