#include "include/defines.h"
#include "include/elements.h"


FilePicker::FilePicker() {
    entry.set_hexpand(true);

    button.add_css_class("nobg");
    button.set_focus_on_click(false);
    button.set_halign(Gtk::Align::END);
    button.set_icon_name(FILE_PICKER_ICON);
    button.signal_clicked().connect(
        sigc::mem_fun(*this, &FilePicker::onPickerClicked)
    );

    set_child(entry);
    add_overlay(button);
}

void FilePicker::onPickerClicked() {
    auto dialog = Gtk::make_managed<Gtk::FileChooserDialog>(
        "Please choose a file", Gtk::FileChooser::Action::OPEN
    );

    auto filter_pem = Gtk::FileFilter::create();
    filter_pem->set_name("PEM files");
    filter_pem->add_pattern("*.pem");
    dialog->set_filter(filter_pem);

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::ACCEPT);

    dialog->signal_response().connect([dialog, this](int response_id) {
        if (response_id == Gtk::ResponseType::ACCEPT) {
            auto file = dialog->get_file();

            if (file) {
                this->entry.set_text(file->get_path());
            }
        }

        dialog->hide();
    });

    dialog->show();
}
