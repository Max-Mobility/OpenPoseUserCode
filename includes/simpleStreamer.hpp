#ifndef SIMPLESTREAMER_H
#define SIMPLESTREAMER_H

#include "/usr/local/include/sio/sio_client.h"

#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

class SimpleStreamer
{

private:

  std::mutex _lock;
  std::condition_variable_any _cond;
  bool _connect_finish = false;
  bool _connected = false;

  sio::client h;
  std::string url;

  void on_connected()
  {
	_lock.lock();
	_cond.notify_all();
	_connect_finish = true;
	_connected = true;
	_lock.unlock();

  };

  void on_close(sio::client::close_reason const& reason)
  {
	_connected = false;
  };

  void on_fail()
  {
	_lock.lock();
	_cond.notify_all();
	_connect_finish = true;
	_connected = false;
	_lock.unlock();
  };

public:

  SimpleStreamer()
  {
  };

  bool connect(std::string url)
  {
	h.set_open_listener(std::bind(&SimpleStreamer::on_connected,this));
	h.set_close_listener(std::bind(&SimpleStreamer::on_close,this, std::placeholders::_1));
	h.set_fail_listener(std::bind(&SimpleStreamer::on_fail,this));
	h.connect(url);
	_lock.lock();
	if(!_connect_finish)
	  {
		_cond.wait(_lock);
	  }
	_lock.unlock();
	return _connected;
  };

  void close()
  {
	h.sync_close();
	h.clear_con_listeners();
  };

  bool sendBuffer(std::string name, char* buff, int length)
  {
	if(_connected)
	  {
		h.socket()->emit(name, std::make_shared<std::string>(buff, length));
	  }
	return _connected;
  };

  bool sendMessage(std::string name, std::string message)
  {
	if(_connected)
	  {
		h.socket()->emit(name, message);
	  }
	return _connected;
  };

  bool isConnected()
  {
	return _connected;
  };

};

#endif
