#pragma once

#include <thread>
#include <atomic>
#include <gtkmm.h>
#include <netlink/netlink.h>

#include "config.h"


class WifiGUI: public Gtk::Window {
public:
    WifiGUI();
    void refresh();
private:
    void eventLoop();
    bool onKeyPressed(guint keyval, guint, Gdk::ModifierType state);
    void onVisibilityButtonPressed();
private:
    Gtk::Overlay overlay;

    WifiConfig config;
    Gtk::Box wifiList;
    Gtk::ScrolledWindow scroller;

    Glib::RefPtr<Gtk::CssProvider> cssProvider;
    Glib::RefPtr<Gtk::EventControllerKey> controller;

    int pipe[2];
    std::thread eventThread;
    nl_sock *sock = nullptr;
    std::atomic_bool running{true};
};
