#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include "process.h"
#include <unistd.h>

using std::string;
using std::to_string;
using std::vector;

//Return this process's ID
int Process::Pid() { return pid; }

//Return this process's CPU utilization
float Process::CpuUtilization() { 
	long jiffies_start, jiffies_end;
	long uptime_start, uptime_end, delta;

	jiffies_start = LinuxParser::ActiveJiffies(pid);
  	uptime_start = LinuxParser::UpTime(pid);
  	
  	// Wait 10ms
	usleep(10000); // in microseconds

	jiffies_end = LinuxParser::ActiveJiffies(pid);
	uptime_end = LinuxParser::UpTime(pid);

	delta = uptime_end - uptime_start;
	if (delta == 0){
 		return 0;
	}
	return (float)(jiffies_end - jiffies_start) / (float)(uptime_end - uptime_start);
}

//Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

//Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

//Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

//Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

//Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
	long ram = stol(LinuxParser::Ram(pid));
	long ram_a = stol(LinuxParser::Ram(a.pid));

	if(ram_a < ram) return true; 
	else return false;
}