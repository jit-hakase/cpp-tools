#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <mutex>

inline void debug_print(const char *lvl, const char *file, int line, const char *fmt,...) {
	static std::mutex log_mtx;
	std::lock_guard<std::mutex> lg(log_mtx);
	
	static char filez[20];
	static char timez[20];
	
	const char *pfile_start = strrchr(file, '\\') ? (strrchr(file, '\\')+1) : file;
	char *pfile_end = const_cast<char*>(pfile_start);
	while (*pfile_end != '.') {
		++pfile_end;
	}
	
	memcpy(filez, pfile_start, pfile_end-pfile_start);
	auto ts = time(nullptr);
	struct tm* ptm = localtime(&ts);
	sprintf(timez, "%04d-%02d-%02d %02d:%02d:%02d", ptm->tm_year+1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	printf("[%19s][%4s][%4d][%s]:", timez, filez+strlen(filez)-4, line, lvl);
	
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

#ifdef DEBUG
	#define DBG(fmt) debug_print("DBG", __FILE__, __LINE__, fmt)
#else
	#define DBG(fmt)
#endif

#define INF(fmt) debug_print("INF", __FILE__, __LINE__, fmt)
#define TRC(fmt) debug_print("TRC", __FILE__, __LINE__, fmt)
#define WRN(fmt) debug_print("WRN", __FILE__, __LINE__, fmt)
#define ERR(fmt) debug_print("ERR", __FILE__, __LINE__, fmt)
#define DIE(fmt) debug_print("DIE", __FILE__, __LINE__, fmt); exit(errno)
