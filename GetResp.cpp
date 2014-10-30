#include "GetResp.h"

GetResp::GetResp() {
	
}

GetResp::GetResp(int _err, const std::string &_value):
err(_err),
value(_value) {
	
}

void GetResp::restore(const std::string &_dump) {
	
	PbGetResp pb;
	pb.ParseFromString(_dump);
	err = pb.err();
	value = pb.value();
	ttl = pb.ttl();
}

void GetResp::dump(std::string &_dump) {
	
	PbGetResp pb;
	pb.set_err(err);
	pb.set_value(value);
	pb.set_ttl(ttl);
	_dump = pb.SerializeAsString();
}
