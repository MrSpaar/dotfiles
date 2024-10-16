#pragma once

#include <gtkmm.h>
#include <type_traits>


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
    std::string get_text() { return entry.get_text(); }
    void set_editable(bool b) { entry.set_editable(b); }
    void set_can_focus(bool b) { entry.set_can_focus(b); }
    void set_text(const std::string &text) { entry.set_text(text); }
private:
    void onPickerClicked();
private:
    Gtk::Entry entry;
    Gtk::Button button;
};


template<typename T>
class ConfigField: public Gtk::Box {
public:
    ConfigField(const char *name) {
        label.set_xalign(0);
        label.set_label(name);
        label.set_size_request(100, -1);
        widget.set_hexpand(true);

        if constexpr (!std::is_same_v<T, Gtk::ComboBoxText>) {
            widget.set_editable(true);
            widget.set_can_focus(true);
        }

        set_spacing(5);
        set_hexpand(true);

        append(label);
        append(widget);
    }

    void setLabel(const char *text) {
        label.set_label(text);
    }

    void setVisible(bool visibility) {
        if (visibility) show();
        else hide();
    }
public:
    T widget;
private:
    Gtk::Label label;
};
