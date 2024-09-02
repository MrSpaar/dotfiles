#!/bin/bash
hyprpaper &

for path in $WALLPAPER_PATH; do
    hyprctl hyprpaper preload $path
done

if [ $(bash ~/.config/waybar/scripts/theme.sh) == 'light' ]; then
    hyprctl hyprpaper wallpaper ", $($WALLPAPER_PATH | awk '{print $1}')"
else
    hyprctl hyprpaper wallpaper ", $(echo $WALLPAPER_PATH | awk '{print $2}')"
fi
