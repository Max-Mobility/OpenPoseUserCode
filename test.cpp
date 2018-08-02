#include "simpleStreamer.h"

#include <unistd.h>

int main(int argc, const char* args[])
{
  std::string url = "http://127.0.0.1:1337";
  char buff[] = {0x00,0x01, 0x02, 0x03};
  SimpleStreamer streamer;
  streamer.connect(url);
  while(true){
	streamer.sendMessage("log", "Hellooooooo!");
	usleep(500000);
	streamer.sendBuffer("log", buff, sizeof(buff));
	usleep(500000);
  }
  return 0;
};
