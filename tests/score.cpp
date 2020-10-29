////
//// Created by neeraj on 4/19/20.
////

#include "../include/SCoRe.h"

int main(int argc, char **argv){
	server::SCoRe s = new server::SCoRe(int argc, char **argv);
	s.get_latest();
	return 0;
}
