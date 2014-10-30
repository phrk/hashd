#include "HashdClientAsync.h"

HashdClientAsync::HashdClientAsync(const std::string &_apiurl, HttpOutRequestDispPtr _req_disp):
	m_http_api(_apiurl, "_user_", "_key_"),
	m_req_disp(_req_disp) {
	
}


void HashdClientAsync::buildCreateHashUrl(const std::string &_name, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["name"] = _name;
	// other in post
	
	m_http_api.buildRequestUrlSigned("create-hash", get_params, _url);
}

void HashdClientAsync::buildSetUrl (const std::string &_hash, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	//get_params["k"] = _k;
	// other in post
	
	m_http_api.buildRequestUrlSigned("set", get_params, _url);
}

void HashdClientAsync::buildSetAndIncTtlUrl(const std::string &_hash, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	//get_params["k"] = _k;
	// other in post
	
	m_http_api.buildRequestUrlSigned("set-and-inc-ttl", get_params, _url);
}

void HashdClientAsync::buildGetUrl(const std::string &_hash, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	//get_params["k"] = _k;
	
	m_http_api.buildRequestUrlSigned("get", get_params, _url);
}

void HashdClientAsync::buildGetWithTtlUrl(const std::string &_hash, std::string &_url) {

	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	//get_params["k"] = _k;
	
	m_http_api.buildRequestUrlSigned("get-with-ttl", get_params, _url);
}

void HashdClientAsync::buildDelUrl(const std::string &_hash, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	//get_params["k"] = _k;
	
	m_http_api.buildRequestUrlSigned("del", get_params, _url);
}

void HashdClientAsync::buildGetTtlUrl(const std::string &_hash, const std::string &_k, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	
	m_http_api.buildRequestUrlSigned("get-ttl", get_params, _url);
}

/*
void HashdClientAsync::buildSetWithTtlUrl (const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t &_ttl_inc, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	get_params["v"] = _v;
	get_params["ttl-inc"] = uint64_to_string(_ttl_inc);
	
	m_http_api.buildRequestUrlSigned("set-with-ttl", get_params, _url);
}


void HashdClientAsync::buildGetWithTtlUrl(const std::string &_hash, const std::string &_k, uint64_t &_ttl_inc, std::string &_url) {
	
	hiaux::hashtable<std::string, std::string> get_params;
	get_params["hash"] = _hash;
	get_params["k"] = _k;
	get_params["ttl-inc"] = uint64_to_string(_ttl_inc);
	
	m_http_api.buildRequestUrlSigned("get-with-ttl", get_params, _url);
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

void HashdClientAsync::onCalledContextBoolBoolStringOk(CallContextPtr _context, const std::string &_resp) {
	
	// parse _resp
	GetResp pb;
	pb.restore(_resp);
	
	HashdContextBoolBoolString* context = ( (HashdContextBoolBoolString*) _context.get() );
	context->onDone(true, pb.exists, pb.value);
}

void HashdClientAsync::onCalledContextBoolBoolStringFail(CallContextPtr _context) {
	
	HashdContextBoolBoolString* context = ( (HashdContextBoolBoolString*) _context.get() );
	context->onDone(false, false, "");
}
*/

void HashdClientAsync::onCalledContextIntOk(CallContextPtr _context, const std::string &_resp) {
	
	HashdContextInt* context = ( (HashdContextInt*) _context.get() );
	
	int err;
	sscanf(_resp.c_str(), "%d", &err);
	
	context->onDone(err);
}

void HashdClientAsync::onCalledContextIntFail(CallContextPtr _context) {
	
	HashdContextInt* context = ( (HashdContextInt*) _context.get() );
	context->onDone(E_HC_CONNECTIVITY_ERROR);
}

void HashdClientAsync::onCalledContextIntStringOk(CallContextPtr _context, const std::string &_resp) {
	
	HashdContextIntString* context = ( (HashdContextIntString*) _context.get() );
	
	GetResp pb;
	pb.restore(base64_decode(_resp));
	
	//std::cout << "HashdClientAsync::onCalledContextIntStringOk " << _resp << std::endl;
	
	context->onDone(pb.err, pb.value);
}

