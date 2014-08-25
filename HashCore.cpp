#include "HashCore.h"

HashCore::HashCore() {
	
}

void HashCore::onSet(const std::string &_hash, const std::string &_k, const std::string &_v) {
	
	hLockTicketPtr ticket = m_lock.lock();
	m_hashes[_hash].set(_k, _v);
}

void HashCore::onSetWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc) {
	
	hLockTicketPtr ticket = m_lock.lock();
	m_hashes[_hash].setWithTtl(_k, _v, _ttl_inc);
}

void HashCore::onGet(const std::string &_hash, const std::string &_k, std::string &_v) {
	
	hLockTicketPtr ticket = m_lock.lock();
	m_hashes[_hash].get(_k, _v);
}

void HashCore::onGetWithTtl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_v) {
	
	hLockTicketPtr ticket = m_lock.lock();
	m_hashes[_hash].getWithTtl(_k, _ttl_inc, _v);
}

void HashCore::onDel(const std::string &_hash, const std::string &_k) {
	
	hLockTicketPtr ticket = m_lock.lock();
	m_hashes[_hash].del(_k);
}

void HashCore::onSetHashNlruShots(const std::string &_hash, uint64_t _n) {
	
	hLockTicketPtr ticket = m_lock.lock();
	m_hashes[_hash].setHashNlruShots(_n);
}

void HashCore::onSetHashDefaultTtl(const std::string &_hash, uint64_t _ttl) {
	
	hLockTicketPtr ticket = m_lock.lock();
	m_hashes[_hash].setHashDefaultTtl(_ttl);
}
