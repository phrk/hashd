#include "hashd.h"

void Hashd::setParamsList(std::vector<std::string> &_required_params, std::vector<std::string> &_optional_params) {
	
	_required_params.push_back("nthreads");
	_required_params.push_back("listen_port");
}

Hashd::Hashd(const std::string &_config_file) {
	
	loadConfig(_config_file);
	startListening(strtoint(m_config["nthreads"]), strtoint(m_config["listen_port"]));
	
	m_core.reset(new HashCore(m_srv_tasklauncher));
	
	m_api.reset(new HashdApi(boost::bind(&HashCore::onCreateHash, m_core.get(), _1, _2, _3),
							boost::bind(&HashCore::onSetAndIncTtl, m_core.get(), _1, _2, _3, _4, _5),
							boost::bind(&HashCore::onSet, m_core.get(), _1, _2, _3, _4),
		 	//				boost::bind(&HashCore::onSetWithTtl, m_core.get(), _1, _2, _3, _4),
							boost::bind(&HashCore::onGet, m_core.get(), _1, _2, _3, _4),
			//				boost::bind(&HashCore::onGetWithTtl, m_core.get(), _1, _2, _3, _4),
							boost::bind(&HashCore::onDel, m_core.get(), _1, _2, _3),
							boost::bind(&HashCore::onGetTtl, m_core.get(), _1, _2, _3, _4)
			//				boost::bind(&HashCore::onSetHashNlruShots, m_core.get(), _1, _2),
			//				boost::bind(&HashCore::onSetHashDefaultTtl, m_core.get(), _1, _2)
			));
	
}

void Hashd::doStart() {
	
	std::cout << "hashd started on port " << m_config["listen_port"] << std::endl;
}

void Hashd::connHandler(HttpConnectionPtr _conn, HttpRequestPtr _req) {	
	
	m_api->handle(_conn, _req);
}
