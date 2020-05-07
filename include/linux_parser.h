#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

// namespace LinuxParser
namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};               
const std::string kPasswordPath{"/etc/passwd"};

// System
std::string OperatingSystem();
std::string Kernel();
std::vector<int> Pids();
float MemoryUtilization();
int TotalProcesses();
int RunningProcesses();
long UpTime();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
std::vector<std::vector<std::string>> CpuUtilization_cores();
long Jiffies();
long ActiveJiffies();
long IdleJiffies();
std::vector<long> Jiffies_cores(std::vector<std::vector<std::string>> &cpu_utilization);
std::vector<long> ActiveJiffies_cores(std::vector<std::vector<std::string>> &cpu_utilization);
std::vector<long> IdleJiffies_cores(std::vector<std::vector<std::string>> &cpu_utilization);
long ActiveJiffies(int pid);

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};

#endif