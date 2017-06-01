#include <gtkmm.h>
#include <iostream>
#include <map>
#include <list>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include "popupwindow.h"
#include <gtkmm/application.h>

class SimonSlays : public Gtk::Window {

private:
	// Display
	Gtk::Image banner, goat, chicken, pig, cow, goatclicked, chickenclicked, pigclicked, cowclicked, loser;
	Gtk::Button gameButton[4];
	Gtk::Button quitButton, resetButton;
	Gtk::Box bigBox, gameBox1, gameBox2, menuBox;
	Gtk::Label stats;
	Gtk::Frame statsFrame;

	// Counters
	int timer_number;

	// Time Markers
	int count_value;
	int timeout_value;
	unsigned int interval;

	// Game Variables
	std::list<int> simonsList, tempList, tempList2;
	int level;

	// Dialogues
	Gtk::MessageDialog turn;

public:
	SimonSlays();
	~SimonSlays();

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


SimonSlays::SimonSlays()
:
	// Images
	banner("banner.png"),
	goat("goat.png"),
	chicken("chicken.png"),
	pig("pig.png"),
	cow("cow.png"),
	goatclicked("goatclicked.png"),
	chickenclicked("chickenclicked.png"),
	pigclicked("pigclicked.png"),
	cowclicked("cowclicked.png"),
	loser("loser.png"),

	// Boxes
	bigBox(Gtk::ORIENTATION_VERTICAL, 10),
	gameBox1(Gtk::ORIENTATION_HORIZONTAL, 0),
	gameBox2(Gtk::ORIENTATION_HORIZONTAL, 0),
	menuBox(Gtk::ORIENTATION_HORIZONTAL, 10),

	// Labels
	stats("Round 1"),

	// Frames
	statsFrame("Player_Name"),

	// Timeouts
	timer_number(0),
	count_value(1),
	timeout_value(250),
	interval(1500),

	// Dialogs
	turn(*this, "Slay, slay!")
{

	// Window Properties
	set_default_size(600,600);
	set_border_width(0);
	set_title("Simon Slays");

	// Dialogs
	turn.set_secondary_text("");

	// Margins
	gameBox1.set_margin_right(100);
	gameBox1.set_margin_left(100);
	gameBox2.set_margin_right(100);
	gameBox2.set_margin_left(100);
	statsFrame.set_margin_right(10);
	statsFrame.set_margin_left(10);
	menuBox.set_margin_right(100);
	menuBox.set_margin_bottom(10);

	// Positions
	add(bigBox);

	bigBox.pack_start(banner, Gtk::PACK_SHRINK);
	banner.set_alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

	bigBox.pack_start(statsFrame, Gtk::PACK_SHRINK);
	statsFrame.add(stats);

	bigBox.pack_start(gameBox1);
	gameBox1.pack_start(gameButton[0]);
	gameBox1.pack_start(gameButton[1]);

	bigBox.pack_start(gameBox2);
	gameBox2.pack_start(gameButton[2]);
	gameBox2.pack_start(gameButton[3]);

	bigBox.pack_start(menuBox, Gtk::PACK_SHRINK);
	menuBox.pack_end(quitButton, Gtk::PACK_SHRINK);
	menuBox.pack_end(resetButton, Gtk::PACK_SHRINK);

	// Mnemonics
	quitButton.add_label("_Quit", true);
	resetButton.add_label("_Reset", true);


	show_all_children();

	// Signals
	quitButton.signal_clicked().connect(sigc::mem_fun(*this, &SimonSlays::on_button_quit));
	resetButton.signal_clicked().connect(sigc::mem_fun(*this, &SimonSlays::on_button_reset));
	for (int i=0; i<4; i++){
		switch(i){
		case 0:
			gameButton[i].set_image(goat);
			break;
		case 1:
			gameButton[i].set_image(chicken);
			break;
		case 2:
			gameButton[i].set_image(pig);
			break;
		case 3:
			gameButton[i].set_image(cow);
			break;
		};

		gameButton[i].signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &SimonSlays::on_button_clicked), i));
	}


	// Game
	resetGame();

};

SimonSlays::~SimonSlays() {
}


void SimonSlays::resetGame(){
        level = 0;
        srand(time(NULL));
	simonsList.clear();
	tempList.clear();
	Gtk::MessageDialog startdialog(*this, "Simon Slays!");
  	startdialog.set_secondary_text("Let us see if you remember the order at which Simon slayed the animals.");
	startdialog.run();
        on_button_clicked(-1);
}

