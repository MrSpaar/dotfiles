#include <fstream>
#include <sstream>

#include "glibmm/convert.h"
#include "glibmm/spawn.h"
#include "include/defines.h"
#include "include/wifi/config.h"
#include "include/string_utils.h"


WifiConfig::WifiConfig() {
    methodField.setLabel("Method");
    for (const char *method: METHODS) {
        methodField.widget.append(method);
    }
    methodField.widget.set_active_text("PSK");
    methodField.widget.signal_changed().connect(
        sigc::mem_fun(*this, &WifiConfig::onMethodChange)
    );

    for (const char *method2: PHASE2_METHODS) {
        method2Field.widget.append(method2);
    }

    passwordField.setLabel("Password");
    passwordField.widget.set_visibility(false);
    passwordField.widget.set_placeholder_text("Enter a password");

    hideButton.set_active(true);
    hideButton.add_css_class("nobg");
    hideButton.set_focus_on_click(false);
    hideButton.set_halign(Gtk::Align::END);
    hideButton.set_icon_name(PASSWORD_VISIBLE_ICON);

    hideButton.signal_clicked().connect(
        sigc::mem_fun(*this, &WifiConfig::onVisibilityToggle)
    );

    passwordOverlay.set_child(passwordField);
    passwordOverlay.add_overlay(hideButton);

    saveButton.set_label("Save");
    saveButton.add_css_class("confirm");
    saveButton.signal_clicked().connect([&]() {
        save();
        hide();
        get_parent()->get_first_child()->show();
    });

    cancelButton.set_label("Cancel");
    cancelButton.add_css_class("cancel");
    cancelButton.signal_clicked().connect([&]() {
        hide();
        get_parent()->get_first_child()->show();
    });

    fieldsBox.set_spacing(MARGIN);
    fieldsBox.set_vexpand(true);
    fieldsBox.set_margin_bottom(MARGIN);
    fieldsBox.set_orientation(Gtk::Orientation::VERTICAL);

    buttonBox.set_spacing(MARGIN);
    buttonBox.set_homogeneous();
    buttonBox.append(cancelButton);
    buttonBox.append(saveButton);
    buttonBox.set_valign(Gtk::Align::END);
    buttonBox.set_orientation(Gtk::Orientation::HORIZONTAL);

    set_margin(MARGIN);
    set_spacing(MARGIN);
    set_orientation(Gtk::Orientation::VERTICAL);

    fieldsBox.append(methodField);
    fieldsBox.append(passwordOverlay);
    fieldsBox.append(identityField);
    fieldsBox.append(caCertField);
    fieldsBox.append(clientCertField);
    fieldsBox.append(clientKeyField);
    fieldsBox.append(domainField);
    fieldsBox.append(method2Field);
    fieldsBox.append(identity2Field);
    onMethodChange();

    scroller.set_child(fieldsBox);
    append(scroller);
    append(buttonBox);
}

inline bool ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

