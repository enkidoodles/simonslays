#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
#include <ctime>
#include <unistd.h>


using namespace std;

int main(int argc, char *argv[]) {
    if (system("cls")) system("clear");
	srand(time(NULL));
	int level = 1;
	int score = 0;
	list<int> simonslist;
	int a, id;
	int answer;
	bool gamecontinue = true;
	bool leveladd = true;
	int timer;

    cout<<"                      BANK ACCOUNT SURVEILANCE SYSTEM!"<<endl;
    cout<<"                                 LOADING...";
    sleep(5);
	while (gamecontinue) {
		cout<<endl<<"                                  LEVEL: " << level<<endl;
		a = rand() % 4;
		simonslist.push_back(a+1); // a + 1 so that random 0 to 3 will be 1 to 4
        id = 10000 + rand() % 89999;

		cout<<"             Person: 2014-"<<id<<" is inputting his/her bank pin...";
	    sleep(3); //sleeps 1 second
    	if (system("cls")) system("clear");

		for (list<int>::iterator count = simonslist.begin(); count != simonslist.end(); count++) { // *count is the item iterator
    		if (*count == 1) cout<<endl<<endl<<"                            [X] [ ] [ ] [ ]";
    		else if (*count == 2) cout<<endl<<endl<<"                            [ ] [X] [ ] [ ]";
    		else if (*count == 3) cout<<endl<<endl<<"                            [ ] [ ] [X] [ ]";
    		else if (*count == 4) cout<<endl<<endl<<"                            [ ] [ ] [ ] [X]";
    		//cout<< *count;
    		sleep(1);
    		if (system("cls")) system("clear");
    		cout<<endl<<endl<<"                            [ ] [ ] [ ] [ ]";
    		sleep(1);
    		if (system("cls")) system("clear");
    		};

    	//if (level < 5) {timer = 3;}
    	//else if (level < 10) {timer = 7;}
    	//else if (level < 25) {timer = 10;}
    	//else if (level < 50) {timer = 15;}

    	//sleep(timer);
    	//if (system("cls")) system("clear");

    	cout<<endl<<"                            Enter the bank pin!!!"<<endl<<"                            BANK PIN: ";
    	for (list<int>::iterator count = simonslist.begin(); count != simonslist.end(); count++) {
    		cin>>answer;
    		if (answer == *count) {
    			//cout<<"you win"<<endl; 
    			leveladd = true;
    			}
    		else {
    			gamecontinue = false;
    			leveladd = false;
    			break;

    			};
    		};
    	if (system("cls")) system("clear");
		
		if (leveladd) {
			level++;
			};

	};

	score = level - 1;
	cout<<"You lost! Dollars: "<<score<<endl;
	cout<<"Thank you for hacking!!!";





	};


