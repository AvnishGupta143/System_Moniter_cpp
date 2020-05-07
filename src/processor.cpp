#include "processor.h"
#include <unistd.h> // for usleep
#include <iostream>
#include "linux_parser.h"
#include <vector>
#include <iostream>
#include <string>

using std::vector;
using std::string;
//Return the aggregate CPU utilization
float Processor::Utilization() { 
	long prev_active = LinuxParser::ActiveJiffies();
	long prev_total = LinuxParser::Jiffies();
	usleep(100000); // in microseconds	
	long active = LinuxParser::ActiveJiffies();
	long total = LinuxParser::Jiffies();

	long change_active = active - prev_active;
	long change_total = total - prev_total;

	float utilization = (float)change_active/(float)change_total;
	return utilization; 
}

vector<float> Processor::Utilization_cores() {
	vector<vector<string>> cpu_utilization_prev = LinuxParser::CpuUtilization_cores();
	vector<long> prev_active = LinuxParser::ActiveJiffies_cores(cpu_utilization_prev);
	vector<long> prev_total = LinuxParser::Jiffies_cores(cpu_utilization_prev);
	usleep(50000); // in microseconds	
	vector<vector<string>> cpu_utilization = LinuxParser::CpuUtilization_cores();
	vector<long> active = LinuxParser::ActiveJiffies_cores(cpu_utilization);
	vector<long> total = LinuxParser::Jiffies_cores(cpu_utilization);

	vector<float> utilization;
	for(int i{1}; i<prev_active.size(); i++){
		long change_active = active[i] - prev_active[i];
		long change_total = total[i] - prev_total[i];
		float u = (float)change_active/(float)change_total;
		if (u > 100.0) u = 100.0;
		utilization.push_back(u);
	}
	return utilization;
}