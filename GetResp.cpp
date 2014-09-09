#include "GetResp.h"

GetResp::GetResp() {
	
}

GetResp::GetResp(bool _exist, const std::string &_value):
exists(_exist),
value(_value) {
	
}

void GetResp::restore(const std::string &_dump) {
	
	PbGetResp pb;
	pb.ParseFromString(_dump);
	exists = pb.exists();
	value = pb.value();
}

void GetResp::dump(std::string &_dump) {
	
	PbGetResp pb;
	pb.set_exists(exists);
	pb.set_value(value);
	_dump = pb.SerializeAsString();
}
