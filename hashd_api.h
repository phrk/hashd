#ifndef _HASHD_H_
#define _HASHD_H_

#include <boost/shared_ptr.hpp>
#include "hiaux/network/HttpApi.h"
#include "hiaux/strings/string_utils.h"

class HashdApi {
	
	HttpApiPtr m_api;
	
	std::string buildApiError(const std::string &_err);
	
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> m_onSet;
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp)> m_onSetWithTtl;
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> m_onGet;
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp)> m_onGetWithTtl;
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> m_onDel;
	boost::function<void(const std::string &_hash, uint64_t _n, std::string &_resp)> m_onSetHashNlruShots;
	boost::function<void(const std::string &_hash, uint64_t _ttl, std::string &_resp)> m_onSetHashDefaultTtl;
	
public:
	
	HashdApi(boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> _onSet,
			boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp)> _onSetWithTtl,
			boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> _onGet,
			boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp)> _onGetWithTtl,
			boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> _onDel,
			boost::function<void(const std::string &_hash, uint64_t _n, std::string &_resp)> _onSetHashNlruShots,
			boost::function<void(const std::string &_hash, uint64_t _ttl, std::string &_resp)> _onSetHashDefaultTtl);
	
	void handle(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
	void onSet(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp);
	void onSetWithTtl(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp);
	void onGet(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp);
	void onGetWithTtl(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp);
	void onDel(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp);
	void onSetHashNlruShots(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp);
	void onSetHashDefaultTtl(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp);
};

typedef boost::shared_ptr<HashdApi> HashdApiPtr;

#endif
