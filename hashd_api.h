#ifndef _HASHD_H_
#define _HASHD_H_

#include <boost/shared_ptr.hpp>
#include "hiaux/network/HttpApi.h"

class HashdApi {
	
	HttpApiPtr m_api;
	
	std::string buildApiError(const std::string &_err);
public:
	
	HashdApi();
	
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