void WifiConfig::update(const APData &data) {
    currentSSID = data.name;

    for (auto& child : fieldsBox.get_children()) {
        if (auto* entryField = dynamic_cast<ConfigField<Gtk::Entry>*>(child)) {
            entryField->widget.set_text("");
        } else if (auto* comboBoxField = dynamic_cast<ConfigField<Gtk::ComboBoxText>*>(child)) {
            comboBoxField->widget.set_active_text("");
        } else if (auto* filePickerField = dynamic_cast<ConfigField<FilePicker>*>(child)) {
            filePickerField->widget.set_text("");
        }
    }

    std::string out, filename = Glib::filename_from_utf8(data.name);
    Glib::spawn_command_line_sync(
        "pkexec --disable-internal-agent bash -c "
        "'cat /var/lib/iwd/" + filename + ".8021x /var/lib/iwd/" + filename + ".psk 2>/dev/null'",
        &out
    );

    if (out.empty()) {
        methodField.widget.set_active_text("PSK");
        onMethodChange();
        return;
    }

    std::string line;
    std::istringstream stream(out);

    while(std::getline(stream, line)) {
        auto [key, value] = parseConfigLine(line);
        if (key.empty()) { continue; }

        if (key == "EAP-Method") {
            methodField.widget.set_active_text(value);
        } else if (key == "EAP-Identity") {
            identityField.widget.set_text(value);
        } else if (key == "Passphrase" || key == "EAP-Password") {
            passwordField.widget.set_text(value);
            methodField.widget.set_active_text("PSK");
        } else if (ends_with(key, "CACert")) {
            caCertField.widget.set_text(value);
        } else if (ends_with(key, "ClientCert")) {
            clientCertField.widget.set_text(value);
        } else if (ends_with(key, "ClientKey")) {
            clientKeyField.widget.set_text(value);
        } else if (ends_with(key, "ServerDomainMask")) {
            domainField.widget.set_text(value);
        } else if (ends_with(key, "Phase2-Method")) {
            method2Field.widget.set_active_text(value);
        } else if (ends_with(key, "Phase2-Identity")) {
            identity2Field.widget.set_text(value);
        } else if (ends_with(key, "Phase2-Password")) {
            passwordField.widget.set_text(value);
        }
    }
}

inline void moveFile(const std::string &filename) {
    Glib::spawn_command_line_sync(
        "pkexec --disable-internal-agent mv '/tmp/" + filename + "' '/var/lib/iwd/" + filename + '\''
    );
}

void WifiConfig::save() {
    std::string filename = Glib::filename_from_utf8(currentSSID) + (
        (methodField.widget.get_active_text() == "PSK") ? ".psk": ".8021x"
    );

    std::ofstream outFile("/tmp/" + filename);
    outFile << "[Security]\n";

    if (methodField.widget.get_active_text() == "PSK") {
        outFile << "Passphrase=" << passwordField.widget.get_text() << '\n';

        outFile.close();
        return moveFile(filename);
    }

    outFile << "EAP-Method=" << methodField.widget.get_active_text() << '\n';
    outFile << "EAP-Identity=" << identityField.widget.get_text() << '\n';

    if (methodField.widget.get_active_text() == "PWD") {
        outFile << "EAP-Password=" << passwordField.widget.get_text() << '\n';

        outFile.close();
        return moveFile(filename);
    }

    std::string methodPrefix = "EAP-" + methodField.widget.get_active_text() + "-";

    outFile << methodPrefix << "ServerDomainMask=" << domainField.widget.get_text() << '\n';
    outFile << methodPrefix << "CACert=" << caCertField.widget.get_text() << '\n';

    if (methodField.widget.get_active_text() == "TLS") {
        outFile << methodPrefix << "ClientCert=" << clientCertField.widget.get_text() << '\n';
        outFile << methodPrefix << "ClientKey=" << clientKeyField.widget.get_text() << '\n';

        outFile.close();
        return moveFile(filename);
    }

    outFile  << methodPrefix << "Phase2-Method=" << method2Field.widget.get_active_text() << '\n';
    outFile  << methodPrefix << "Phase2-Identity=" << identity2Field.widget.get_text() << '\n';
    outFile  << methodPrefix << "Phase2-Password=" << passwordField.widget.get_text() << '\n';

    outFile.close();
    moveFile(filename);
}

void WifiConfig::onMethodChange() {
    Glib::ustring method = methodField.widget.get_active_text();

    identityField.set_visible(method != "PSK");
    clientKeyField.set_visible(method == "TLS");
    clientCertField.set_visible(method == "TLS");
    domainField.set_visible(method == "TTLS" || method == "PEAP");
    method2Field.set_visible(method == "TTLS" || method == "PEAP");
    identity2Field.set_visible(method == "TTLS" || method == "PEAP");
    caCertField.set_visible(method == "TTLS" || method == "PEAP" || method == "TLS");
}

void WifiConfig::onVisibilityToggle() {
    bool active = hideButton.get_active();
    passwordField.widget.set_visibility(!active);
    hideButton.set_icon_name(active? PASSWORD_VISIBLE_ICON: PASSWORD_HIDDEN_ICON);
}
