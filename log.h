#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>

#define INF(fmt, ...) printf("[%s %s] [%10s:%04d] [INF] " fmt "\n", __DATE__, __TIME__, basename(__FILE__), __LINE__, ##__VA_ARGS__);
#define TRC(fmt, ...) printf("[%s %s] [%10s:%04d] [TRC] " fmt "\n", __DATE__, __TIME__, basename(__FILE__), __LINE__, ##__VA_ARGS__);
#define WRN(fmt, ...) printf("[%s %s] [%10s:%04d] [WRN] " fmt "\n", __DATE__, __TIME__, basename(__FILE__), __LINE__, ##__VA_ARGS__);
#define ERR(fmt, ...) printf("[%s %s] [%10s:%04d] [ERR] " fmt "\n", __DATE__, __TIME__, basename(__FILE__), __LINE__, ##__VA_ARGS__);
#define DIE(fmt, ...) printf("[%s %s] [%10s:%04d] [DIE] " fmt "\n", __DATE__, __TIME__, basename(__FILE__), __LINE__, ##__VA_ARGS__); exit(errno)
