#ifndef _LRU_HASH_H_
#define _LRU_HASH_H_

#include "hiconfig.h"
#include "Hash.h"

class LruHash : public Hash {
public:
	
	class Record {
	public:
		std::string value;
		uint64_t kill_ts;
	
		Record(const std::string &_value, uint64_t _kill_ts);
	};
	
	hiaux::hashtable<std::string, Record> m_records;
	size_t m_n_lru_shots;
	uint64_t m_default_ttl;
	
	LruHash();
	virtual void set(const std::string &_k, const std::string &_v);
	//void setWithTtl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc);
	virtual bool get(const std::string &_k, std::string &_v);
	//void getWithTtl(const std::string &_k, uint64_t _ttl_inc, std::string &_v);
	virtual void del(const std::string &_k);
	
	//void setHashNlruShots(uint64_t _n);
	//void setHashDefaultTtl(uint64_t _ttl);
};


#endif
