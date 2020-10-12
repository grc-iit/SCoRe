
#pragma once
#ifndef SCORE_DAEMONIZE_H
#define SCORE_DAEMONIZE_H

#include <execinfo.h>
#include <future>
#include <thread>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <cstring>
#include <boost/stacktrace.hpp>
#include <fstream>
#include "singleton.h"
//#include "reverse_trie_queue.h"
//#include "SCoReServer.h"

#define RUNNING_DIR    "./tmp"

void finalize();

static void log_message(char *filename, char *message) {
	FILE *logfile;
	logfile = fopen(filename, "a");
	if (!logfile) return;
	fprintf(logfile, "%s\n", message);
	fclose(logfile);
}

const char main_log_file[256] = "SCoRe.log";

struct ArgHolder {
	int argc;
	char **argv;
};

template<class T>
class Daemon {
private:


	static ArgHolder *arg;

	void RunInternal() {
		instance_i = new T(arg->argc, arg->argv);
	}

public:
	static std::string main_log_file;
	std::thread worker;
	T *instance_i;

	Daemon() {}

	Daemon(int argc, char *argv[]) {
		arg = new ArgHolder();
		arg->argc = argc;
		arg->argv = argv;
		worker = std::thread(&Daemon::RunInternal, this);
		catchSignals();
		while (true) sleep(1);
	}

	~Daemon() {}

private:
	static void finalize();

	static void signalHandler(int sig);

	static void logMessage(const std::string &filename, const std::string &message);

	void catchSignals() {
		signal(SIGTERM, signalHandler); /* catch kill signal */
		signal(SIGHUP, signalHandler); /* catch hangup signal */
		signal(SIGABRT, signalHandler); /* catch hangup signal */
		signal(SIGSEGV, signalHandler); /* catch hangup signal */
		signal(SIGBUS, signalHandler); /* catch hangup signal */
		signal(SIGCHLD, SIG_IGN);              /* ignore child */
		signal(SIGTSTP, SIG_IGN);              /* ignore tty signals */
		signal(SIGTTOU, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
	}

};


#endif

#include "../src/daemonize.cpp"

