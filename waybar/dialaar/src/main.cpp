#include "include/wifi/gui.h"


int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.examples.base");
    return app->make_window_and_run<WifiGUI>(argc, argv);
}
