#include "LruHash.h"

LruHash::Record::Record(const std::string &_value, uint64_t _kill_ts):
	 value(_value),
	 kill_ts(_kill_ts) {
	
}

LruHash::LruHash():
	 m_n_lru_shots(10),
	 m_default_ttl(0) {
	
}

void LruHash::set(const std::string &_k, const std::string &_v) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		if (m_default_ttl != 0)
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, time(0) + m_default_ttl)));
		else
			m_records.insert(std::pair<std::string,Record>(_k, Record(_v, 0)));
	} else
		it->second.value = _v;
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

bool LruHash::get(const std::string &_k, std::string &_v) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		return false;
	} else
		_v = it->second.value;
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
void LruHash::del(const std::string &_k) {
	
	hiaux::hashtable<std::string, Record>::iterator it = m_records.find(_k);
	m_records.erase(it);
}
