#include "simonslays.h"

int main(int argc, char** argv) {

        Glib::RefPtr<Gtk::Application> app =
        Gtk::Application::create(argc, argv, "simonslaysthegame.exe");


        SimonSlays gameWindow;
        return app->run(gameWindow);

}
