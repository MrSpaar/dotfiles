#pragma once

#include "include/defines.h"
#include "include/elements.h"
#include "include/wifi/config.h"


class WifiEntry: public Gtk::Box {
public:
    explicit WifiEntry(const APData &data);

    std::string getSSID();
    bool update(const APData &raw);
    void bindConfig(WifiConfig &widget);
private:
    APData data;

    LabeledIcon label;
    Gtk::Button configButton;
    Gtk::Button connectButton;

    static constexpr std::array<const char*, 5> icons{
        SIGNAL_NONE_ICON, SIGNAL_WEAK_ICON, SIGNAL_OK_ICON,
        SIGNAL_GOOD_ICON, SIGNAL_EXCELLENT_ICON
    };
};
