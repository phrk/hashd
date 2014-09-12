#ifndef _HASHD_CLIENT_ASYNC_H_
#define _HASHD_CLIENT_ASYNC_H_

#include <boost/shared_ptr.hpp>
#include "hiaux/network/HttpApiClient.h"
#include "hiaux/network/HttpSimpleRequester.h"
#include "hiaux/network/HttpSimpleRequesterPost.h"

#include "../GetResp.h"

#include "hiaux/crypt/base64.h"

class HashdContextBool : public CallContext {
public:
	boost::function<void(bool)> onDone;
	virtual ~HashdContextBool() { }
};

typedef boost::shared_ptr<HashdContextBool> HashdContextBoolPtr;

class HashdContextBoolBoolString : public CallContext {
public:
	boost::function<void(bool, bool, const std::string &)> onDone;
	virtual ~HashdContextBoolBoolString() { }
};

typedef boost::shared_ptr<HashdContextBoolBoolString> HashdContextBoolBoolStringPtr;

class HashdClientAsync {
	HttpApiClient m_http_api;
	HttpOutRequestDispPtr m_req_disp;
	
	void buildSetUrl (const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_url);
	void buildSetWithTtlUrl (const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_url);
	
	void buildGetUrl(const std::string &_hash, const std::string &_k, std::string &_url);
	void buildGetWithTtlUrl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_url);
	
	void buildDelUrl(const std::string &_hash, const std::string &_k, std::string &_url);
	void buildSetHashNlruShotsUrl(const std::string &_hash, uint64_t _n, std::string &_url);
	void buildSetHashDefaultTtlUrl(const std::string &_hash, uint64_t _ttl, std::string &_url);

	void onCalledContextBoolOk(CallContextPtr _context, const std::string &_resp);
	void onCalledContextBoolFail(CallContextPtr _context);
	
	void onCalledContextBoolBoolStringOk(CallContextPtr _context, const std::string &_resp);
	void onCalledContextBoolBoolStringFail(CallContextPtr _context);

public:
	
	HashdClientAsync(const std::string &_apiurl, HttpOutRequestDispPtr _req_disp);
	
	void set(const std::string &_hash, const std::string &_k, const std::string &_v, boost::function<void(bool)> _onDone);
//	void setWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, boost::function<void(bool)> _onDone);
	
	void get(const std::string &_hash, const std::string &_k, boost::function<void(bool, bool, const std::string &_v)> _onDone);
//	void getWithTtl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, boost::function<void(bool, const std::string &_v)> _onDone);
	
//	void del(const std::string &_hash, const std::string &_k, boost::function<void(bool)> _onDone);
//	void setHashNlruShots(const std::string &_hash, uint64_t _n, boost::function<void(bool)> _onDone);
//	void setHashDefaultTtl(const std::string &_hash, uint64_t _ttl, boost::function<void(bool)> _onDone);
};

typedef boost::shared_ptr<HashdClientAsync> HashdClientAsyncPtr;

#endif
