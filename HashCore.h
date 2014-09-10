#ifndef _HASH_CORE_H_
#define _HASH_CORE_H_

#include "hiconfig.h"
#include "hiaux/structs/hashtable.h"
#include "hiaux/threads/locks.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "Hash.h"
#include "LruHash.h"

class HashCore {
	
	hiaux::hashtable<std::string, Hash> m_hashes;
	hAutoLock m_lock;
	
public:
	
	HashCore();
	
	void onSet(const std::string &_hash, const std::string &_k, const std::string &_v);
//	void onSetWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc);
	void onGet(const std::string &_hash, const std::string &_k, bool &_exists, std::string &_v);
//	void onGetWithTtl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_v);
	void onDel(const std::string &_hash, const std::string &_k);
//	void onSetHashNlruShots(const std::string &_hash, uint64_t _n);
//	void onSetHashDefaultTtl(const std::string &_hash, uint64_t _ttl);
};

typedef boost::shared_ptr<HashCore> HashCorePtr;

#endif
