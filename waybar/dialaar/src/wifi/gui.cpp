#include <thread>
#include <unistd.h>
#include <unordered_set>
#include <netlink/msg.h>
#include <netlink/socket.h>

#include "include/defines.h"
#include "include/wifi/gui.h"
#include "include/wifi/entry.h"


WifiGUI::WifiGUI() {
    wifiList.set_margin(MARGIN);
    wifiList.set_spacing(MARGIN);
    wifiList.set_orientation(Gtk::Orientation::VERTICAL);

    scroller.set_vexpand(true);
    scroller.set_child(wifiList);

    refresh();
    eventThread = std::thread(&WifiGUI::eventLoop, this);

    controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
        sigc::mem_fun(*this, &WifiGUI::onKeyPressed), false
    );

    cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_data(R"(
        entry {
            outline-color: @blue_1;
        }

        entry selection {
            color: black;
            background-color: RGBA(153, 193, 241, 0.5);
        }

        .confirm {
            color: @success_color;
        }

        .cancel {
            color: @destructive_color;
        }

        .nobg {
            font-size: 18px;
            background: transparent;
        }
    )");

    Gtk::StyleProvider::add_provider_for_display(
        get_display(),
        cssProvider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    overlay.add_overlay(scroller);
    config.hide();
    overlay.add_overlay(config);

    set_child(overlay);
    set_title("Test");
    set_default_size(APP_SIZE);
    add_controller(controller);
}


bool WifiGUI::onKeyPressed(guint keyval, guint, Gdk::ModifierType state) {
    if (keyval != GDK_KEY_Escape) return true;

    close();
    running.store(false);

    if (sock != nullptr) {
        nl_socket_free(sock);
    }

    if (eventThread.joinable()) {
        ::write(pipe[1], "stop", 4);
        eventThread.join();
    }

    return false;
}

int handleEvent(struct nl_msg *msg, void *arg) {
    Glib::signal_idle().connect_once(
        [arg]() { ((WifiGUI *) arg)->refresh(); }
    );

    return NL_OK;
}

void WifiGUI::eventLoop() {
    Glib::spawn_command_line_async("iwctl station wlan0 scan");

    sock = nl_socket_alloc();
    ::pipe(pipe);

    nl_socket_disable_seq_check(sock);
    nl_socket_modify_cb(sock, NL_CB_VALID, NL_CB_CUSTOM, handleEvent, this);

    nl_join_groups(sock, RTMGRP_LINK | RTMGRP_IPV4_IFADDR);
    if (nl_connect(sock, NETLINK_ROUTE)) {
        throw std::runtime_error("Can't connect network socket");
    }

    nl_socket_add_membership(sock, RTNLGRP_LINK);
    nl_socket_add_membership(sock, RTNLGRP_IPV4_IFADDR);
    nl_socket_add_membership(sock, RTNLGRP_IPV4_ROUTE);

    struct pollfd fds[2];
    fds[0].fd = nl_socket_get_fd(sock);
    fds[0].events = POLLIN;
    fds[1].fd = pipe[0];
    fds[1].events = POLLIN;

    while (running.load()) {
        int ret = poll(fds, 2, -1);
        if (ret < 0) {
            throw std::runtime_error("Error while polling socket");
        }

        if (fds[0].revents & POLLIN) {
            ret = nl_recvmsgs_default(sock);

            if (ret < 0) {
                throw std::runtime_error("Error receiving Netlink message" + std::string(nl_geterror(ret)));
            }
        }
    }
}

void WifiGUI::refresh() {
    std::string out;

    Glib::spawn_command_line_sync(R"(sh -c "
        iwctl station wlan0 get-networks |
            tail -n +5 | head -n -1 |
            sed -e 's:\[0m::g' -e 's:\*\x1b.*:\*:g' -e 's:\x1b::g' -e 's:\[1;90m::g' |
            awk '{
                shift = 0;
                if (\$1 == \">\") { shift = 1; printf \"y \" } else { printf \"n \" };
                signal = gsub(/\\*/, \"*\", \$(NF));
                name = \$(1+shift);
                for (i = 2+shift; i < NF-1; i++ ) name = name \" \" \$(i);
                print \"\\\"\" name \"\\\"\", \$(NF-1), signal
            }'
    ")", &out);

    std::string line;
    std::istringstream stream(out);

    std::unordered_set<std::string> names;
    std::vector<Gtk::Widget*> toRemove;

    while (std::getline(stream, line)) {
        APData data(line);
        names.insert(data.name);

        bool updated = false;
        for (auto *entry : wifiList.get_children()) {
            auto wifiEntry = static_cast<WifiEntry*>(entry);
            if (wifiEntry->update(data)) {
                updated = true;
                break;
            }
        }

        if (!updated) {
            auto entry = Gtk::make_managed<WifiEntry>(data);
            entry->bindConfig(config);
            wifiList.append(*entry);
        }
    }

    auto children = wifiList.get_children();
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        auto wifiEntry = static_cast<WifiEntry*>(*it);

        if (names.find(wifiEntry->getSSID()) == names.end()) {
            wifiList.remove(*wifiEntry);
        }
    }
}
