#include "HashCore.h"


Hash::Hash() {	
}

void Hash::set(const std::string &_k, const std::string &_v) {
	
	hiaux::hashtable<std::string, std::string>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
			m_records.insert(std::pair<std::string, std::string>(_k, _v));
	} else
		it->second = _v;
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

bool Hash::get(const std::string &_k, std::string &_v) {
	
	hiaux::hashtable<std::string, std::string>::iterator it = m_records.find(_k);
	
	if (it == m_records.end()) {
		
		return false;
	} else
		_v = it->second;
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
void Hash::del(const std::string &_k) {
	
	hiaux::hashtable<std::string, std::string>::iterator it = m_records.find(_k);
	m_records.erase(it);
}
/*
void HashCore::Hash::setHashNlruShots(uint64_t _n) {
	
	m_n_lru_shots = _n;
}

void HashCore::Hash::setHashDefaultTtl(uint64_t _ttl) {
	
	m_default_ttl = _ttl;
}
*/