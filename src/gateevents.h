#pragma once
#include <string>



class IGateEvent {
public:
	virtual int getType() const= 0;
	virtual ~IGateEvent(){

	}
};



