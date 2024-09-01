#/bin/bash
echo $(gsettings get org.gnome.desktop.interface color-scheme | sed "s/'prefer-\(.*\)'/\1/")
