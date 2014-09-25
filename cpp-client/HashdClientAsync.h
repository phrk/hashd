#ifndef _HASHD_CLIENT_ASYNC_H_
#define _HASHD_CLIENT_ASYNC_H_

#include <boost/shared_ptr.hpp>
#include "hiaux/network/HttpApiClient.h"
#include "hiaux/network/HttpSimpleRequester.h"
#include "hiaux/network/HttpSimpleRequesterPost.h"

#include "../HashCoreErrors.h"

#include "../GetResp.h"
#include "../opts.pb.h"

#include "hiaux/crypt/base64.h"

class HashdContextIntUint64 : public CallContext {
public:
	boost::function<void(int, uint64_t)> onDone;
	virtual ~HashdContextIntUint64() { }
};

typedef boost::shared_ptr<HashdContextIntUint64> HashdContextIntUint64Ptr;

class HashdContextInt : public CallContext {
public:
	boost::function<void(int)> onDone;
	virtual ~HashdContextInt() { }
};

typedef boost::shared_ptr<HashdContextInt> HashdContextIntPtr;

class HashdContextBool : public CallContext {
public:
	boost::function<void(bool)> onDone;
	virtual ~HashdContextBool() { }
};

typedef boost::shared_ptr<HashdContextBool> HashdContextBoolPtr;

class HashdContextIntString : public CallContext {
public:
	boost::function<void(int, const std::string &)> onDone;
	virtual ~HashdContextIntString() { }
};

typedef boost::shared_ptr<HashdContextIntString> HashdContextIntStringPtr;

class HashdClientAsync {
	HttpApiClient m_http_api;
	HttpOutRequestDispPtr m_req_disp;
	
	void buildCreateHashUrl(const std::string &_name, std::string &_url);
	void buildSetUrl (const std::string &_hash, const std::string &_k, std::string &_url);
	void buildSetAndIncTtlUrl(const std::string &_hash, const std::string &_k, std::string &_url);
	void buildGetUrl(const std::string &_hash, const std::string &_k, std::string &_url);
	void buildDelUrl(const std::string &_hash, const std::string &_k, std::string &_url);
	void buildGetTtlUrl(const std::string &_hash, const std::string &_k, std::string &_url);
	
	/*
	void buildSetWithTtlUrl (const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_url);
	
	void buildGetUrl(const std::string &_hash, const std::string &_k, std::string &_url);
	void buildGetWithTtlUrl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_url);
	
	void buildSetHashNlruShotsUrl(const std::string &_hash, uint64_t _n, std::string &_url);
	void buildSetHashDefaultTtlUrl(const std::string &_hash, uint64_t _ttl, std::string &_url);
	*/

	void onCalledContextIntOk(CallContextPtr _context, const std::string &_resp);
	void onCalledContextIntFail(CallContextPtr _context);

	void onCalledContextIntStringOk(CallContextPtr _context, const std::string &_resp);
	void onCalledContextIntStringFail(CallContextPtr _context);

	void onCalledContextIntUint64Ok(CallContextPtr _context, const std::string &_resp);
	void onCalledContextIntUint64Fail(CallContextPtr _context);
	/*
	void onCalledContextBoolOk(CallContextPtr _context, const std::string &_resp);
	void onCalledContextBoolFail(CallContextPtr _context);
	
	*/

public:
	
	HashdClientAsync(const std::string &_apiurl, HttpOutRequestDispPtr _req_disp);
	
	void createHash(const std::string &_name, const std::map<std::string, std::string> &_opts, boost::function<void(int _err)> _onDone);
	void set(const std::string &_hash, const std::string &_k, const std::string &_v, boost::function<void(int _err)> _onDone);
	void setAndIncTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t _ttl_inc, boost::function<void(int _err)> _onDone);
	void get(const std::string &_hash, const std::string &_k, boost::function<void(int _err, const std::string &_v)> _onDone);
	void del(const std::string &_hash, const std::string &_k, boost::function<void(int _err)> _onDone);
	void getTtl(const std::string &_hash, const std::string &_k, boost::function<void(int _err, uint64_t _ttl)> _onDone);
};

typedef boost::shared_ptr<HashdClientAsync> HashdClientAsyncPtr;

#endif
