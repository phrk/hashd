#include "TtlHash.h"

TtlHash::Record::Record(const std::string &_value, uint64_t _kill_ts):
	 value(_value),
	 kill_ts(_kill_ts) {
	
}

TtlHash::TtlHash(const std::string &_name, uint64_t _defailt_ttl, uint64_t _cleanup_period, uint64_t _cleanup_check_size):
	Hash(_name),
	m_default_ttl(_defailt_ttl),
	m_cleanup_period(_cleanup_period),
	m_cleanup_check_size(_cleanup_check_size),
	m_last_cleanup_ts(0) {
}

void TtlHash::set(const std::string &_k, const std::string &_v) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		if (m_default_ttl != 0) {
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + m_default_ttl)));
			//std::cout << "TtlHash::set " << _k << " ttl: " <<  m_default_ttl << std::endl;
		}
		else {
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, 0)));
			//std::cout << "TtlHash::set " << _k << " ttl: " << 0 << std::endl;
		}
	} else
		it->second.value = _v;
}

void TtlHash::set_and_inc_ttl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		if (m_default_ttl != 0) {
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + m_default_ttl + _ttl_inc )));
			//std::cout << "TtlHash::set_and_inc_ttl " << _k << " ttl: " <<  m_default_ttl + _ttl_inc << std::endl;
		}
		else {
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + _ttl_inc )));
			//std::cout << "TtlHash::set_and_inc_ttl " << _k << " ttl: " << 0 << std::endl;
		}

	} else {
		//std::cout << "TtlHash::set_and_inc_ttl " << _k << " inc_ttl: " << _ttl_inc << std::endl;
		it->second.value = _v;
		it->second.kill_ts += _ttl_inc;
	}
}

int TtlHash::getWithTtl(const std::string &_k, std::string &_v, uint64_t &_ttl) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it != m_records.end()) {
		
		uint64_t now = time(0);
		
		if (it->second.kill_ts < now) {
			
			m_records.erase(it);
			return E_HC_KEY_DONT_EXISTS;
		}
		
		_v = it->second.value;
		_ttl = it->second.kill_ts - now;
		return E_HC_OK;
	}
	else {
		return E_HC_KEY_DONT_EXISTS;
	}
	
	return E_HC_OK;
}

/*
void HashCore::Hash::setWithTtl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + _ttl_inc)));
	} else {
		it->second.value = _v;
		it->second.kill_ts += _ttl_inc;
	}
}*/

bool TtlHash::get(const std::string &_k, std::string &_v) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		return false;
	} else {
		_v = it->second.value;
	}
	return true;
}
/*
void HashCore::Hash::getWithTtl(const std::string &_k, uint64_t _ttl_inc, std::string &_v) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
	} else {
		_v = it->second.value;
		it->second.kill_ts += _ttl_inc;
	}
}
*/
bool TtlHash::del(const std::string &_k) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it != m_records.end()) {
		m_records.erase(it);
		return true;
	}
	else {
		return false;
	}
}

int TtlHash::getTtl(const std::string &_k, uint64_t &_ttl) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it != m_records.end()) {
		
		uint64_t now = time(0);
		
		if (it->second.kill_ts < now) {
			
			m_records.erase(it);
			return E_HC_KEY_DONT_EXISTS;
		}
		
		_ttl = it->second.kill_ts - now;
		return E_HC_OK;
	}
	else {
		return E_HC_KEY_DONT_EXISTS;
	}
}

void TtlHash::cleanUp() {
	
	hLockTicketPtr ticket = m_lock.lock();
	
	//std::cout << "TtlHash::cleanUp\n";
	
	if (m_records.size() == 0)
		return;
	
	uint64_t keys_viewed = 0;
	uint64_t now = time(0);
	
	hiaux::hashtable<std::string, Record>::iterator it;
	
	if (m_last_cleanup_ts == 0) {
		
		it = m_records.begin();
		
	} else {
		
		it = m_records.find(m_last_cleanup_key_viewed);
		if (it == m_records.end())
			it = m_records.begin();
	}
	
	while (keys_viewed < m_cleanup_period && it != m_records.end()) {
		
		if (it->second.kill_ts < now && it->second.kill_ts != 0) {
			std::cout << "erasing " << it->first << std::endl; 
			m_records.erase(it);
			
			if (m_records.size() == 0)
				return;
		}
		
		if (it == m_records.end()) {
			it = m_records.begin();
			m_last_cleanup_key_viewed = it->first;
			return;
		}
		
		keys_viewed++;
		it++;
	}
	
	if (it == m_records.end()) {
		it = m_records.begin();
		m_last_cleanup_key_viewed = it->first;
	}
}

void TtlHash::doService() {
	
	if (m_last_cleanup_ts == 0 || (time(0) - m_last_cleanup_ts >= m_cleanup_period)) {
			
		cleanUp();
		m_last_cleanup_ts = time(0);
	}
}

void TtlHash::dump(int _fd) {
	
	TtlHashDescr hash_pb;

	hash_pb.set_name(m_name);
	hash_pb.set_default_ttl(m_default_ttl);
	hash_pb.set_cleanup_period(m_cleanup_period);
	hash_pb.set_cleanup_check_size(m_cleanup_check_size);

	std::string dump = hash_pb.SerializeAsString();
	
	uint32_t record_type = HC_DR_TTL_HASH_DESCR;
	uint32_t record_size = dump.size();
	
	iovec atom[3];
	
	atom[0].iov_base = (void*) &record_type;
	atom[0].iov_len = sizeof(record_type);
	
	atom[1].iov_base = (void*) &record_size;
	atom[1].iov_len = sizeof(record_size);
	
	atom[2].iov_base = (void*) dump.data();
	atom[2].iov_len = record_size;
	
	writev(_fd, atom, 3);
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.begin();
	hiaux::hashtable<std::string, Record>::iterator end = m_records.end();
	
	while (it != end) {
		
		TtlHashKv kv_pb;
		kv_pb.set_hash(m_name.c_str());
		kv_pb.set_k(it->first.c_str());
		kv_pb.set_v(it->second.value.c_str());
		kv_pb.set_kill_ts(it->second.kill_ts);
		
		std::string dump = kv_pb.SerializeAsString();
		
		uint32_t record_type = HC_DR_TTL_HASH_KV;
		uint32_t record_size = dump.size();
		
		iovec atom[3];
	
		atom[0].iov_base = (void*) &record_type;
		atom[0].iov_len = sizeof(record_type);
	
		atom[1].iov_base = (void*) &record_size;
		atom[1].iov_len = sizeof(record_size);
	
		atom[2].iov_base = (void*) dump.data();
		atom[2].iov_len = record_size;
	
		writev(_fd, atom, 3);
		
		it++;
	}
}

void TtlHash::restore(void *_pb) {
	
	TtlHashKv *pb = (TtlHashKv*)_pb;
	m_records.insert(std::pair<std::string, Record>(pb->k(), Record(pb->v(), pb->kill_ts())));
}

