//
// Created by neeraj on 4/26/20.
//

#include "daemonize.h"

template<typename T>
ArgHolder *Daemon<T>::arg = nullptr;

template<typename T>
std::string Daemon<T>::main_log_file = "SCoRe.log";

template<class T>
void Daemon<T>::signalHandler(int sig) {
	switch (sig) {
		case SIGHUP: {
			logMessage(main_log_file, "SIGHUP Caught");
			break;
		}
		case SIGTERM: {
			logMessage(main_log_file, "SIGTERM Caught");
			finalize();
			exit(0);
		}
		default: {
			void *array[20];
			size_t size;
			// get void*'s for all entries on the stack
			size = backtrace(array, 20);
			// print out all the frames to stderr
			fprintf(stderr, "ERROR: signal %d:\n", sig);
			backtrace_symbols_fd(array, size, STDERR_FILENO);
			::raise(SIGTERM);
		}
	}
}

template<class T>
void Daemon<T>::finalize() {
	Singleton<Daemon>::GetInstance()->instance_i->Stop();
	Singleton<Daemon>::GetInstance()->worker.join();
}

template<class T>
void Daemon<T>::logMessage(const std::string &filename, const std::string &message) {
	std::ofstream outfile;
	outfile.open(filename, std::ios_base::app); // append instead of overwrite
	outfile << message;
}
