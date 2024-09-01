# My dotfiles

Configuration files, scripts and modules I use with my [Arch](https://archlinux.org/) and [Hyprland](https://hyprland.org/) install :
- [Rofi](https://davatorium.github.io/rofi/) application launcher
- [Dunst](https://dunst-project.org/) notifications with volume, mic and brightness bars
- [Waybar](https://github.com/Alexays/Waybar) to display system informations with bluetooth and wifi control (WIP)
- Media keys : volume, mic, brightness, lock, screenshot (+ screencast) and more
- Configurable theme switcher for GTK3 and GTK4 apps, Dunst, Rofi and wallpaper (using [hyprpaper](https://wiki.hyprland.org/Hypr-Ecosystem/hyprpaper/))

## Configure dark and light themes

- Modify [`~/.config/waybar/config`](./waybar/config):
```json
"cffi/theme-switcher": {
    // ...

    "dark_theme_label": "Dark 🌕",
    "dark_theme_gtk3": "adw-gtk3-dark",
    "dark_theme_rofi": "path/to/theme.rasi",
    "dark_theme_wallpaper": "/absolute/path/to/dark.png",

    "light_theme_label": "Light ☀️",
    "light_theme_gtk3": "adw-gtk3",
    "light_theme_rofi": "path/to/theme.rasi",
    "light_theme_wallpaper": "/absolute/path/to/light.png",
}
```

- Modify [`~/.config/hypr/hyprpaper.conf`](./hypr/hyprpaper.conf):
```ini
preload = ~/path/to/bg/light.jpg
preload = ~/path/to/bg/dark.jpg
wallpaper = , ~/path/to/parent/bg_$MODE.jpg
```

> [!WARNING]
> Your wallpapers must share the same prefix and end with `_light.jpg` and `_dark.jpg`

## Dependencies

| Group                | Required                                                                             | Optionnal                         |
| :------------------- | :----------------------------------------------------------------------------------- | :-------------------------------- |
| System               | `waybar` `rofi` `dunst` `xdg-open` `hyprpaper`                                       | None                              |
| Screenshots          | `grim` `slurp` `wl-clipboard` `ffmpeg`                                               | `libwebp`                         |
| Screencasts          | `wl-recorder`                                                                        | `libvpx`                          |
| Theme switching      | `xdg-desktop-portal-hyprland` `xdg-desktop-portal-gtk` `gsettings` `meson` `gtk+3.0` | None                              |
| Volume control       | `wpctl`                                                                              | None                              |
| Calculator media key |                                                                                      | `gnome-calculator` or `qalculate` |
| Session locking      | `dm-tool`                                                                            |                                   |

If you want proper theme switching for GTK apps, you'll need to create `/usr/share/xdg-desktop-portal/hyprland-portals.conf`:
```ini
[preferred]
default=hyprland;gtk
```

> [!TIP]
> You might want to install `lightdm` and `lightdm-elephant-greeter-git` if you want the same greeter for login and unlocking sessions
