#!/bin/bash
if [ $2 == "toggle" ]; then
    wpctl set-mute @DEFAULT_$1@ toggle
else
    wpctl set-volume @DEFAULT_$1@ 5%$2
fi

VOLUME=$(wpctl get-volume @DEFAULT_$1@ | awk '{print $2 * 100}')
MUTED=$(wpctl get-volume @DEFAULT_$1@ | grep -o '\[MUTED\]')

URGENCY=normal
MODE=$(bash ~/.config/hypr/scripts/theme.sh)

if [ $1 == "SOURCE" ]; then
    TYPE="mic"; LABEL="Input"; ID=69
else
    TYPE="vol"; LABEL="Output"; ID=70
fi

if [ "$MUTED" == "[MUTED]" ]; then
    URGENCY=low
    ICON=~/.config/hypr/icons/$TYPE-mute-$MODE.png
elif [ $VOLUME -lt 33 ]; then
    ICON=~/.config/hypr/icons/$TYPE-min-$MODE.png
elif [ $VOLUME -lt 66 ]; then
    ICON=~/.config/hypr/icons/$TYPE-mid-$MODE.png
else
    ICON=~/.config/hypr/icons/$TYPE-max-$MODE.png
fi

echo $ICON
dunstify -h int:value:$VOLUME -r $ID -u $URGENCY -i $ICON "$LABEL volume [$VOLUME%]"
