#include "TtlHash.h"

TtlHash::Record::Record(const std::string &_value, uint64_t _kill_ts):
	 value(_value),
	 kill_ts(_kill_ts) {
	
}

TtlHash::TtlHash(uint64_t _defailt_ttl, uint64_t _cleanup_period, uint64_t _cleanup_check_size):
	m_default_ttl(_defailt_ttl),
	m_cleanup_period(_cleanup_period),
	m_cleanup_check_size(_cleanup_check_size),
	m_last_cleanup_ts(0) {
}

void TtlHash::set(const std::string &_k, const std::string &_v) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		if (m_default_ttl != 0)
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + m_default_ttl)));
		else
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, 0)));
	} else
		it->second.value = _v;
}

void TtlHash::set_and_inc_ttl(const std::string &_k, const std::string &_v, uint64_t &_ttl_inc) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		if (m_default_ttl != 0)
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + m_default_ttl + _ttl_inc )));
		else
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + _ttl_inc )));

	} else {
		it->second.value = _v;
		it->second.kill_ts += _ttl_inc;
	}
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
			//std::cout << "erasing " << it->first << std::endl; 
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
