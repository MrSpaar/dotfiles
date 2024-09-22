#!/bin/bash
if [ -n "$1" ]; then
    DIR="$1"
else
    DIR="$HOME/Media/Screencasts"
fi

FILENAME="$DIR/screencast_$(date +'%Y-%m-%d_%H-%M-%S')"
REGION=$(slurp)

if [ $? -ne 0 ] || [ -z "$REGION" ]; then
    echo "Error: slurp failed or did not return a valid region."
    exit 1
fi

if [[ $(ffmpeg -v quiet -codecs | grep ".*DE.*libvpx.*") ]]; then
    EXT="webm"
    wf-recorder -g "$REGION" -f $FILENAME.$EXT -c libvpx &
else
    EXT="mp4"
    wf-recorder -g "$REGION" -f $FILENAME.$EXT &
fi


PID=$!
MODE=$(bash ~/.config/waybar/scripts/theme.sh)

dunstify -b -t 0 "Recording..." -A "action, Stop" -i ~/.config/hypr/icons/reg-$MODE.png
kill $PID
echo -n "file://$FILENAME.$EXT" | wl-copy -t text/uri-list

ffmpeg -y -i "$FILENAME.$EXT" -vf "select=eq(n\,0)" -q:v 3 /tmp/cap_screenshot.png
ACTION=$(dunstify "Saved to $FILENAME.$EXT" -i /tmp/cap_screenshot.png --action="action, Open")

if [ "$ACTION" == "2" ]; then
    xdg-open $FILENAME.$EXT &
fi
