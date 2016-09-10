#include "YZThread.h"
#include <thread>

namespace YZ {

YZThread::YZThread(void) : need_quit(false)
{
}

YZThread::~YZThread(void)
{
}

void YZThread::start() 
{
	auto t = std::thread(std::bind(&YZThread::socketWorkThread, this));
	//t.join();
    t.detach();
}

void YZThread::stop() 
{
	need_quit = true;
}

void YZThread::socketWorkThread()
{
	onExecute();
}

void YZThread::onExecute()
{

}

}