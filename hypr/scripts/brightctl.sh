#!/bin/bash
brightnessctl s $1 > /dev/null


BRIGHTNESS=$(brightnessctl -m | awk -F ',' '{print $4}')
BRIGHTNESS="${BRIGHTNESS%\%}"
MODE=$(bash ~/.config/waybar/scripts/theme.sh)

if [ $BRIGHTNESS -lt 33 ]; then
    ICON=~/.config/hypr/icons/bri-min-$MODE.png
elif [ $BRIGHTNESS -lt 66 ]; then
    ICON=~/.config/hypr/icons/bri-mid-$MODE.png
else
    ICON=~/.config/hypr/icons/bri-full-$MODE.png
fi

dunstify -h int:value:$BRIGHTNESS  -i $ICON "Brightness [$BRIGHTNESS%]" -r 68
