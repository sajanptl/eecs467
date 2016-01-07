/* Key Listener
 * 
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


/////////////////////////// Message Handler Object /////////////////////////////
class MsgHandler
{
public:
	MsgHandler();
    void onMessage(const lcm::ReceiveBuffer* rbuf, 
                   const std::string& chan,
                   const key_press_t* msg);
private:
	int64_t prevLocalTime;
	int64_t prevLcmTime;
};

///////////////////////////////// Main Function ////////////////////////////////
int main()
{
	signal(SIGINT, sigHandler); // register ctrl+C to stop program
		
    // init LCM-related things
	LCM lcm;
	MsgHandler mh;
	string chan("A0_KEY_PRESS");
    if (!lcm.good()) // check if LCM initialized correctly
    {
        cout << "LCM Init Error" << endl;
        return -1;
    }
	lcm.subscribe(chan, &MsgHandler::onMessage, &mh);

	cout << "Begin Key Listening" << endl;
	while (!KILL) lcm.handle();
		
	cout << "Ending Key Producer Program" << endl;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////

/* MsgHandler Implementation */
MsgHandler::MsgHandler() : prevLocalTime(0), prevLcmTime(0) {}

void MsgHandler::onMessage(const lcm::ReceiveBuffer* rbuf, 
                   		   const std::string& chan,
                   		   const key_press_t* msg)
{
//// THIS WORKS, BUT IT'S THE WRONG RESULTS TO CALCULATE. WE NEED AVG KEY PRESS
//// PER MINUTE ACCORDING TO A0 HANDOUT.

	// update local delta time
	int64_t time = utime_now();
	int64_t dlocal = time - prevLocalTime;
	prevLocalTime = time;

	// update lcm delta time
	int64_t dlcm = msg->utime - prevLcmTime;
	prevLcmTime = msg->utime;

	// print out results
	cout << "Local Delta (s): " << (double)dlocal / 1000.0; 
	cout << " LCM Delta (s): " << (double)dlcm / 1000.0;
	cout << " Key: " << msg->key << endl;
}


/* Kill Signal Handler */
void sigHandler(int p)
{
	KILL = 1;
}
// End of File
