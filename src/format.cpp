#include <string>
#include <iostream> 
#include "format.h"

using std::string;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
	float h = seconds/3600.0;
	int HH = h;
	int MM = ((h - HH)*60);
	int SS = seconds - HH*3600 - MM*60;
	string uptime;
	if (HH>10){
		if(SS<10) uptime = std::to_string(HH) + ":" + std::to_string(MM) + ":0" + std::to_string(SS);
		else uptime = std::to_string(HH) + ":" + std::to_string(MM) + ":" + std::to_string(SS);
	}
	else{
		if(SS<10) uptime = "0" + std::to_string(HH) + ":" + std::to_string(MM) + ":0" + std::to_string(SS);
		else uptime = "0" + std::to_string(HH) + ":" + std::to_string(MM) + ":" + std::to_string(SS);
	}
	return uptime;
}