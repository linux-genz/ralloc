#ifndef _PM_CONFIG_HPP_
#define _PM_CONFIG_HPP_


#include <assert.h>

#include "pfence_util.h"

/* prefixing indicator */
// persistent data in pmmalloc
#define PM_PERSIST
// transient data in pmmalloc
#define PM_TRANSIENT

#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

#ifdef DEBUG
  #define DBG_PRINT(msg, ...) \
    fprintf(stderr, "%s:%d %s " msg "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    fflush(stderr);
#else
  #define DBG_PRINT(msg, ...)
#endif

/* user customized macros */
#define HEAPFILE_PREFIX "/dev/shm/"
#define ENABLE_FILTER_FUNC 1
// #define DEBUG 1
const uint64_t MAX_FILESIZE = 16*1024*1024*1024ULL;
const uint64_t MAX_THREADS = 512;
const int MAX_ROOTS = 1024;
const int MAX_SECTION = 10;
const uint64_t MAX_BLOCK_NUM = (2ULL << 31);//max number of blocks per sb
const int DESC_SPACE_CAP = 128;//number of desc sbs per desc space
const uint64_t SB_SPACE_SIZE = 1*1024*1024*1024ULL;
const int PAGESIZE = 4096;
const int SBSIZE = (16 * PAGESIZE); // size of a superblock
const int DESCSBSIZE = (1024 * 64);//assume sizeof(Descriptor) is 64
const int DESC_SPACE_SIZE = DESC_SPACE_CAP * DESCSBSIZE;
//Note: by this config, approximately 1 sb space needs 8 desc space

/* constant variables */
const int TYPE_SIZE = 4;
const int PTR_SIZE = sizeof(void*);
const int HEADER_SIZE = (TYPE_SIZE + PTR_SIZE);
const int CACHE_LINE_SIZE = 64;

const int LARGE = 249; // tag indicating the block is large
const int SMALL = 250; // tag indicating the block is small

const int ACTIVE = 0; // 4 status of a superblock
const int FULL = 1;
const int PARTIAL = 2;
const int EMPTY = 3;

const int MAXCREDITS = 64; // 2^(bits for credits in active)
const int GRANULARITY = 16; // granularity of sizeclass size in byte
const int MAX_SMALLSIZE = 2048; // largest size of a small object in byte
const int PROCHEAP_NUM = MAX_THREADS; // number of processor heap

const uint64_t FREELIST_CAP = 10*1024*1024ULL; //largest amount of nodes in freelist
const uint64_t FREESTACK_CAP = FREELIST_CAP;
const uint64_t FREEQUEUE_CAP = FREELIST_CAP;
const uint64_t PARTIAL_CAP = 10*1024ULL;//largest amount of nodes in partial list
#endif