#include <cstddef>
#include <climits>
#include "LFLinkedList.hpp"

LFList::LFList(LFListNode& head, LFListNode& tail)
{
    this->head = &head;
    this->tail = &tail;

    // the sentinels
    this->head->key = LONG_MIN;
    this->head->next = &tail;
    this->head->prev = NULL;

    this->tail->key = LONG_MAX;
    this->tail->next = NULL;
    this->tail->prev = &head;

    this->pred = &head;
    this->curr = NULL;

    this->free = NULL;

#ifdef COUNTERS
    this->adds = 0;
    this->rems = 0;
    this->cons = 0;
    this->trav = 0;
    this->fail = 0;
    this->rtry = 0;
#endif
}
