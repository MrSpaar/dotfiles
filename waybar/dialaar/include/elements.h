#pragma once
#include <gtkmm.h>


struct APData {
    int signal;
    bool current;
    std::string name;
    std::string security;

    explicit APData(const std::string &raw);
};


class LabeledIcon: public Gtk::Box {
public:
    LabeledIcon() {
        append(image);
        append(label);
        set_spacing(5);
    }

    void setLabel(const char *text) {
        label.set_label(text);
        label.set_xalign(0.0);
    }

    void setIcon(const char *iconName) {
        image.set_from_icon_name(iconName);
    }
private:
    Gtk::Label label;
    Gtk::Image image;
};


class FilePicker: public Gtk::Overlay {
public:
    FilePicker();
private:
    void onPickerClicked();
private:
    Gtk::Entry entry;
    Gtk::Button button;
};


template<typename T>
class ConfigField: public Gtk::Box {
public:
    ConfigField() {
        label.set_xalign(0);
        label.set_size_request(100, -1);
        widget.set_hexpand(true);

        set_spacing(5);
        set_hexpand(true);

        append(label);
        append(widget);
    }

    void setLabel(const char *text) {
        label.set_label(text);
    }
public:
    T widget;
private:
    Gtk::Label label;
};
