#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//Read and return the OperatingSystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

//Read and return the Kernel
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string mem_free, mem_total, temp;
  string line1, line2;
  float mem_used = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()){
    std::getline(filestream, line1);
    std::getline(filestream, line2);
    std::istringstream linestream1(line1);
    std::istringstream linestream2(line2);
    linestream1 >> temp >> mem_total;
    linestream2 >> temp >> mem_free;
    mem_used = (stof(mem_total) - stof(mem_free))/stof(mem_total);
  }
  return mem_used;
}

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string temp, total_proc, line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> temp >> total_proc;
      if (temp == "processes"){
        return stoi(total_proc);
      }
    }
  }
  return 0; 
}

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string temp, running_proc, line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> temp >> running_proc;
      if (temp == "procs_running"){
        return stoi(running_proc);
      }
    }
  }
  return 0; 
}

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string suspend_time, idle_time;
  string line;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  std::getline(filestream,line);
  std::istringstream stream(line);
  stream >> suspend_time >> idle_time;
  return stol(suspend_time); 
}

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  vector<string> v;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      string temp;
      std::istringstream linestream(line);
      linestream >> temp;
      if (temp == "cpu"){
        while(linestream >> temp){
          v.push_back(temp);
        }
        return v;
      }
    }
  }
  return {}; 
}

vector<vector<string>> LinuxParser::CpuUtilization_cores() { 
  string line;
  vector<vector<string>> v_cores;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      string temp;
      std::istringstream linestream(line);
      linestream >> temp;
      if (temp.find("cpu") != string::npos){
        vector<string> v;
        while(linestream >> temp){
          v.push_back(temp);
        }
        v_cores.push_back(v);
      }
    }
    return v_cores;
  }
  return {}; 
}

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total {0};
  vector<string> cpu_utilization;
  cpu_utilization = LinuxParser::CpuUtilization();
  for (int state = kUser_; state  <= kSteal_; state++){
    total += stol(cpu_utilization[state]);
  }
  return total;
}

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> v_str = LinuxParser::CpuUtilization();
  vector<long> v;
  for(auto st:v_str){
    v.push_back(stol(st));
  }
  long jiffies = v[kUser_] + v[kNice_] + v[kSystem_] + v[kIRQ_] + v[kSoftIRQ_] + v[kSteal_];
  return jiffies;
 }

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> v_str = LinuxParser::CpuUtilization();
  vector<long> v;
  for(auto st:v_str){
    v.push_back(stol(st));
  }
  long jiffies = v[kIdle_] + v[kIOwait_];
  return jiffies;
}

////Read and return the total number of jiffies in cores
vector<long> LinuxParser::Jiffies_cores(vector<vector<string>> &cpu_utilization) { 
  vector<long> total;
  for (auto cpu_I: cpu_utilization)
  { 
    int t{0};
    for (int state = kUser_; state  <= kSteal_; state++){
      t += stol(cpu_I[state]);
    }
    total.push_back(t);
  }
  return total;
}

//Read and return the number of active jiffies for the system  in cores
vector<long> LinuxParser::ActiveJiffies_cores(vector<vector<string>> &cpu_utilization) { 
  vector<long> v, jiffies;
  for (auto cpu_I: cpu_utilization)
  {
    for(auto st:cpu_I){
      v.push_back(stol(st));
    }
    long j = v[kUser_] + v[kNice_] + v[kSystem_] + v[kIRQ_] + v[kSoftIRQ_] + v[kSteal_];
    jiffies.push_back(j);
  }
  return jiffies;
 }

//Read and return the number of idle jiffies for the system  in cores
vector<long> LinuxParser::IdleJiffies_cores(vector<vector<string>> &cpu_utilization) {
  vector<long> v, jiffies;
  for (auto cpu_I: cpu_utilization)
  {
    for(auto st:cpu_I){
      v.push_back(stol(st));
    }
    long j = v[kIdle_] + v[kIOwait_];
    jiffies.push_back(j);
  }
  return jiffies;
 }

//Read and return the number of active jiffies for a PID 14,15,16,17,22
long LinuxParser::ActiveJiffies(int pid) { 
  string line, temp;
  string pidFilename = to_string(pid);
  vector<string> v;
  std::ifstream filestream(kProcDirectory + pidFilename + kStatFilename);
  if(filestream.is_open()){
    getline(filestream,line);
    std::istringstream linestream(line);
    while(linestream>>temp){
      v.push_back(temp);
    }
    long sum = stoi(v[13]) + stoi(v[14]) + stoi(v[15]) + stoi(v[16]);
    return sum; 
  }
  return 0;
}

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream,cmd);
  }
  return cmd;
}

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) {   
  string line, temp, ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> temp >> ram;
      if(temp == "VmSize:"){
        int RAM = stoi(ram)/1024;
        return to_string(RAM);
      }
    }
  }
  return "0"; 
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, temp, UID;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> temp;
      if(temp == "Uid:"){
        linestream >> UID;
        return UID;
      }
    }
  }
  return string(); 
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, user, t1, t2, t3;
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> t1 >> t2 >> t3;
      if(t2 == LinuxParser::Uid(pid)){
        return user;
      }
    }
  }
  return string(); 
}

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, temp;
  long uptime = 0;
  vector<string> v;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    while(linestream >> temp){
      v.push_back(temp);
    }
    uptime = stol(v[21])/sysconf(_SC_CLK_TCK);
  }
  return uptime;
}