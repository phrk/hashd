#include "hashd_api.h"

std::string HashdApi::buildApiError(const std::string &_err) {
	return "{ \"status_code\" : \"-1\", \"status\" : \"" + _err + "\" }";
}

HashdApi::HashdApi(
	boost::function<void(const std::string &_name, const std::map<std::string, std::string> &_opts, int &_err)> _onCreateHash,
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, int &_err)> _onSetAndIncTtl,	
	boost::function<void(const std::string &_hash, const std::string &_k, const std::string &_v, int &_err)> _onSet,
	boost::function<void(const std::string &_hash, const std::string &_k, std::string &_v, int &_err)> _onGet,
	boost::function<void(const std::string &_hash, const std::string &_k, std::string &_v, uint64_t &_ttl, int &_err)> _onGetWithTtl,
	boost::function<void(const std::string &_hash, const std::string &_k, int &_err)> _onDel,
	boost::function<void(const std::string &_hash, const std::string &_k, uint64_t &_ttl, int &_err)> _onGetTtl):

	m_onCreateHash(_onCreateHash),
	m_onSet(_onSet),
	m_onSetAndIncTtl(_onSetAndIncTtl),
	m_onGet(_onGet),
	m_onGetWithTtl(_onGetWithTtl),
	m_onDel(_onDel),
	m_onGetTtl(_onGetTtl)
	{
	
	m_api.reset(new HttpApi( boost::bind(&HashdApi::buildApiError, this, _1)));
	
	m_api->addUser("_user_", "_key_");
	
	std::vector<std::string> args_names;

	args_names.push_back("name");
	args_names.push_back("opts");
	m_api->addMethodSignedAsync("create-hash", args_names, boost::bind(&HashdApi::onCreateHash, this, _1, _2));
	args_names.clear();

	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	m_api->addMethodSignedAsync("set", args_names, boost::bind(&HashdApi::onSet, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("k");
	args_names.push_back("v");
	args_names.push_back("ttl-inc");
	m_api->addMethodSignedAsync("set-and-inc-ttl", args_names, boost::bind(&HashdApi::onSetAndIncTtl, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("k");
	m_api->addMethodSignedAsync("get", args_names, boost::bind(&HashdApi::onGet, this, _1, _2));
	args_names.clear();

	args_names.push_back("hash");
	args_names.push_back("k");
	m_api->addMethodSignedAsync("get-with-ttl", args_names, boost::bind(&HashdApi::onGetWithTtl, this, _1, _2));
	args_names.clear();

	args_names.push_back("hash");
	args_names.push_back("k");
	m_api->addMethodSignedAsync("del", args_names, boost::bind(&HashdApi::onDel, this, _1, _2));
	args_names.clear();
	
	args_names.push_back("hash");
	args_names.push_back("k");
	m_api->addMethodSignedAsync("get-ttl", args_names, boost::bind(&HashdApi::onGetTtl, this, _1, _2));
	args_names.clear();
}

void HashdApi::handle(HttpConnectionPtr _conn, HttpRequestPtr _req) {

	m_api->handle(_conn, _req);
}

void HashdApi::onCreateHash(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	std::map<std::string, std::string> opts;
		
	OptsPb opts_pb;
	opts_pb.ParseFromString( base64_decode( _params["opts"] ) );
	
	for (int i = 0; i<opts_pb.opts_size(); i++) {

		PairPb p = opts_pb.opts(i);
		opts.insert( std::pair<std::string, std::string> ( p.key() , p.value() ));
	}
	
	int err;
	char bf[255];
	
	m_onCreateHash( _params["name"], opts, err);
				
	sprintf(bf, "%d", err);
	_onDone(bf);
}

void HashdApi::onSet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	int err;
	char bf[255];
	
	m_onSet(_params["hash"], _params["k"], _params["v"], err);
			
	sprintf(bf, "%d", err);
	_onDone(bf);
}

void HashdApi::onSetAndIncTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	uint64_t ttl_inc = string_to_uint64(_params["ttl-inc"]);
	int err;
	char bf[255];
	
	m_onSetAndIncTtl(_params["hash"], _params["k"], _params["v"], ttl_inc, err);
				
	sprintf(bf, "%d", err);
	_onDone(bf);
}

void HashdApi::onGet(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	int err;
	GetResp pb;
	m_onGet(_params["hash"], _params["k"], pb.value, pb.err);
	
	std::string resp;
	pb.dump(resp);
	
	_onDone( base64_encode ( (unsigned char *)resp.c_str(), resp.size() ) );
}

void HashdApi::onGetWithTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	int err;
	GetResp pb;
	m_onGetWithTtl(_params["hash"], _params["k"], pb.value, pb.ttl, pb.err);
	
	std::string resp;
	pb.dump(resp);
	
	_onDone( base64_encode ( (unsigned char *)resp.c_str(), resp.size() ) );
}

void HashdApi::onDel(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	int err;
	char bf[255];
	m_onDel(_params["hash"], _params["k"], err);
				
	sprintf(bf, "%d", err);
	_onDone(bf);
}

void HashdApi::onGetTtl(hiaux::hashtable<std::string, std::string> &_params, boost::function< void(const std::string&)> _onDone) {
	
	int err;
	char bf[50];
	uint64_t ttl;
	
	GetResp pb;
	m_onGetTtl(_params["hash"], _params["k"], ttl, pb.err);
	
	pb.value = uint64_to_string(ttl);
	
	std::string resp;
	pb.dump(resp);
	
	_onDone( base64_encode ( (unsigned char *)resp.c_str(), resp.size() ) );
}
