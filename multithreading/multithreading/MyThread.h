#ifndef _MYTHREAD_H
#define _MYTHREAD_H

#ifdef __cplusplus
extern "C"
{
#else
typedef struct MyThread MyThread;
#endif

typedef int(*MyThreadFunc)(void *);

struct MyThread
{
	void* handle;
	unsigned long id;

	int flag;
	long result;
	MyThreadFunc func;
	void* params;
};


int threadLaunch(MyThread* thread_out, MyThreadFunc func, void* params);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _MYTHREAD_H