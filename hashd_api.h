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
#include "opts.pb.h"
#include "HashCoreErrors.h"

class HashdApi {
	
	HttpApiPtr m_api;
	
	std::string buildApiError(const std::string &_err);
	
	boost::function<void(const std::string &_name, const std::map<std::string, std::string> &_opts, int &_err)> m_onCreateHash;
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, int &_err)> m_onSetAndIncTtl;	
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, int &_err)> m_onSet;
	boost::function<void(const std::string &_hash, const std::string &_k, std::string &_v, int &_err)> m_onGet;
	boost::function<void(const std::string &_hash, const std::string &_k, std::string &_v, uint64_t &_ttl, int &_err)> m_onGetWithTtl;
	boost::function<void(const std::string &_hash, const std::string &_k, int &_err)> m_onDel;
	boost::function<void(const std::string &_hash, const std::string &_k, uint64_t &_ttl, int &_err)> m_onGetTtl;
	
public:
	
	HashdApi(boost::function<void(const std::string &_name, const std::map<std::string, std::string> &_opts, int &_err)> _onCreateHash,
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, int &_err)> _onSetAndIncTtl,
			boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, int &_err)> _onSet,
			boost::function<void(const std::string &_hash, const std::string &_k, std::string &_v, int &_err)> _onGet,
			boost::function<void(const std::string &_hash, const std::string &_k, std::string &_v, uint64_t &_ttl, int &_err)> _onGetWithTtl,
			boost::function<void(const std::string &_hash, const std::string &_k, int &_err)> _onDel,
			boost::function<void(const std::string &_hash, const std::string &_k, uint64_t &_ttl, int &_err)> _onGetTtl
				);
	
	void handle(HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	void onCreateHash(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
	void onSetAndIncTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);	
	void onSet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
	void onGet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
	void onGetWithTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
	void onDel(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
	void onGetTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone);
};

typedef boost::shared_ptr<HashdApi> HashdApiPtr;

#endif
