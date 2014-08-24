#include "hashd.h"

int main (int argc, char **argv) {
	
	Hashd *h = new Hashd("hashd.conf");
	h->start();
}
