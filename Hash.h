#ifndef _HASH_H_
#define _HASH_H_

#include "hiconfig.h"
#include "hiaux/strings/string_utils.h"
#include <string>

class Hash {
public:
	
	hiaux::hashtable<std::string, std::string> m_records;
	
	Hash();
	virtual void set(const std::string &_k, const std::string &_v);
	//void setWithTtl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc);
	virtual bool get(const std::string &_k, std::string &_v);
	//void getWithTtl(const std::string &_k, uint64_t _ttl_inc, std::string &_v);
	virtual void del(const std::string &_k);
	
	//void setHashNlruShots(uint64_t _n);
	//void setHashDefaultTtl(uint64_t _ttl);
};

#endif
