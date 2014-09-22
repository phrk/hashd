#ifndef _HASHD_H_
#define _HASHD_H_

#include "hiconfig.h"

#include "hiaux/tools/Daemon.h"
#include "hashd_api.h"

#include "HashCore.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

class Hashd : public Daemon {

	HashdApiPtr m_api;
	HashCorePtr m_core;
	
public:
	
	Hashd(const std::string &_config_file);
	
	virtual void setParamsList(std::vector<std::string> &_required_params, std::vector<std::string> &_optional_params);
	virtual void doStart();
	
	virtual void connHandler(HttpConnectionPtr _conn, HttpRequestPtr _req);
};

#endif
