#ifndef _HASH_CORE_H_
#define _HASH_CORE_H_

#include "hiconfig.h"
#include "hiaux/structs/hashtable.h"
#include "hiaux/threads/locks.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

class HashCore {
	
	class Hash {
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
		
		Hash();
		void set(const std::string &_k, const std::string &_v);
		void setWithTtl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc);
		bool get(const std::string &_k, std::string &_v);
		void getWithTtl(const std::string &_k, uint64_t _ttl_inc, std::string &_v);
		void del(const std::string &_k);
		
		void setHashNlruShots(uint64_t _n);
		void setHashDefaultTtl(uint64_t _ttl);
	};
	
	
	hiaux::hashtable<std::string, Hash> m_hashes;
	hAutoLock m_lock;
	
public:
	
	HashCore();
	
	void onSet(const std::string &_hash, const std::string &_k, const std::string &_v);
	void onSetWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc);
	void onGet(const std::string &_hash, const std::string &_k, bool &_exists, std::string &_v);
	void onGetWithTtl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_v);
	void onDel(const std::string &_hash, const std::string &_k);
	void onSetHashNlruShots(const std::string &_hash, uint64_t _n);
	void onSetHashDefaultTtl(const std::string &_hash, uint64_t _ttl);
};

typedef boost::shared_ptr<HashCore> HashCorePtr;

#endif
