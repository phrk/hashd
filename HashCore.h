#ifndef _HASH_CORE_H_
#define _HASH_CORE_H_

#include "hiconfig.h"
#include "hiaux/structs/hashtable.h"
#include "hiaux/threads/locks.h"
#include "hiaux/threads/tasklauncher.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "HashCoreErrors.h"
#include "Hash.h"
#include "TtlHash.h"

class HashCore {
	
	TaskLauncherPtr m_launcher;
	
	hiaux::hashtable<std::string, HashPtr> m_hashes;
	hAutoLock m_lock;
	
	uint64_t m_last_service_ts;
	
	TaskLauncher::TaskRet serviceTask();
	void checkRunService();
	
public:
	
	HashCore(TaskLauncherPtr _launcher);
	
	void onCreateHash(const std::string &_name, const std::map<std::string, std::string> &_opts, int &_err);	
	void onSet(const std::string &_hash, const std::string &_k, const std::string &_v, int &_err);
	void onSetAndIncTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, int &_err);
	void onGet(const std::string &_hash, const std::string &_k, std::string &_v, int &_err);
	void onDel(const std::string &_hash, const std::string &_k, int &_err);
	void onGetTtl(const std::string &_hash, const std::string &_k, uint64_t &_ttl, int &_err);
	
	void restoreTtlHashDescr(const char *_str);
	void restoreTtlHashKv(const char *_str);
	
	void dump(int _fd);
	void restore(int _fd);
};

typedef boost::shared_ptr<HashCore> HashCorePtr;

#endif
