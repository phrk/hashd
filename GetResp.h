#ifndef _GET_RESP_H_
#define _GET_RESP_H_

#include "hiconfig.h"
#include "GetResp.pb.h"

class GetResp {
public:
	GetResp();
	GetResp(bool _exist, const std::string &_value);
	
	bool exists;
	std::string value;

	void restore(const std::string &_dump);
	void dump(std::string &_dump);
};

#endif
