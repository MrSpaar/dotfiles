#!/bin/bash
THEME=$(gsettings get org.gnome.desktop.interface color-scheme | sed "s/'prefer-\(.*\)'/\1/")

if [ "$1" == "json" ]; then
    echo "{\"text\": \"\", \"alt\": \"$THEME\", \"tooltip\": \"\", \"class\": \"$THEME\", \"percentage\": 0}"
elif [ "$1" == "wallpaper" ]; then
    index=$([ $THEME == "light" ] && echo 1 || echo 2)
    echo $(echo $WALLPAPER_PATH | awk -v i=$index '{print $i}')
else
    echo "$THEME"
fi
