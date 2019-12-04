/*

Copyright 2019 University of Rochester

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. 

*/ 

/*

MIT License

Copyright (c) 2018 Ricardo Leite

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef __TCACHE_H_
#define __TCACHE_H_

#include "pm_config.hpp"
#include "pfence_util.h"
#include "SizeClass.hpp"
#include "pptr.hpp"
#include "RegionManager.hpp"

/*
 * This is from LRMALLOC:
 * https://github.com/ricleite/lrmalloc
 * 
 * This defines thread-local cache, using C++ TLS.
 * During normal exit, all cached blocks will be given back to superblocks.
 * 
 * The head (_block) of each cache list uses absolute address while
 * the list itself is linked by pptr since block free list is linked by pptr.
 *
 * In the destructor of TCacheBin, all blocks will be flushed back to their 
 * superblock as long as ralloc::initialized is true.
 * 
 * Wentao Cai (wcai6@cs.rochester.edu)
 */

struct TCaches;

struct TCacheBin
{
private:
	char* _block;//absolute address of block
	uint32_t _block_num;

public:
	// common, fast ops
	void push_block(char* block);
	// push block list, cache *must* be empty
	void push_list(char* block, uint32_t length);

	char* pop_block(); // can return nullptr
	// manually popped list of blocks and now need to update cache
	// `block` is the new head
	void pop_list(char* block, uint32_t length);
	char* peek_block() const { return _block; }

	uint32_t get_block_num() const { return _block_num; }
	TCacheBin() noexcept:_block(nullptr), _block_num(0) {};
	// slow operations like fill/flush handled in cache user
};

namespace ralloc{
	extern void public_flush_cache();
}
struct TCaches
{
	TCacheBin t_cache[MAX_SZ_IDX];
	TCaches():t_cache(){};
	~TCaches(){
		ralloc::public_flush_cache();
	}
};

/* thread-local cache */
namespace ralloc{
	extern thread_local TCaches t_caches;
}
#endif // __TCACHE_H_

