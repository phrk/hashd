#include "hiconfig.h"
#include "hashd.h"

#include "signal.h"
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char **argv) {

	
	Hashd *h = new Hashd("hashd.conf");
	h->start(false);
}