void HashdClientAsync::onCalledContextIntStringFail(CallContextPtr _context) {
	
	HashdContextIntString* context = ( (HashdContextIntString*) _context.get() );
	context->onDone(E_HC_CONNECTIVITY_ERROR, "");
}

void HashdClientAsync::onCalledContextIntUint64Ok(CallContextPtr _context, const std::string &_resp) {
	
	HashdContextIntUint64* context = ( (HashdContextIntUint64*) _context.get() );
	
	GetResp pb;
	pb.restore(base64_decode(_resp));
	
	context->onDone(pb.err, string_to_uint64(pb.value));
}

void HashdClientAsync::onCalledContextIntUint64Fail(CallContextPtr _context) {
	
	HashdContextIntUint64* context = ( (HashdContextIntUint64*) _context.get() );
	context->onDone(E_HC_CONNECTIVITY_ERROR, 0);
}

void HashdClientAsync::onCalledContextIntStringUint64Ok(CallContextPtr _context, const std::string &_resp) {
		
	HashdContextIntStringUint64* context = ( (HashdContextIntStringUint64*) _context.get() );
	
	GetResp pb;
	pb.restore(base64_decode(_resp));
	
	context->onDone(pb.err, pb.value, pb.ttl);
}

void HashdClientAsync::onCalledContextIntStringUint64Fail(CallContextPtr _context) {
	
	HashdContextIntStringUint64* context = ( (HashdContextIntStringUint64*) _context.get() );
	context->onDone(E_HC_CONNECTIVITY_ERROR, "", 0);
}


///////////////////////// Public methods

