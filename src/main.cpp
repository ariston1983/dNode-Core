#include "dNode-Core.hpp"

int test(){
  return 10;
};

void setup(){
  Serial.begin(115200);
  delay(2000);

  nodeTester::init();
  bool _res = nodeTester::begin<int>("Test")->exec(&test)->target(1)->run();
  if (!_res) Serial.println("Test failed");
};
void loop(){};
