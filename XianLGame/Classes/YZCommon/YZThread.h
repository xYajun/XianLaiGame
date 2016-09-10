#ifndef __NH_YZThread_H__
#define __NH_YZThread_H__

#include <functional>
#include <thread>

namespace YZ {

	typedef std::function<void(void* pragma)> YZThreadFunc;

	class YZThread
	{
	public:
		YZThread(void);
		virtual ~YZThread(void);

	public:
		void start();
		void stop();
		virtual void onExecute();

	protected:	
		bool need_quit;

	private:
		void socketWorkThread();

	private:
		std::thread* _thread;
		YZThreadFunc _func;
	};

};

#endif		//__YZ_YZThread_H__
