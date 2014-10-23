#include "hiconfig.h"
#include "hashd.h"

#include "signal.h"
#include <sys/types.h>
#include <sys/wait.h>

void signal_callback_handler(int signum) {
	
	int status;
	wait(&status);
}

void empty_handler(int signal) {
	
	//std::cout << "empty handler\n";
}

int main (int argc, char **argv) {

	signal(SIGCHLD, signal_callback_handler);
	signal(SIGPIPE, empty_handler);
	
	Hashd *h = new Hashd("hashd.conf");
	h->start();
}
