#include "simonslays.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <fstream>

SimonSlays::SimonSlays()
 :

	// CONSTRUCTORS

	// Images
	banner("banner.png"),
	lsidebar("side.png"),
	rsidebar("side.png"),
	top("hbar.png"),
	bottom("hbar.png"),
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
	bigBox(Gtk::ORIENTATION_VERTICAL, 0),
	statBox(Gtk::ORIENTATION_HORIZONTAL, 2),
	body(Gtk::ORIENTATION_HORIZONTAL, 0),
	gameBox(Gtk::ORIENTATION_VERTICAL, 0),
	gameBox1(Gtk::ORIENTATION_HORIZONTAL, 0),
	gameBox2(Gtk::ORIENTATION_HORIZONTAL, 0),
	menuBox(Gtk::ORIENTATION_HORIZONTAL, 10),

	// Labels
	stats("Round 1"),
	score("0"),
	credits("Jennie Ron Ablog & John Christian Sun"),

	// Frames
	statsFrame("Round 0"),
	hsFrame("High Score"),
	creditsFrame("Programmers"),

	// Timeouts
	timer_number(0),
	count_value(1),
	timeout_value(250),
	interval(1500)

{

	// Window Properties
	set_mnemonics_visible(true);
	set_default_icon_from_file("icon.ico");
	set_default_size(600,600);
	set_size_request(600, 600);
	set_border_width(0);
	set_title("Simon Slays");
	set_position(Gtk::WIN_POS_CENTER_ALWAYS);

	// Size Requests
	statsFrame.set_size_request(290, -1);
	hsFrame.set_size_request(290, -1);	
	banner.set_size_request(580, 100);

	// Margins
	statBox.set_margin_left(10);
	statBox.set_margin_top(10);
	statBox.set_margin_right(10);
	statBox.set_margin_bottom(10);
	menuBox.set_margin_left(10);
	menuBox.set_margin_top(10);
	menuBox.set_margin_right(10);
	menuBox.set_margin_bottom(10);

	// Packing
	add(bigBox);

	bigBox.pack_start(banner);
	banner.set_alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

	bigBox.pack_start(statBox);
	statBox.pack_start(statsFrame);
	statBox.pack_start(hsFrame);
	statsFrame.add(stats);
	hsFrame.add(score);
	
	bigBox.pack_start(top);

	bigBox.pack_start(body);
	body.pack_start(lsidebar, Gtk::PACK_SHRINK);
	body.pack_start(gameBox, Gtk::PACK_SHRINK);
	gameBox.pack_start(gameBox1);
	gameBox1.pack_start(gameButton[0]);
	gameBox1.pack_start(gameButton[1]);

	gameBox.pack_start(gameBox2);
	gameBox2.pack_start(gameButton[2]);
	gameBox2.pack_start(gameButton[3]);

	body.pack_start(rsidebar, Gtk::PACK_SHRINK);

	bigBox.pack_start(bottom);

	bigBox.pack_start(menuBox, Gtk::PACK_SHRINK);
	menuBox.pack_end(quitButton, Gtk::PACK_SHRINK);
	menuBox.pack_end(resetButton, Gtk::PACK_SHRINK);
	menuBox.pack_start(creditsFrame);
	creditsFrame.add(credits);

	// Mnemonics
	quitButton.add_label("_Exit", true);
	resetButton.add_label("_New Game", true);

	// Show
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

	// Actual game starts here
	resetGame();

};

SimonSlays::~SimonSlays() {
}

void SimonSlays::resetGame(){

	// High Score
	std::ifstream hsFile;
	hsFile.open("hs.txt");
	if (!hsFile.fail()){
		hsFile >> hs;
		hsFile.close();
	}
	else{
		hs = 0;
	}

	stats.set_text("Round 0");
	score.set_text(std::string(1, char(hs+48)));
        level = 0;
        srand(time(NULL));
	simonsList.clear();
	tempList.clear();

	show_entry_dialog();

	Gtk::MessageDialog startDialog(*this, "Welcome to Simon's Farm, " + playerName + "!");
  	startDialog.set_secondary_text("Let us see if you remember the order at which Simon slayed the animals.");
	startDialog.run();

	// Start game
        on_button_clicked(-1);
}

void SimonSlays::on_button_clicked(int n){

	slay(n);

	if (tempList.empty()){

		for(int i=0; i<4; i++)
			gameButton[i].set_sensitive(false);

		level++;
		stats.set_text("Round " + std::string(1,char(level+48)));
		if ((level-1) > hs)
			score.set_text(std::string(1,char(level+47)));

		simonsList.push_back(rand() % 4);

		tempList.clear();
		tempList2.clear();

		for(std::list<int>::iterator count = simonsList.begin(); count != simonsList.end(); count++){
			tempList.push_back(*count);
			tempList2.push_back(*count);
		}

		sigc::slot<bool> my_slot_2 = sigc::bind(sigc::mem_fun(*this, &SimonSlays::simonMove), tempList2);
		sigc::connection conn_2 = Glib::signal_timeout().connect(my_slot_2, interval);
	}

	else{
		if(n == *tempList.begin())
			tempList.pop_front();
		else{
			if ((level - 1) > hs){
				std::ofstream hsFile("hs.txt");
				hsFile << std::string(1, char(level+47));
				score.set_text(std::string(1, char(level+47)));
				hsFile.close();
			}
			Gtk::MessageDialog endDialog(*this, "Game Over");
			endDialog.set_message("You lost!");
  			endDialog.set_secondary_text("Score: " + std::string(1,char(level+47)) + "\n\nWould you like to play again?");
			endDialog.set_image(loser);
			loser.show();
			endDialog.add_button(Gtk::StockID(Gtk::Stock::CANCEL), Gtk::RESPONSE_CANCEL);

			switch(endDialog.run()) {

			case Gtk::RESPONSE_OK:
				endDialog.hide();
	        		resetGame();
				break;
			case Gtk::RESPONSE_CANCEL:
			default:
				exit(1);
			};

		}

		if (tempList.size() == 0)
			on_button_clicked(-1);
	}

}

bool SimonSlays::simonMove(std::list<int>& someList){

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


bool SimonSlays::on_timeout(int n, int i){
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

void SimonSlays::show_entry_dialog()
{
    	Gtk::MessageDialog namedialog(*this,"New Game");
    	Gtk::Entry entry;
	
	namedialog.set_secondary_text("Enter your name: ");

    	namedialog.add_button(Gtk::StockID(Gtk::Stock::CANCEL), Gtk::RESPONSE_CANCEL);

    	namedialog.get_content_area()->add(entry);
    	namedialog.show_all();

    	std::string username;

	switch(namedialog.run()) {

		case Gtk::RESPONSE_OK:
        		username = entry.get_text();
			if(username.length() == 0){
				username = "stranger";
			}
			playerName = username;
        		statsFrame.set_label(username.c_str());
        		break;
		case Gtk::RESPONSE_CANCEL:
		default:
			exit(1);
	}
}

void SimonSlays::on_button_quit(){
        hide();
}

void SimonSlays::on_button_reset(){
	resetGame();
}
