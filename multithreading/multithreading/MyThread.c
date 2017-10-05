#include "MyThread.h"

#include <Windows.h>

unsigned long __stdcall threadLaunchInternal(void* params)
{
	MyThread* theThread = (MyThread*)params;

	theThread->flag = 0;
	theThread->result = theThread->func(theThread->params);
	theThread->flag = 1;

	return theThread->result;
}

int threadLaunch(MyThread* thread_out, MyThreadFunc func, void* params)
{
	if (thread_out && !thread_out->handle && func)
	{
		thread_out->func = func;
		thread_out->params = params;
		thread_out->flag = -1;
		thread_out->result = -1;

		thread_out->handle = CreateThread(0, 0,
			threadLaunchInternal, thread_out, 0,
			&thread_out->id);

		return thread_out->id;
	}

	return -1;
}