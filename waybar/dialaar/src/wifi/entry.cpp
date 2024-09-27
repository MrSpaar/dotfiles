#include <iostream>
#include <sstream>

#include "glibmm/spawn.h"
#include "include/defines.h"
#include "include/wifi/entry.h"


APData::APData(const std::string &raw) {
    std::istringstream iss(raw);
    char activeFlag;

    iss >> activeFlag;
    current = (activeFlag == 'y');

    std::string ssid;
    std::getline(iss >> std::ws, ssid, '"');
    std::getline(iss, ssid, '"');
    name = ssid;

    iss >> security >> signal;
}


WifiEntry::WifiEntry(const APData &apData): data(apData) {
    label.setLabel(data.name.c_str());
    label.setIcon(
        data.current ? CONNECTED_ICON : icons[data.signal]
    );

    connectButton.set_child(label);
    connectButton.set_hexpand(true);
    connectButton.signal_clicked().connect([&]() {
        if (data.current) {
            Glib::spawn_command_line_sync("iwctl station wlan0 disconnect");
        } else {
            Glib::spawn_command_line_sync("iwctl station wlan0 connect " + data.name);
        }

        data.current = !data.current;
    });

    configButton.set_icon_name(CONFIG_ICON);

    set_spacing(MARGIN);
    append(connectButton);
    append(configButton);
}

bool WifiEntry::update(const APData &updated) {
    if (updated.name != data.name)
        return false;

    data = updated;
    label.setIcon(
        data.current ? CONNECTED_ICON : icons[data.signal]
    );

    return true;
}

void WifiEntry::bindConfig(WifiConfig &widget) {
    configButton.signal_clicked().connect([&]() {
        if (widget.update(data)) {
            get_parent()->get_parent()->get_parent()->hide();
            widget.show();
        }
    });
}

std::string WifiEntry::getSSID() {
    return data.name;
}
