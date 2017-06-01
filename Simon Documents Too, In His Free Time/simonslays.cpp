#include "simonslays.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <fstream>

/**
 * @brief Constructor of class SimonSlays.
 * @details 1 Calls the constructors of Gtk::Widgets on which it passes parameters necessary for: loading of images, initialization of Gtk::Labels, button labels, orientation of boxes, and variable values needed by the timeout functions used.	

2 Sets the window's properties: default size, border width, initial position, icon and title.	

3 Sets the margins of Gtk::Box statBox and Gtk::Box menuBox.

4 Pack starts other boxes to Gtk::Box bigBox and their children to themselves.

5 Connects Gtk::Button quitButton and Gtk::Button resetButton to signal handlers void SimonSlays::on_button_quit(); and void SimonSlays::on_button_reset();

6 Calls void SimonSlays::resetGame(); to start a new game.
 */

SimonSlays::SimonSlays() {
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

/**
 * @brief Destructor of class SimonSlays.
 * @details 1 This is the destructor of class SimonSlays.
 */

SimonSlays::~SimonSlays() {
}

/**
 * @brief Sets game conditions necessary to start a new game.
 * @details 1 Initializes game variable int level to default value 0.

2 Reads the initial value for int hs when score.txt file exists.

3 Calls void SimonSlays::show_entry_dialog() to fetch player name and void SimonSlays::startDialog() to signify start of new game.

4 Calls void SimonSlays::on_button_clicked() with -1 as parameter to start game.
 */

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

/**
 * @brief Signal handler for Gtk::Button gameButton[] and primary handler for the actual game.
 * @details 1 Passes int n to void SimonSlays::slay() which animates the button's image to signify click.
	
2 [SIMON'S TURN] IF list<int> tempList is empty: Buttons are made insensitive to prevent user interaction during the flashing of latest sequence.

Flashing of sequence is made possible by scheduling a timeout function bool SimonSlays::simonMove().
	  
[PLAYER'S TURN] ELSE:
		
Tests if user's move is correct (or if parameter int n is equal to tempList.begin())
		
Correct move leads to list<int> simonsList.pop_front();
		
Wrong move leads to updating high score file (score.txt) if necessary, and running the local obj Gtk::MessageDialog endDialog();
 * 
 * @param n The integer pertaining to the index of the button clicked in Gtk::Button gameButton[] IF user clicked a button.
		
-1 IF AND ONLY IF function is called within code i.e. done to start the game.
 */

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

/**
 * @brief Mimics Simon's (CPU) every move for each round. Scheduled to be called every unsigned int interval milliseconds.
 * @details 1 IF list<int>& someList is empty: Buttons are made sensitive to make way for user interaction.
		
Returns false to unschedule self.
	  
ELSE: Passes first element of list<int>& someList to void SimonSlays::slay() which animates the button's image to signify Simon's move.
		
Removes first element of list<int>& someList.
 * 
 * @param someList A mutable list that contains the current sequence of Simon's moves.
 * @return Returns false if list<int>& someList is empty to unschedule self, and true if otherwise.
 */

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

/**
 * @brief Animates the button images and schedules bool SimonSlays::on_time_out(int i, int j)
 * @details 1 Sets the image gameButton[n] to its corresponding clicked version then calls bool SimonSlays::on_time_out() with int timer_number and int n as parameter.
 * 
 * @param n Shall contain index of the button clicked in Gtk::Button gameButton[] IF user clicked a button.
 */

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

/**
 * @brief Scheduled for one-time call with a duration of int timeout_value, 250 milliseconds. Changes button images from clicked version to default version.
 * @details 1 On first call, sets button images to default and returns false thereby unscheduling self.

2 If not first call, return true.
 * 
 * @param n Index of the timer in std::map<int, int> counters.
 * @param i Index of the button clicked in Gtk::Button gameButton[].
 * 
 * @return Returns false on first call to unschedule self, and true otherwise.
 */

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

/**
 * @brief Shows the dialog for name entry on new game.
 * @details 1 IF button OK is pressed, username will be set to player's input on entry; if player's input is empty, username will be 'stranger'
	
2 IF button CANCEL is pressed, the program will be terminated.
 */

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

/**
 * @brief Exits the game.
 * @details This function exits the game window!
 */

void SimonSlays::on_button_quit(){
        hide();
}

/**
 * @brief Resets the game.
 * @details This function calls resetGame() to reset the game!
 */

void SimonSlays::on_button_reset(){
	resetGame();
}
