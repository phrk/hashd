#include "HashdClientAsync.h"

HashdClientAsync::HashdClientAsync(const std::string &_apiurl, HttpOutRequestDispPtr _req_disp):
	m_http_api(_apiurl),
	m_req_disp(_req_disp) {
	
}

void HashdClientAsync::buildSetUrl (const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	get_params["v"] = _v;
	
	m_http_api.buildRequestUrlSigned("set", get_params, _url);
}

void HashdClientAsync::buildSetWithTtlUrl (const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	get_params["v"] = _v;
	get_params["ttl-inc"] = uint64_to_string(_ttl_inc);
	
	m_http_api.buildRequestUrlSigned("set-with-ttl", get_params, _url);
}

void HashdClientAsync::buildGetUrl(const std::string &_hash, const std::string &_k, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	
	m_http_api.buildRequestUrlSigned("get", get_params, _url);
}

void HashdClientAsync::buildGetWithTtlUrl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	get_params["ttl-inc"] = uint64_to_string(_ttl_inc);
	
	m_http_api.buildRequestUrlSigned("get-with-ttl", get_params, _url);
}

void HashdClientAsync::buildDelUrl(const std::string &_hash, const std::string &_k, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	
	m_http_api.buildRequestUrlSigned("del", get_params, _url);
}

void HashdClientAsync::buildSetHashNlruShotsUrl(const std::string &_hash, uint64_t _n, std::string &_url) {

	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["n"] = uint64_to_string(_n);
	
	m_http_api.buildRequestUrlSigned("set-hash-n-lru-shots", get_params, _url);
}

void HashdClientAsync::buildSetHashDefaultTtlUrl(const std::string &_hash, uint64_t _ttl, std::string &_url) {

	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["ttl"] = uint64_to_string(_ttl);
	
	m_http_api.buildRequestUrlSigned("set-hash-default-ttl", get_params, _url);	
}

void HashdClientAsync::onCalledContextBoolOk(CallContextPtr _context, const std::string &_resp) {
	
	HashdContextBool* context = ( (HashdContextBool*) _context.get() );
	context->onDone(true);
}

void HashdClientAsync::onCalledContextBoolFail(CallContextPtr _context) {
	
	HashdContextBool* context = ( (HashdContextBool*) _context.get() );
	context->onDone(false);
}

void HashdClientAsync::onCalledContextBoolStringOk(CallContextPtr _context, const std::string &_resp) {
	
	HashdContextBoolString* context = ( (HashdContextBoolString*) _context.get() );
	context->onDone(true, _resp);
}

void HashdClientAsync::onCalledContextBoolStringFail(CallContextPtr _context) {
	
	HashdContextBoolString* context = ( (HashdContextBoolString*) _context.get() );
	context->onDone(false, "");
}

// Public methods

void HashdClientAsync::set(const std::string &_hash, const std::string &_k, const std::string &_v, boost::function<void(bool)> _onDone) {
	
	std::string url;
	buildSetUrl(_hash, _k, _v, url);
	
	HashdContextBoolPtr context (new HashdContextBool);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextBoolOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextBoolFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::setWithTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, boost::function<void(bool)> _onDone) {

	std::string url;
	buildSetWithTtlUrl(_hash, _k, _v, _ttl_inc, url);
	
	HashdContextBoolPtr context (new HashdContextBool);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextBoolStringOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextBoolStringFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::get(const std::string &_hash, const std::string &_k, boost::function<void(bool, const std::string &_v)> _onDone) {
	
	std::string url;
	buildGetUrl(_hash, _k, url);
	
	HashdContextBoolStringPtr context (new HashdContextBoolString);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextBoolStringOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextBoolStringFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::getWithTtl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, boost::function<void(bool, const std::string &_v)> _onDone) {
	
	std::string url;
	buildGetWithTtlUrl(_hash, _k, _ttl_inc, url);
	
	HashdContextBoolStringPtr context (new HashdContextBoolString);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextBoolStringOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextBoolStringFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::del(const std::string &_hash, const std::string &_k, boost::function<void(bool)> _onDone) {

	std::string url;
	buildDelUrl(_hash, _k, url);
	
	HashdContextBoolPtr context (new HashdContextBool);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextBoolStringOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextBoolStringFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::setHashNlruShots(const std::string &_hash, uint64_t _n, boost::function<void(bool)> _onDone) {

	std::string url;
	buildSetHashNlruShotsUrl(_hash, _n, url);
	
	HashdContextBoolPtr context (new HashdContextBool);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextBoolStringOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextBoolStringFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::setHashDefaultTtl(const std::string &_hash, uint64_t _ttl, boost::function<void(bool)> _onDone) {

	std::string url;
	buildSetHashDefaultTtlUrl(_hash, _ttl, url);
	
	HashdContextBoolPtr context (new HashdContextBool);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextBoolStringOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextBoolStringFail, this, _1)));

	m_req_disp->addRequester(requester);
}
