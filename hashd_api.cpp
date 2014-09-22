#include "hashd_api.h"

std::string HashdApi::buildApiError(const std::string &_err) {
	return "{ \"status_code\" : \"-1\", \"status\" : \"" + _err + "\" }";
}

HashdApi::HashdApi(boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v)> _onSet,
//		boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc)> _onSetWithTtl,
		boost::function<void(const std::string &_hash, const std::string &_k, bool &_exists, std::string &_v)> _onGet,
//		boost::function<void(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_v)> _onGetWithTtl,
		boost::function<void(const std::string &_hash, const std::string &_k)> _onDel
//		boost::function<void(const std::string &_hash, uint64_t _n)> _onSetHashNlruShots,
//		boost::function<void(const std::string &_hash, uint64_t _ttl)> _onSetHashDefaultTtl
		):
	
	m_onSet(_onSet),
//	m_onSetWithTtl(_onSetWithTtl),
	m_onGet(_onGet),
//	m_onGetWithTtl(_onGetWithTtl),
	m_onDel(_onDel)
//	m_onSetHashNlruShots(_onSetHashNlruShots),
//	m_onSetHashDefaultTtl(_onSetHashDefaultTtl) 
	{
	
	m_api.reset(new HttpApi( boost::bind(&HashdApi::buildApiError, this, _1)));
	
	m_api->addUser("_user_", "_key_");
	
	std::vector<std::string> args_names;

	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	m_api->addMethodSignedAsync("set", args_names, boost::bind(&HashdApi::onSet, this, _1, _2));
	args_names.clear();
	/*
	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	args_names.push_back("ttl-inc");
	m_api->addMethodSignedAsync("set-with-ttl", args_names, boost::bind(&HashdApi::onSetWithTtl, this, _1, _2));
	args_names.clear();
	*/
	args_names.push_back("hash");
	args_names.push_back("k");
	m_api->addMethodSignedAsync("get", args_names, boost::bind(&HashdApi::onGet, this, _1, _2));
	args_names.clear();
	/*
	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("ttl-inc");
	m_api->addMethodSignedAsync("get-with-ttl", args_names, boost::bind(&HashdApi::onGetWithTtl, this, _1, _2));
	args_names.clear();
	*/
	args_names.push_back("hash");
	args_names.push_back("k");
	m_api->addMethodSignedAsync("del", args_names, boost::bind(&HashdApi::onDel, this, _1, _2));
	args_names.clear();
	/*
	args_names.push_back("hash");
	args_names.push_back("n");
	m_api->addMethodSignedAsync("set-hash-n-lru-shots", args_names, boost::bind(&HashdApi::onSetHashNlruShots, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("ttl");
	m_api->addMethodSignedAsync("set-hash-default-ttl", args_names, boost::bind(&HashdApi::onSetHashDefaultTtl, this, _1, _2));
	args_names.clear();
	*/
}

void HashdApi::handle(HttpConnectionPtr _conn, HttpRequestPtr _req) {

	m_api->handle(_conn, _req);
}

void HashdApi::onSet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	m_onSet(_params["hash"], _params["k"], _params["v"]);
	_onDone("{ \"status\": \"ok\" }");
}
/*
void HashdApi::onSetWithTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	uint64_t ttl_inc = string_to_uint64(_params["ttl-inc"]);
	m_onSetWithTtl(_params["hash"], _params["k"], _params["v"], ttl_inc);
	_onDone("{ \"status\": \"ok\" }");
}*/

void HashdApi::onGet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	
	GetResp pb;
	m_onGet(_params["hash"], _params["k"], pb.exists, pb.value);
	
	std::string resp;
	pb.dump(resp);
	_onDone(resp);
}
/*
void HashdApi::onGetWithTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	uint64_t ttl_inc = string_to_uint64(_params["ttl-inc"]);
	std::string v;
	m_onGetWithTtl(_params["hash"], _params["k"], ttl_inc, v);
	_onDone(v);
}*/

void HashdApi::onDel(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	m_onDel(_params["hash"], _params["k"]);
	_onDone("{ \"status\": \"ok\" }");
}
/*
void HashdApi::onSetHashNlruShots(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	uint64_t n = string_to_uint64(_params["n"]);
	m_onSetHashNlruShots(_params["hash"], n);
	_onDone("{ \"status\": \"ok\" }");
}

void HashdApi::onSetHashDefaultTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	uint64_t ttl = string_to_uint64(_params["ttl"]);
	m_onSetHashNlruShots(_params["hash"], ttl);
	_onDone("{ \"status\": \"ok\" }");
}
*/