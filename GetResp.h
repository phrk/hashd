#ifndef _GET_RESP_H_
#define _GET_RESP_H_

#include "hiconfig.h"
#include "GetResp.pb.h"

class GetResp {
public:
	GetResp();
	GetResp(int _err, const std::string &_value);
	
	int err;
	std::string value;
	uint64_t ttl;

	void restore(const std::string &_dump);
	void dump(std::string &_dump);
};

#endif
