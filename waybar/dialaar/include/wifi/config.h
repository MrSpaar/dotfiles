#pragma once
#include "include/elements.h"


class WifiConfig: public Gtk::Box {
public:
    explicit WifiConfig();
    bool update(const APData &data);
private:
    void onMethodChange();
    void onVisibilityToggle();
private:
    ConfigField<Gtk::Entry> passwordField;
    Gtk::ToggleButton hideButton;
    Gtk::Overlay passwordOverlay;

    ConfigField<Gtk::ComboBoxText> methodField;
    ConfigField<Gtk::Entry> identityField;
    ConfigField<FilePicker> caCertField;
    ConfigField<FilePicker> clientCertField;
    ConfigField<FilePicker> clientKeyField;
    ConfigField<Gtk::Entry> domainField;
    ConfigField<Gtk::ComboBoxText> method2Field;
    ConfigField<Gtk::Entry> identity2Field;

    Gtk::Button saveButton;
    Gtk::Button cancelButton;
    Gtk::Box buttonBox;

    Gtk::Box fieldsBox;
    Gtk::ScrolledWindow scroller;
};
