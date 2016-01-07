/* Key Producer
 * 
 * This program reads in characters typed in the terminal one at a time and 
 * publishes them in a timestamped lcm message. This is a part of EECS 467 W16
 * Assignment 0.
 * 
 * Q (capital Q/Shift + q/Caps Lock + Q) quits the program.
 *
 * Author: Sajan Patel
 * Uniqname: sajanptl
 * Date: 6 January 2016
 */
// Standard Headers
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <csignal>
#include <string>

// EECS 467 Headers
#include "common/timestamp.h"

// LCM and Message Headers
#include <lcm/lcm-cpp.hpp>
#include "key_press_t.hpp"

using lcm::LCM;
using std::string;
using std::cout;
using std::cin;
using std::endl;

//////////////////////////////// Kill Signal Stuff /////////////////////////////
sig_atomic_t KILL;
void sigHandler(int p);

///////////////////////////////// Main Function ////////////////////////////////
int main()
{
	system("stty raw -echo"); // turn off buffering until enter key
	
	signal(SIGINT, sigHandler); // register ctrl+C to stop program
		
    // init LCM-related things
	LCM lcm;
	key_press_t kp;
	string chan("A0_KEY_PRESS");
    if (!lcm.good()) // check if LCM initialized correctly
    {
        cout << "LCM Init Error" << endl;
		system("stty sane");
        return -1;
    }
	
	cout << "Begin Key Production" << endl;
	while (cin >> kp.key)
	{
		kp.utime = utime_now();
		lcm.publish(chan, &kp);
		
		cout << endl << kp.key << "\t" << kp.utime << endl;

		if (KILL || (kp.key == 'Q')) break;
	}
		
	cout << "Ending Key Producer Program" << endl;
	system("stty sane"); // re-enable normal terminal
	return 0;
}
////////////////////////////////////////////////////////////////////////////////

/* Kill Signal Handler */
void sigHandler(int p)
{
	KILL = 1;
}
// End of File
