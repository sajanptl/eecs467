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
	int64_t startLocal;
	int64_t startLcm;
	bool first;
	int count;
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
	while (!KILL) lcm.handleTimeout(1000 / 15);
		
	cout << "Ending Key Producer Program" << endl;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////

/* MsgHandler Implementation */
MsgHandler::MsgHandler() : startLocal(0), startLcm(0), first(true), count(0) {}

void MsgHandler::onMessage(const lcm::ReceiveBuffer* rbuf, 
                   		   const std::string& chan,
                   		   const key_press_t* msg)
{
	int64_t time = utime_now();
	++count;
	
	if (first)
	{
		first = false;
		startLocal = time;
		startLcm = msg->utime;
	}
	else
	{
		cout << "Local Avg: " << (double)count / ((double)(time - startLocal) / 1000000.0);
		cout << "\tLCM Avg: " << (double)count / ((double)(msg->utime - startLcm) / 1000000.0);
		cout << "\tKey: " << msg->key << endl;
	}
}

/* Kill Signal Handler */
void sigHandler(int p)
{
	KILL = 1;
}
// End of File