void HashdClientAsync::createHash(const std::string &_name, const std::map<std::string, std::string> &_opts, boost::function<void(int _err)> _onDone) {
	
	std::string url;
	buildCreateHashUrl(_name, url);
	
	OptsPb opts_pb;
	
	std::map<std::string, std::string>::const_iterator it = _opts.begin();
	std::map<std::string, std::string>::const_iterator end = _opts.end();
	
	while (it != end) {
		
		PairPb *pair_pb = opts_pb.add_opts();
		pair_pb->set_key(it->first);
		pair_pb->set_value(it->second);
		
		it++;
	}
	
	//std::cout << "HashdClientAsync::createHash: _opts.size() " << _opts.size() << std::endl;
	
	std::string opts_dump = opts_pb.SerializeAsString();
	
	HttpApiPostData pb;
	HttpApiPostDataField *field = pb.add_fields();
	field->set_field("opts");
	field->set_value( base64_encode( (const unsigned char*) opts_dump.c_str(), opts_dump.size()) );

	std::string tmp_postdata = pb.SerializeAsString();
	
	std::string postdata = base64_encode((unsigned char *)tmp_postdata.c_str(), tmp_postdata.size());
	
	HashdContextIntPtr context (new HashdContextInt);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequesterPost (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
										boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										postdata,
										boost::bind(&HashdClientAsync::onCalledContextIntOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextIntFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::set(const std::string &_hash, const std::string &_k, const std::string &_v, boost::function<void(int _err)> _onDone) {
	
	std::string url;
	buildSetUrl(_hash, url);
	
	HttpApiPostData pb;
	HttpApiPostDataField *field = pb.add_fields();
	field->set_field("v");
	field->set_value(_v);
	
	{
		HttpApiPostDataField *field = pb.add_fields();
		field->set_field("k");
		field->set_value(_k);
	}
	
	std::string tmp_postdata = pb.SerializeAsString();
	
	std::string postdata = base64_encode((unsigned char *)tmp_postdata.c_str(), tmp_postdata.size());
	
	HashdContextIntPtr context (new HashdContextInt);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequesterPost (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										postdata,
										boost::bind(&HashdClientAsync::onCalledContextIntOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextIntFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::setAndIncTtl(const std::string &_hash, const std::string &_k, const std::string &_v, uint64_t _ttl_inc, boost::function<void(int _err)> _onDone) {
	
	std::string url;
	buildSetAndIncTtlUrl(_hash, url);
	
	HttpApiPostData pb;
	{
		HttpApiPostDataField *field = pb.add_fields();
		field->set_field("v");
		field->set_value(_v);
	}
	{
		HttpApiPostDataField *field = pb.add_fields();
		field->set_field("k");
		field->set_value(_k);
	}
	{
		HttpApiPostDataField *field = pb.add_fields();
		field->set_field("ttl-inc");
		field->set_value( uint64_to_string(_ttl_inc) );
	}

		
	std::string tmp_postdata = pb.SerializeAsString();
	
	std::string postdata = base64_encode((unsigned char *)tmp_postdata.c_str(), tmp_postdata.size());
	
	HashdContextIntPtr context (new HashdContextInt);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequesterPost (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										postdata,
										boost::bind(&HashdClientAsync::onCalledContextIntOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextIntFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::get(const std::string &_hash, const std::string &_k, boost::function<void(int _err, const std::string &_v)> _onDone) {
	
	std::string url;
	buildGetUrl(_hash, url);
	
	HttpApiPostData pb;
	{
		HttpApiPostDataField *field = pb.add_fields();
		field->set_field("k");
		field->set_value(_k);
	}
	
	std::string tmp_postdata = pb.SerializeAsString();
	std::string postdata = base64_encode((unsigned char *)tmp_postdata.c_str(), tmp_postdata.size());
	
	HashdContextIntStringPtr context (new HashdContextIntString);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequesterPost (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										postdata,
										boost::bind(&HashdClientAsync::onCalledContextIntStringOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextIntStringFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::getWithTtl(const std::string &_hash,
									const std::string &_k,
									boost::function<void(int _err, const std::string &_v, uint64_t _ttl)> _onDone) {
	std::string url;
	buildGetWithTtlUrl(_hash, url);
	
	HttpApiPostData pb;
	{
		HttpApiPostDataField *field = pb.add_fields();
		field->set_field("k");
		field->set_value(_k);
	}

	std::string tmp_postdata = pb.SerializeAsString();
	std::string postdata = base64_encode((unsigned char *)tmp_postdata.c_str(), tmp_postdata.size());

	HashdContextIntStringUint64Ptr context (new HashdContextIntStringUint64);
	context->onDone = _onDone;

	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequesterPost (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										postdata,
										boost::bind(&HashdClientAsync::onCalledContextIntStringUint64Ok, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextIntStringUint64Fail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::del(const std::string &_hash, const std::string &_k, boost::function<void(int)> _onDone) {

	std::string url;
	buildDelUrl(_hash, url);
	
	HttpApiPostData pb;
	{
		HttpApiPostDataField *field = pb.add_fields();
		field->set_field("k");
		field->set_value(_k);
	}
	
	std::string tmp_postdata = pb.SerializeAsString();
	std::string postdata = base64_encode((unsigned char *)tmp_postdata.c_str(), tmp_postdata.size());
	
	HashdContextIntPtr context (new HashdContextInt);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequesterPost (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										postdata,
										boost::bind(&HashdClientAsync::onCalledContextIntOk, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextIntFail, this, _1)));

	m_req_disp->addRequester(requester);
}

void HashdClientAsync::getTtl(const std::string &_hash, const std::string &_k, boost::function<void(int _err, uint64_t _ttl)> _onDone) {
	
	std::string url;
	buildGetTtlUrl(_hash, _k, url);
	
	HashdContextIntUint64Ptr context (new HashdContextIntUint64);
	context->onDone = _onDone;
	
	HttpOutRequestDisp::RequesterPtr requester
		(new HttpSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
									boost::bind(&HttpOutRequestDisp::onCallPost, m_req_disp.get(), _1, _2, _3, _4),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										context,
										url,
										boost::bind(&HashdClientAsync::onCalledContextIntUint64Ok, this, _1, _2),
										boost::bind(&HashdClientAsync::onCalledContextIntUint64Fail, this, _1)));

	m_req_disp->addRequester(requester);
}

/*
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
*/