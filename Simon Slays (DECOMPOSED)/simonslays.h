#ifndef __SIMONSLAYS__
#define __SIMONSLAYS__

#include <gtkmm.h>
#include <map>
#include <list>
#include <cstring>

class SimonSlays : public Gtk::Window {

private:

	// Display
	Gtk::Image banner, lsidebar, rsidebar, top, bottom, goat, chicken, pig, cow, goatclicked, chickenclicked, pigclicked, cowclicked, loser;
	Gtk::Button gameButton[4];
	Gtk::Button quitButton, resetButton;
	Gtk::Box bigBox, statBox, body, gameBox, gameBox1, gameBox2, menuBox;
	Gtk::Label stats, score, credits;
	Gtk::Frame statsFrame, hsFrame, creditsFrame;

	// Counters
	int timer_number;

	// Time Markers
	int count_value;
	int timeout_value;
	unsigned int interval;

	// Game Variables
	std::list<int> simonsList, tempList, tempList2;
	int level;
	int hs;
	std::string playerName;

public:
	SimonSlays();
	~SimonSlays();

	// Entry
	void show_entry_dialog();

	// Signal Handlers
	void on_button_quit();
	void on_button_reset();
	void on_button_clicked(int);

	// Graphic functions
	void slay(int);

	// Callback functions
	bool on_timeout(int, int);

	// Maps
	std::map<int, sigc::connection> timers;
	std::map<int, int> counters;

	// Game
	bool simonMove(std::list<int>&);
	void resetGame();

};


#endif
