#include "HashdClientAsync.h"

void onFinished() {
	
}

void onFinished(bool _success) {
	std::cout << "onFinished " << _success << std::endl;
}

void onGot(bool _succ, bool _exists, const std::string &_v) {
	
	std::cout << "onGot: " << _exists << " " << _v << std::endl;
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
	
		client->set("_hash_", "_key_", "_value_1488_", boost::bind(&onFinished, _1));
	
		sleep(1);
	
		client->get("_hash_", "_key_", boost::bind(&onGot, _1, _2, _3));
		
		pool->join();
	
	} catch (const char * _s) {
		std::cout << _s << std::endl;
	} catch (...) {
		std::cout << "unknown exception\n";
		exit(0);
	}
	
	return 0;
}
