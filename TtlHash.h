#ifndef _TTL_HASH_H_
#define _TTL_HASH_H_

#include "hiconfig.h"
#include "Hash.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "HashCoreErrors.h"
#include "TtlHashDescr.pb.h"
#include "TtlHashKv.pb.h"

#include "hiaux/threads/locks.h"
#include "hiaux/fs/file_utils.h"
#include <sys/uio.h>

#define HC_DR_TTL_HASH_DESCR 0
#define HC_DR_TTL_HASH_KV 1

class TtlHash : public Hash {
	
	class Record {
	public:
		std::string value;
		uint64_t kill_ts;
	
		Record(const std::string &_value, uint64_t _kill_ts);
	};
	
	hiaux::hashtable<std::string, Record> m_records;
	
	hAutoLock m_lock;
	
	uint64_t m_default_ttl;
	
	uint64_t m_cleanup_period;
	uint64_t m_cleanup_check_size;

	uint64_t m_last_cleanup_ts;
	std::string m_last_cleanup_key_viewed;

	void cleanUp();

public:
		
	TtlHash(const std::string &_name, uint64_t _defailt_ttl, uint64_t _cleanup_period, uint64_t _cleanup_check_size);
	
	virtual void set(const std::string &_k, const std::string &_v);
	virtual void set_and_inc_ttl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc);
	virtual bool get(const std::string &_k, std::string &_v);
	virtual bool del(const std::string &_k);
	virtual int getTtl(const std::string &_k, uint64_t &_ttl);
	
	virtual void doService();
	
	virtual void dump(int _fd);
	virtual void restore(void *_pb);
};

typedef boost::shared_ptr<TtlHash> TtlHashPtr;

#endif
