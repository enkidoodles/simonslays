 #include <gtkmm.h>
#include <iostream>
#include <map>
#include <list>
#include <cstdlib>
#include <ctime>
#include <cstring>

class SimonSlays : public Gtk::Window {

private:
	// Display
	Gtk::Image banner, goat, chicken, pig, cow, goatclicked, chickenclicked, pigclicked, cowclicked;
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

public:
	SimonSlays();
	~SimonSlays();

	// Signal Handlers
	void on_button_quit();
	void on_button_clicked(int);

	// Graphic functions
	void slay(int);

	// Callback functions
	bool on_timeout(int, int);

	// Maps
	std::map<int, sigc::connection> timers;
	std::map<int, int> counters;

};


SimonSlays::SimonSlays()
:
	// Images
	banner("banner.png"),
	goat("goat.png"),
	chicken("chicken.png"),
	pig("pig.png"),
	cow("cow.png"),
	goatclicked("goatsad.png"),
	chickenclicked("chickensad.png"),
	pigclicked("pigsad.png"),
	cowclicked("cowclicked.png"),

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
	timeout_value(500)
{
	// Window Properties
	set_default_size(600,600);
	set_border_width(0);
	set_title("Simon Slays");

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


	// ACTUAL GAME STARTS HERE

	srand(time(NULL));
	gameOver = false;
	level = 0;
	score = 0;
	while (!gameOver){

		// CPU MOVES
		level++;
		for(int i=0; i<4; i++){
			gameButton[i].set_sensitive(false);
		}

		random_index = rand() % 4;
		simonsList.push_back(random_index);

		for(std::list<int>::iterator count = simonsList.begin(); count != simonsList.end(); count++){
			slay(*count);
		}

		// PLAYER MOVES
		for(int i=0; i<4; i++){
			gameButton[i].set_sensitive(true);
		}

		//dialog this

	}
};

SimonSlays::~SimonSlays() {
}

void SimonSlays::on_button_quit(){
        hide();
}

void SimonSlays::on_button_clicked(int n){
	slay(n);

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
	};

	sigc::slot<bool> my_slot = sigc::bind<int, int>(sigc::mem_fun(*this, &SimonSlays::on_timeout), timer_number, n);
	sigc::connection conn = Glib::signal_timeout().connect(my_slot, timeout_value);

	timers[timer_number] = conn;
	counters[timer_number] = count_value + 1;
	timer_number++;
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
