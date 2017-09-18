#ifndef _APPLICATION_STATE_H
#define _APPLICATION_STATE_H

class ApplicationState abstract
{
public:
	virtual void updateInput() = 0;
	virtual void updateNetworking() = 0;
	virtual void updateState() = 0;
	virtual void display() = 0;

protected:
	ApplicationState *next, *prev;


};

#endif //_APPLICATION_STATE_H
