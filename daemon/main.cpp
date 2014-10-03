#include "hiconfig.h"
#include "hashd.h"

void signal_callback_handler(int signum) {
	
	int status;
	wait(&status);
}

int main (int argc, char **argv) {

	signal(SIGCHLD, signal_callback_handler);
	
	Hashd *h = new Hashd("hashd.conf");
	h->start();
}
