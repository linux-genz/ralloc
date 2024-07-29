// C++ version of:
// https://github.com/parlab-tuwien/lockfree-linked-list.git
// using pptr

/* (C) Jesper Larsson Traff, May 2020 */
/* Improved lock-free linked list implementations */

#ifndef _LF_LINKED_LIST_
#define _LF_LINKED_LIST_

#include <cstddef>
#include "pptr.hpp"

// Revisit: finish this
class LFListNode {
public:
    atomic_pptr<LFListNode> next;
    atomic_pptr<LFListNode> prev;
    LFListNode *free;   // for freelist
    char padding[40];   // fill the (64-byte) cacheline
    long key;
};

class LFList {
public:
    LFList(LFListNode& head, LFListNode& tail);
    ~LFList();

private:
    LFListNode *head, *tail; // sentinels, possibly shared
    LFListNode *curr;        // private cursor (last operation)
    LFListNode *pred;        // predecessor of cursor
    LFListNode *free;        // private free list
#ifdef COUNTERS
    uint64_t adds, rems, cons, trav, fail, rtry;
#endif
};
#endif // _LF_LINKED_LIST_
