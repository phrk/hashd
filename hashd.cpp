#include "hashd.h"

void Hashd::setParamsList(std::vector<std::string> &_required_params, std::vector<std::string> &_optional_params) {
	
	_required_params.push_back("nthreads");
	_required_params.push_back("listen_port");
}

Hashd::Hashd(const std::string &_config_file) {
	
	loadConfig(_config_file);
	startListening(strtoint(m_config["nthreads"]), strtoint(m_config["listen_port"]));
}

void Hashd::doStart() {
	
	std::cout << "hashd started on port " << m_config["listen_port"] << std::endl;
}

void Hashd::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {	
	
}
