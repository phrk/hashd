#ifndef _HASHD_H_
#define _HASHD_H_

#include "hiaux/tools/Daemon.h"
#include "hashd_api.h"

class Hashd : public Daemon {

public:
	
	Hashd(const std::string &_config_file);
	
	virtual void setParamsList(std::vector<std::string> &_required_params, std::vector<std::string> &_optional_params);
	virtual void doStart();
	
	virtual void connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
};

#endif