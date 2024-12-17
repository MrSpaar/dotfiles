#!/bin/bash
pidof slurp && exit 0

if [ -n "$1" ]; then
    DIR="$1"
else
    DIR="$HOME/Media/Screenshots"
fi

FILENAME="$DIR/screenshot_$(date +'%Y-%m-%d_%H-%M-%S')"
REGION=$(slurp)

if [ $? -ne 0 ] || [ -z "$REGION" ]; then
    echo "Error: slurp failed or did not return a valid region."
    exit 1
fi

grim -g "$REGION" "$FILENAME.png"
ffmpeg  -hide_banner -loglevel error -i "$FILENAME.png" -vcodec libwebp -lossless 1 "$FILENAME.webp"

EXT="png"
[ -f "$FILENAME.webp" ] && EXT="webp"

echo -n "file://$FILENAME.$EXT" | wl-copy -t text/uri-list
ACTION=$(dunstify "Saved to $FILENAME.$EXT" -i "$FILENAME.png" --action="action, Open")

[ "$EXT" = "webp" ] && rm "$FILENAME.png"
[ "$ACTION" == "2" ] && xdg-open "$FILENAME.$EXT" &
