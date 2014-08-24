#ifndef _HASH_CORE_H_
#define _HASH_CORE_H_

#include "hiconfig.h"
#include "hiaux/structs/hashtable.h"

#include <boost/shared_ptr.hpp>

class HashCore {
public:
	
	void onSet(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp);
	void onSetWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp);
	void onGet(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp);
	void onGetWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp);
	void onDel(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp);
	void onSetHashNlruShots(const std::string &_hash, uint64_t _n, std::string &_resp);
	void onSetHashDefaultTtl(const std::string &_hash, uint64_t _ttl, std::string &_resp);
};

typedef boost::shared_ptr<HashCore> HashCorePtr;

#endif
