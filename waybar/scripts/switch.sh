if [ "$(bash ~/.config/waybar/scripts/theme.sh)" == "light" ]; then
    _CURRENT=1; _OTHER=2
else
    _CURRENT=2; _OTHER=1
fi

function CURRENT {
    echo "$1" | awk -v i=$_CURRENT '{print $i}'
}
function OTHER {
    echo "$1" | awk -v i=$_OTHER '{print $i}'
}

gsettings set org.gnome.desktop.interface color-scheme "$(OTHER "prefer-light prefer-dark")"
gsettings set org.gnome.desktop.interface gtk-theme "$(OTHER "$GTK3_THEME")"

hyprctl hyprpaper preload $(OTHER "$WALLPAPER_PATH")  > /dev/null
hyprctl hyprpaper wallpaper ", $(OTHER "$WALLPAPER_PATH")"  > /dev/null

sed -i "s|$(CURRENT "$ROFI_THEME")|$(OTHER "$ROFI_THEME")|" ~/.config/rofi/config.rasi
sed -i "s|$(CURRENT "$QT_THEME")|$(OTHER "$QT_THEME")|" ~/.config/qt6ct/qt6ct.conf

sed -i "s|background = \"#$(CURRENT "$DUNST_BG_COLOR")\"|background = \"#$(OTHER "$DUNST_BG_COLOR")\"|" ~/.config/dunst/dunstrc
sed -i "s|foreground = \"#$(CURRENT "$DUNST_FG_COLOR")\"|foreground = \"#$(OTHER "$DUNST_FG_COLOR")\"|" ~/.config/dunst/dunstrc
killall dunst > /dev/null 2>&1

pkill -SIGRTMIN+8 waybar
exit 0