void SimonSlays::on_button_quit(){
        hide();
}

void SimonSlays::on_button_reset(){
	resetGame();
}

void SimonSlays::on_button_clicked(int n){

	slay(n);

	// Kapag wala pang laman yung tempList na naghohold ng current value ng simonsList...
	if (tempList.empty()){

		// Gagawin niya munang insensitive mga buttons para kapag napindot habang nag-aanimation, walang effect...
		for(int i=0; i<4; i++)
			gameButton[i].set_sensitive(false);

		// Nag-progress yung level kasi empty yung tempList, na ibig sabihin na-pop lahat ng elements so tama lahat ng moves...
		level++;
		stats.set_text("Round " + std::string(1,char(level+48)));

		// So pagkaprogress mag-aappend tayo sa original list natin ng bagong element for the next round...
		simonsList.push_back(rand() % 4);

		// Inupdate na natin yung tempList para makuha niya yung latest value ng simonsList...
		tempList.clear();
		tempList2.clear();	// Eto for simonMove();

		for(std::list<int>::iterator count = simonsList.begin(); count != simonsList.end(); count++){
			tempList.push_back(*count);
			tempList2.push_back(*count);
		}

		// Ipakita yung sequence sa loob ng simonsList...
		sigc::slot<bool> my_slot_2 = sigc::bind(sigc::mem_fun(*this, &SimonSlays::simonMove), tempList2);
		sigc::connection conn_2 = Glib::signal_timeout().connect(my_slot_2, interval);
	}

	else{
		if(n == *tempList.begin())
			tempList.pop_front();
		else{
			std::cout << "Game over!" << std::endl;

			// game over dialogue
			//Gtk::Dialog dialog(const Glib::ustring& "You just lost :(", bool modal=false)
			//Gtk::Dialog dialog("You just lost :(", true);
			//dialog.set_image(loser);
  			//dialog.set_secondary_text("Score: " + std::string(1,char(level+48)));
			
			Gtk::MessageDialog dialog(*this, "Game Over");
			dialog.set_message("You lost!");
  			dialog.set_secondary_text("Score: " + std::string(1,char(level+47)));

		  	if (dialog.run()) 
				resetGame();
		}

		if (tempList.size() == 0)
			on_button_clicked(-1);
	}

}

void SimonSlays::slay(int n){

	switch(n){

	case 0:
		gameButton[n].set_image(goatclicked);
		break;
	case 1:
		gameButton[n].set_image(chickenclicked);
		break;
	case 2:
		gameButton[n].set_image(pigclicked);
		break;
	case 3:
		gameButton[n].set_image(cowclicked);
		break;
	default:
		return;
	};

	sigc::slot<bool> my_slot = sigc::bind<int, int>(sigc::mem_fun(*this, &SimonSlays::on_timeout), timer_number, n);
	sigc::connection conn = Glib::signal_timeout().connect(my_slot, timeout_value);

	timers[timer_number] = conn;
	counters[timer_number] = count_value + 1;
	timer_number++;
}

bool SimonSlays::simonMove(std::list<int>& someList){

	std::cout << "simon is moving. " << std::endl;
	if (someList.size() == 0){
                for(int i=0; i<4; i++)
                        gameButton[i].set_sensitive(true);
		return false;
	}

	int first = *someList.begin();
	someList.pop_front();

	switch(first){
	case 0:
		slay(first);
		break;
	case 1:
		slay(first);
		break;
	case 2:
		slay(first);
		break;
	case 3:
		slay(first);
		break;
	};

	return true;

}


bool SimonSlays::on_timeout(int n, int i)
{
	if (--counters[n] == 0){
		counters.erase(n);
		timers.erase(n);
                switch(i){
                case 0:
                        gameButton[i].set_image(goat);
                        break;
                case 1:
                        gameButton[i].set_image(chicken);
                        break;
                case 2:
                        gameButton[i].set_image(pig);
                        break;
                case 3:
                        gameButton[i].set_image(cow);
                        break;
		};
		return false;
	}
	return true;
}



int main(int argc, char** argv) {

        Glib::RefPtr<Gtk::Application> app =
        Gtk::Application::create(argc, argv, "app.exe");


        SimonSlays gameWindow;
        return app->run(gameWindow);

}
