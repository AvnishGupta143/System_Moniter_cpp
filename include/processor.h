#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization(); 
  std::vector<float> Utilization_cores();

 private:
};

#endif