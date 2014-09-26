#include "hashd.h"

void Hashd::setParamsList(std::vector<std::string> &_required_params, std::vector<std::string> &_optional_params) {
	
	_required_params.push_back("nthreads");
	_required_params.push_back("listen_port");
}

Hashd::Hashd(const std::string &_config_file) {
	
	m_last_dump_ts = time(0);
	loadConfig(_config_file);
	startListening(strtoint(m_config["nthreads"]), strtoint(m_config["listen_port"]));
	
	m_core.reset(new HashCore(m_srv_tasklauncher));
	
	std::vector<std::string> files;
	listFiles(".", files);
	
	std::string filename = "";
	// if bak
	if (isStringInVec("hashd_dump.bak", files))
		filename = "hashd_dump.bak";
	// if dump
	else if (isStringInVec("hashd_dump", files))
		filename = "hashd_dump";
	// else do not restore
	
	if (filename != "") {
		int fd = open(filename.c_str(), O_RDONLY);
		m_core->restore(fd);
		close(fd);
	}
	
	
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
	
	checkDump();
	m_api->handle(_conn, _req);
}

void Hashd::checkDump() {
	
	if (time(0) - m_last_dump_ts > 30) {

		m_last_dump_ts = time(0);
		if (fork() == 0) {
			
			dump();
			
		}
	}
}

void Hashd::dump() {
	
	int fd = open("hashd_dump.tmp",  O_RDWR | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	
	
	m_core->dump(fd);
	fsync(fd);
	close(fd);
	
	std::vector<std::string> files;
	listFiles(".", files);
	
	if (isStringInVec("hashd_dump", files)) {
		// dump -> bak
		rename("hashd_dump", "hashd_dump.bak");
		// tmp -> dump
		rename("hashd_dump.tmp", "hashd_dump");

	} else
		rename("hashd_dump.tmp", "hashd_dump");
	
	listFiles(".", files);
	
	if (isStringInVec("hashd_dump.bak", files))
		unlink("hashd_dump.bak");
	
	exit(0);
}
