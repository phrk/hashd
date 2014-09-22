#ifndef _HASHD_API_H_
#define _HASHD_API_H_

#include "hiconfig.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include "hiaux/network/HttpApi.h"
#include <string>
#include <vector>
#include "hiaux/strings/string_utils.h"
#include "GetResp.h"

class HashdApi {
	
	HttpApiPtr m_api;
	
	std::string buildApiError(const std::string &_err);
	
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v)> m_onSet;
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc)> m_onSetWithTtl;
	boost::function<void(const std::string &_hash, const std::string &_k, bool &_exists, std::string &_v)> m_onGet;
	boost::function<void(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_v)> m_onGetWithTtl;
	boost::function<void(const std::string &_hash, const std::string &_k)> m_onDel;
	boost::function<void(const std::string &_hash, uint64_t _n)> m_onSetHashNlruShots;
	boost::function<void(const std::string &_hash, uint64_t _ttl)> m_onSetHashDefaultTtl;
	
public:
	
	HashdApi(boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v)> _onSet,
//			boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc)> _onSetWithTtl,
			boost::function<void(const std::string &_hash, const std::string &_k, bool &_exists, std::string &_v)> _onGet,
//			boost::function<void(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_v)> _onGetWithTtl,
			boost::function<void(const std::string &_hash, const std::string &_k)> _onDel
//			boost::function<void(const std::string &_hash, uint64_t _n)> _onSetHashNlruShots,
//			boost::function<void(const std::string &_hash, uint64_t _ttl)> _onSetHashDefaultTtl
				);
	
	void handle(HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	void onSet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
//	void onSetWithTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
	void onGet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
//	void onGetWithTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
	void onDel(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
//	void onSetHashNlruShots(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
//	void onSetHashDefaultTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
};

typedef boost::shared_ptr<HashdApi> HashdApiPtr;

#endif
