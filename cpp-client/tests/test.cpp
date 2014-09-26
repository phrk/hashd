#include "HashdClientAsync.h"

void onFinished() {
	
}

void onFinished(int _err) {
	std::cout << "onFinished " << _err << std::endl;
}

void onGot(int _err, const std::string &_v) {
	
	std::cout << "onGot: " << _err << " " << _v << std::endl;
}

void onGotTtl(int _err, uint64_t _ttl) {
	
	std::cout << "onGotTtl: " << _err << " " << _ttl << std::endl;
}

int main (int argv, char **argc) {
	
	try {
	
		const std::string apiurl = "http://localhost:49012/";
	
		size_t max_parallel = 8;
	
		hThreadPoolPtr pool(new hThreadPool(max_parallel));
	
		pool->run();
	
		TaskLauncherPtr launcher(new TaskLauncher(pool,
					max_parallel, 
					boost::bind(&onFinished)));
		
		HttpOutRequestDispPtr req_disp(new HttpOutRequestDisp(launcher));
		HashdClientAsyncPtr client (new HashdClientAsync(apiurl, req_disp));
	
		std::map<std::string, std::string> opts;
		opts["type"] = "ttl";
		opts["default_ttl"] = "0";
		opts["cleanup_period"] = "10";
		opts["cleanup_check_size"] = "1000";
		
//		client->createHash("_hash_", opts, boost::bind(&onFinished, _1));	
//		sleep(1);
	
//		client->set("_hash_", "_key_", "_value_1488_", boost::bind(&onFinished, _1));
	
//		client->setAndIncTtl("_hash_", "_dead_key_", "_value_1488_", 30, boost::bind(&onFinished, _1));
//	
//		sleep(1);
	
		client->get("_hash_", "_key_", boost::bind(&onGot, _1, _2));
		
		sleep(1);
		
//		client->getTtl("_hash_", "_key_", boost::bind(&onGotTtl, _1, _2));
		
//		sleep(1);
		
//		client->setAndIncTtl("_hash_", "_key_", "_value_1488_2_", 666, boost::bind(&onFinished, _1));
		
//		sleep(1);
		
//		client->getTtl("_hash_", "_dead_key_", boost::bind(&onGotTtl, _1, _2));
		
		sleep(1);
		
		//client->del("_hash_", "_key_", boost::bind(&onFinished, _1));
		
		pool->join();
	
	} catch (const char * _s) {
		std::cout << _s << std::endl;
	} catch (...) {
		std::cout << "unknown exception\n";
		exit(0);
	}
	
	return 0;
}
