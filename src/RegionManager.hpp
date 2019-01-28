/*
 * (c) Copyright 2016 Hewlett Packard Enterprise Development LP
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details. You should have received a copy of the GNU Lesser
 * General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */

/* 
 * This is the C++ version of region_manager code from Makalu
 * https://github.com/HewlettPackard/Atlas/tree/makalu/makalu_alloc
 */

#ifndef _REGION_MANAGER_HPP_
#define _REGION_MANAGER_HPP_

#include <string>
#include <fstream>
#include <atomic>

#include "pm_config.hpp"

using namespace std;

class RegionManager{
	const uint64_t FILESIZE;
	const char * HEAPFILE;
public:
	int FD = 0;
	char *base_addr = nullptr;
	atomic<char *>* curr_addr_ptr;//this always points to the place of base_addr+1

	RegionManager(const string& file_path):
		FILESIZE(MAX_FILESIZE),
		HEAPFILE(file_path.c_str()),
		curr_addr_ptr(nullptr){
		if(exists_test(HEAPFILE)){
			__remap_persistent_region();
		} else {
			__map_persistent_region();
		}
	};
	~RegionManager(){
		__close_persistent_region();
	}
	//mmap anynomous, not used by default
	// void __map_transient_region();

	inline static bool exists_test (const string& name){
		ifstream f(name.c_str());
		return f.good();
	}

	//mmap file
	void __map_persistent_region();
	void __remap_persistent_region();

	//persist the curr and base address
	void __close_persistent_region();

	//print the status
	// void __close_transient_region();

	//store heap root
	void __store_heap_start(void*);

	//retrieve heap root
	void* __fetch_heap_start();

	int __nvm_region_allocator(void** /*ret */, size_t /* alignment */, size_t /*size */);
};


#endif /* _REGION_MANAGER_HPP_ */