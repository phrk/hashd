#ifndef _HASH_H_
#define _HASH_H_

#include "hiconfig.h"
#include "hiaux/strings/string_utils.h"
#include <string>
#include <boost/shared_ptr.hpp>

class Hash {
public:
	
	hiaux::hashtable<std::string, std::string> m_records;
	
	Hash();
	virtual void set(const std::string &_k, const std::string &_v);
	virtual void set_and_inc_ttl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc);
	virtual bool get(const std::string &_k, std::string &_v);
	virtual bool del(const std::string &_k);
	virtual int getTtl(const std::string &_k, uint64_t &_ttl);
	virtual void doService();
};

typedef boost::shared_ptr<Hash> HashPtr;

#endif
