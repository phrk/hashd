#include "HashCore.h"

HashCore::HashCore(TaskLauncherPtr _launcher):
m_launcher(_launcher),
m_last_service_ts(0) {
	
}

TaskLauncher::TaskRet HashCore::serviceTask() {
	
	hiaux::hashtable<std::string, HashPtr>::iterator it;
	
	std::string last_check_hash;
	HashPtr hash;
	
	{
		hLockTicketPtr ticket = m_lock.lock();
		
		if (m_hashes.size() == 0)
			return TaskLauncher::NO_RELAUNCH;
		
		it = m_hashes.begin();
		hash = it->second;
		last_check_hash = it->first;
	}
	
	while (true) {
		
		hash->doService();
		
		{
			hLockTicketPtr ticket = m_lock.lock();
			it = m_hashes.find(last_check_hash);
			it++;
			
			if (it == m_hashes.end())
				return TaskLauncher::NO_RELAUNCH;
			
			hash = it->second;
			last_check_hash = it->first;
		}
		
	}
	
	return TaskLauncher::NO_RELAUNCH;
}

void HashCore::checkRunService() {
	
	if ( time(0) - m_last_service_ts > 10 ) {
		
		m_launcher->addTask(NEW_LAUNCHER_TASK2(&HashCore::serviceTask, this));
		m_last_service_ts = time(0);
	}
}

void HashCore::onCreateHash(const std::string &_name, const std::map<std::string, std::string> &_opts, int &_err) {
	
	hLockTicketPtr ticket = m_lock.lock();

	checkRunService();
	
	if (m_hashes.find(_name) != m_hashes.end()) {
		
		_err = E_HC_HASH_ALREADY_EXISTS;
		return;
	}
	
	if (_opts.at("type") == "simple") {
		
		m_hashes.insert(std::pair<std::string, HashPtr>(_name, HashPtr(new Hash(_name))));
		_err = E_HC_OK;
		
	} else if (_opts.at("type") == "ttl") {
				
		if (_opts.find("default_ttl") == _opts.end()) {
			
			_err = E_HC_CREATE_HASH_ERROR;
			return;
		}
		
		if (_opts.find("cleanup_period") == _opts.end()) {
			
			_err = E_HC_CREATE_HASH_ERROR;
			return;
		}
		
		if (_opts.find("cleanup_check_size") == _opts.end()) {
			
			_err = E_HC_CREATE_HASH_ERROR;
			return;
		}
		
		m_hashes.insert(std::pair<std::string, HashPtr>(_name, HashPtr(new TtlHash(_name,
																					string_to_uint64(_opts.at("default_ttl")),
																					string_to_uint64(_opts.at("cleanup_period")),
																					string_to_uint64(_opts.at("cleanup_check_size"))
																					))));
		_err = E_HC_OK;
	}
	else
		_err = E_HC_CREATE_HASH_ERROR;
}

void HashCore::onSet(const std::string &_hash, const std::string &_k, const std::string &_v, int &_err) {
			
	hLockTicketPtr ticket = m_lock.lock();
	
	checkRunService();
	
	hiaux::hashtable<std::string, HashPtr>::iterator it = m_hashes.find(_hash);
	
	if (it == m_hashes.end()) {
		
		_err = E_HC_HASH_DONT_EXISTS;
		return;
	}
	
	it->second->set(_k, _v);
	_err = E_HC_OK;
}

void HashCore::onSetAndIncTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, int &_err) {
	
	hLockTicketPtr ticket = m_lock.lock();
	
	checkRunService();
	
	hiaux::hashtable<std::string, HashPtr>::iterator it = m_hashes.find(_hash);
	
	if (it == m_hashes.end()) {
		
		_err = E_HC_HASH_DONT_EXISTS;
		return;
	}
	
	it->second->set_and_inc_ttl(_k, _v, _ttl_inc);
	_err = E_HC_OK;
}

