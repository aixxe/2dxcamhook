## 2dxcamhook

Hook library for replacing in-game camera textures with [Spout2](https://github.com/leadedge/Spout2) senders

### Compatibility

- beatmania IIDX 27 HEROIC VERSE
- beatmania IIDX 28 BISTROVER
- beatmania IIDX 29 CastHour
- beatmania IIDX 30 RESIDENT

<sub>※ In-game cameras are not supported in the LDJ-003 version of IIDX 30</sub>

### Configuration

#### Using OBS Studio

- Install the [Spout2 Plugin for OBS Studio](https://github.com/Off-World-Live/obs-spout2-plugin/releases/)
- Right-click a Source or Scene in OBS Studio and select "Filters"
- Press the add button under Effect Filters and select "Spout Filter"
- Set the name to "Camera A" and press the "Change Spout Filter Name" button
- Repeat the same steps for the second camera, using name "Camera B" instead

**Note:** The game expects the camera texture to be 1280x720 in size. If you have set OBS to output in a higher resolution, you should also add a "Scaling/Aspect Ratio" filter above the "Spout Filter" and set the resolution to 1280x720. It will appear incorrectly in the camera check menu, but will be normal when in-game.

### Installation

- Compile from source or download a pre-built version from the [releases](https://github.com/aixxe/2dxcamhook/releases/) page
- Copy the appropriate `2dxcamhook.dll` build to your game directory
- Alter your launch command to load the library during startup

#### [Bemanitools](https://github.com/djhackersdev/bemanitools)

```
launcher.exe [...] -K 2dxcamhook.dll -p io.disable_cams=true
```

#### [spice2x](https://spice2x.github.io)

```
spice64.exe [...] -iidxdisablecams -k 2dxcamhook.dll
```