#pragma once
#include "include/elements.h"


class WifiConfig: public Gtk::Box {
public:
    explicit WifiConfig();
    void update(const APData &data);
private:
    void save();
    void onMethodChange();
    void onVisibilityToggle();
private:
    std::string currentSSID;

    Gtk::Overlay passwordOverlay;
    Gtk::ToggleButton hideButton;
    ConfigField<Gtk::Entry> passwordField{ "Password" };

    ConfigField<Gtk::ComboBoxText> methodField{ "Method" };
    ConfigField<Gtk::Entry> identityField{ "Identity" };
    ConfigField<FilePicker> caCertField{ "CA Cert" };
    ConfigField<FilePicker> clientCertField{ "Client Cert" };
    ConfigField<FilePicker> clientKeyField{ "Key Cert" };
    ConfigField<Gtk::Entry> domainField{ "Domain" };
    ConfigField<Gtk::ComboBoxText> method2Field{ "Phase2 Method" };
    ConfigField<Gtk::Entry> identity2Field{ "Phase2 Identity" };

    Gtk::Button saveButton;
    Gtk::Button cancelButton;
    Gtk::Box buttonBox;

    Gtk::Box fieldsBox;
    Gtk::ScrolledWindow scroller;

    static constexpr const char* METHODS[] = {
        "PSK", "PWD", "PEAP", "TLS", "TTLS"
    };

    static constexpr const char* PHASE2_METHODS[] = {
        "CHAP", "MSCHAP", "MSCHAPv2", "PAP",
        "Tunneled-CHAP", "Tunneled-MSCHAP", "Tunneled-MSCHAPv2", "Tunneled-PAP"
    };
};
