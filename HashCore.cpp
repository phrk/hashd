#include "HashCore.h"

HashCore::HashCore() {
	
}

void HashCore::onSet(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp) {
	
}

void HashCore::onSetWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp) {
	
}

void HashCore::onGet(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp) {
	
}

void HashCore::onGetWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp) {
	
}

void HashCore::onDel(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp) {
	
}

void HashCore::onSetHashNlruShots(const std::string &_hash, uint64_t _n, std::string &_resp) {
	
}

void HashCore::onSetHashDefaultTtl(const std::string &_hash, uint64_t _ttl, std::string &_resp) {
	
}