void HashCore::onGet(const std::string &_hash, const std::string &_k, std::string &_v, int &_err) {
	
	hLockTicketPtr ticket = m_lock.lock();
	
	checkRunService();
	
	hiaux::hashtable<std::string, HashPtr>::iterator it = m_hashes.find(_hash);
	
	if (it == m_hashes.end()) {
		
		_err = E_HC_HASH_DONT_EXISTS;
		return;
	}
	
	if (it->second->get(_k, _v)) {
		
		_err = E_HC_OK;
	}
	else {
		
		_err = E_HC_KEY_DONT_EXISTS;
	}
}

void HashCore::onDel(const std::string &_hash, const std::string &_k, int &_err) {
	
	hLockTicketPtr ticket = m_lock.lock();
	
	checkRunService();
	
	hiaux::hashtable<std::string, HashPtr>::iterator it = m_hashes.find(_hash);
	
	if (it == m_hashes.end()) {
		
		_err = E_HC_HASH_DONT_EXISTS;
		return;
	}
	
	if (it->second->del(_k)) {
		
		_err = E_HC_OK;
	}
	else {
		
		_err = E_HC_KEY_DONT_EXISTS;
	}	
}

void HashCore::onGetTtl(const std::string &_hash, const std::string &_k, uint64_t &_ttl, int &_err) {

	hLockTicketPtr ticket = m_lock.lock();
	
	checkRunService();
	
	hiaux::hashtable<std::string, HashPtr>::iterator it = m_hashes.find(_hash);
	
	if (it == m_hashes.end()) {
		
		_err = E_HC_HASH_DONT_EXISTS;
		return;
	}
	
	_err = it->second->getTtl(_k, _ttl);
}

void HashCore::dump(int _fd) {
	
	hiaux::hashtable<std::string, HashPtr>::iterator it = m_hashes.begin();
	hiaux::hashtable<std::string, HashPtr>::iterator end = m_hashes.end();
	
	while (it != end) {
		
		it->second->dump(_fd);
		it++;
	}
}

void HashCore::restoreTtlHashDescr(const char *_str) {
		
	TtlHashDescr pb;
	pb.ParseFromString(_str);
	
	if (m_hashes.find (pb.name()) != m_hashes.end()) {
		std::cout << "HashCore::restoreTtlHashDescr hash " << pb.name() << " already exists" <<  std::endl;
		return;
	}
	
	m_hashes.insert(std::pair<std::string, HashPtr>(pb.name(), HashPtr(new TtlHash(pb.name(),
																				pb.default_ttl(),
																				pb.cleanup_period(),
																				pb.cleanup_check_size()
																				))));
	
}

void HashCore::restoreTtlHashKv(const char *_str) {
	
	TtlHashKv pb;
	pb.ParseFromString(_str);
	
	hiaux::hashtable<std::string, HashPtr>::iterator it = m_hashes.find(pb.hash());
	if (it == m_hashes.end()) {
		
		std::cout << "HashCore::restoreTtlHashKv hash " << pb.hash() << " dont exists. Key:" << pb.k() << " Value:" << pb.v() << std::endl;
		return;
	}
	
	it->second->restore((void*)&pb);
}

void HashCore::restore(int _fd) {
	
	// dump record: |record_type|record_size|record|
	
	size_t file_size = lseek(_fd, 0, SEEK_END);
	size_t nread = 0;
	lseek(_fd, 0, SEEK_SET);
	
	while (nread < file_size) {
		
		uint32_t record_type;
		uint32_t record_size;
	
		read(_fd, &record_type, sizeof(record_type));
		read(_fd, &record_size, sizeof(record_size));
		
		char record_dump  [record_size+1];
		read(_fd, &record_dump, record_size);
		//record_dump[record_size] = '\0';
		
		if (record_type == HC_DR_TTL_HASH_DESCR) {
			
			restoreTtlHashDescr(record_dump);
		}
		else if (record_type == HC_DR_TTL_HASH_KV) {
			
			restoreTtlHashKv(record_dump);
		}
		
		nread += 2*sizeof(uint32_t) + record_size;
	}
}
