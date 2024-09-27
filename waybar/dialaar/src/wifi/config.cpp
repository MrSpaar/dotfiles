#include "include/defines.h"
#include "include/wifi/config.h"


WifiConfig::WifiConfig() {
    methodField.setLabel("Method");
    methodField.widget.append(PSK_METHOD);
    methodField.widget.append(TLS_METHOD);
    methodField.widget.append(PEAP_METHOD);
    methodField.widget.append(TTLS_METHOD);
    methodField.widget.set_active_text(PSK_METHOD);
    methodField.widget.signal_changed().connect(
        sigc::mem_fun(*this, &WifiConfig::onMethodChange)
    );

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

    identityField.setLabel("Identity");
    caCertField.setLabel("CA Cert");
    clientCertField.setLabel("Client Cert");
    clientKeyField.setLabel("Key Cert");
    domainField.setLabel("Domain mask");
    method2Field.setLabel("Phase2 Method");
    identity2Field.setLabel("Phase2 Identity");

    saveButton.set_label("Save");
    cancelButton.set_label("Cancel");
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
    buttonBox.append(saveButton);
    buttonBox.append(cancelButton);
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

bool WifiConfig::update(const APData &data) {
    std::string out, err;
    Glib::spawn_command_line_sync(
        "pkexec --disable-internal-agent cat /var/lib/iwd/" + Glib::filename_from_utf8(data.name) + ".8021x",
        &out, &err, NULL
    );

    if (!err.empty()) {
        return false;
    }


    return true;
}

void WifiConfig::onMethodChange() {
    Glib::ustring method = methodField.widget.get_active_text();

    if (method == TTLS_METHOD || method == PEAP_METHOD) {
        identityField.show();
        caCertField.show();
        domainField.show();
        method2Field.show();
        identity2Field.show();
        clientCertField.hide();
        clientKeyField.hide();
    } else if (method == TLS_METHOD) {
        identityField.show();
        caCertField.show();
        clientCertField.show();
        clientKeyField.show();
        domainField.hide();
        method2Field.hide();
        identity2Field.hide();
    } else {
        identityField.hide();
        caCertField.hide();
        domainField.hide();
        method2Field.hide();
        identity2Field.hide();
        clientCertField.hide();
        clientKeyField.hide();
    }
}

void WifiConfig::onVisibilityToggle() {
    bool active = hideButton.get_active();
    passwordField.widget.set_visibility(!active);
    hideButton.set_icon_name(active? PASSWORD_VISIBLE_ICON: PASSWORD_HIDDEN_ICON);
}
