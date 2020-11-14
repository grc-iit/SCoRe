
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

static void daemonize(const char *pid_file_name){
    pid_t pid = 0;
    int fd;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Success: Let the parent terminate */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* On success: The child process becomes session leader */
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    /* Ignore signal sent from child to parent process */
    signal(SIGCHLD, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Success: Let the parent terminate */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
        close(fd);
    }

    /* Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2) */
    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");

    /* Try to write PID of daemon to lockfile */
    if (pid_file_name != nullptr)
    {
        char str[256];
        int pid_fd;
        pid_fd = open(pid_file_name, O_RDWR|O_CREAT, 0640);
        if (pid_fd < 0) {
            /* Can't open lockfile */
            exit(EXIT_FAILURE);
        }
//        if (lockf(pid_fd, F_TLOCK, 0) < 0) {
//            /* Can't lock file */
//            exit(EXIT_FAILURE);
//        }
        /* Get current PID */
        sprintf(str, "%d\n", getpid());
        /* Write PID to lockfile */
        write(pid_fd, str, strlen(str));
    }
}

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

	Daemon(int argc, char *argv[], bool fork=false, std::string char_file="/tmp/daemon.pid") {
		arg = new ArgHolder();
		arg->argc = argc;
		arg->argv = argv;
        pid_t pid = 0;
        if(fork){
            daemonize(char_file.c_str());
            catchSignals();
            Daemon::RunInternal();
            exit(0);
        }
        else {
            worker = std::thread(&Daemon::RunInternal, this);
            catchSignals();
            while (true) sleep(1);
        }
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

