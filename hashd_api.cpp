#include "hashd_api.h"

std::string HashdApi::buildApiError(const std::string &_err) {
	return "{ \"status_code\" : \"-1\", \"status\" : \"" + _err + "\" }";
}

HashdApi::HashdApi(boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> _onSet,
		boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp)> _onSetWithTtl,
		boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> _onGet,
		boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_resp)> _onGetWithTtl,
		boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, std::string &_resp)> _onDel,
		boost::function<void(const std::string &_hash, uint64_t _n, std::string &_resp)> _onSetHashNlruShots,
		boost::function<void(const std::string &_hash, uint64_t _ttl, std::string &_resp)> _onSetHashDefaultTtl):
	
	m_onSet(_onSet),
	m_onSetWithTtl(_onSetWithTtl),
	m_onGet(_onGet),
	m_onGetWithTtl(_onGetWithTtl),
	m_onDel(_onDel),
	m_onSetHashNlruShots(_onSetHashNlruShots),
	m_onSetHashDefaultTtl(_onSetHashDefaultTtl) {
	
	m_api.reset(new HttpApi( boost::bind(&HashdApi::buildApiError, this, _1)));
	
	std::vector<std::string> args_names;

	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	m_api->addMethod("set", args_names, boost::bind(&HashdApi::onSet, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	args_names.push_back("ttl-inc");
	m_api->addMethod("set-with-ttl", args_names, boost::bind(&HashdApi::onSetWithTtl, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	m_api->addMethod("get", args_names, boost::bind(&HashdApi::onGet, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	args_names.push_back("ttl-inc");
	m_api->addMethod("get-with-ttl", args_names, boost::bind(&HashdApi::onGetWithTtl, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	m_api->addMethod("del", args_names, boost::bind(&HashdApi::onDel, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("n");
	m_api->addMethod("set-hash-n-lru-shots", args_names, boost::bind(&HashdApi::onSetHashNlruShots, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("ttl");
	m_api->addMethod("set-hash-default-ttl", args_names, boost::bind(&HashdApi::onSetHashDefaultTtl, this, _1, _2));
	args_names.clear();
}

void HashdApi::handle(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {

	m_api->handle(_conn, _req);
}

void HashdApi::onSet(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp) {
	
}

void HashdApi::onSetWithTtl(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp) {
	
}

void HashdApi::onGet(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp) {
	
}

void HashdApi::onGetWithTtl(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp) {
	
}

void HashdApi::onDel(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp) {
	
}

void HashdApi::onSetHashNlruShots(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp) {
	
}

void HashdApi::onSetHashDefaultTtl(hiaux::hashtable<std::string, std::string> &_params, std::string &_resp) {
	
}
