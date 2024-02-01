/*
 * Copyright (C) 2019 University of Rochester. All rights reserved.
 * Licenced under the MIT licence. See LICENSE file in the project root for
 * details. 
 */

/*
 * Copyright (C) 2018 Ricardo Leite
 * Licenced under the MIT licence. This file shares some portion from
 * LRMalloc(https://github.com/ricleite/lrmalloc) and its copyright 
 * is retained. See LICENSE for details about MIT License.
 */

#ifndef __SIZE_CLASSES_H
#define __SIZE_CLASSES_H

#include <cstddef>

#include "pm_config.hpp"

/*
 * This along with Sizeclass.cpp is from LRMALLOC:
 * https://github.com/ricleite/lrmalloc
 * 
 * This defines size classes with (kinda) constant parameters.
 * In other words, this data never gets changed in any run.
 * As a result, we don't persist it.
 *
 * The interface is in the class SizeClass, including constructor
 * and get_sizeclass. To use, just instantiate SizeClass and call 
 * get_sizeclass(size). SizeClass is safe to have multiple instances.
 *
 * Wentao Cai (wcai6@cs.rochester.edu)
 */

// contains size classes
struct SizeClassData
{
public:
	// size of block
	uint32_t block_size;
	// superblock size
	// always a multiple of page size
	uint32_t sb_size;
	// cached number of blocks, equal to sb_size / block_size
	uint32_t block_num;
	// number of blocks held by thread-specific caches
	uint32_t cache_block_num;

public:
	size_t get_block_num() const { return block_num; }
};

class SizeClass{
private:
	SizeClassData sizeclasses[MAX_SZ_IDX];
	uint8_t sizeclass_lookup[MAX_SZ + 1];
public:
	SizeClass();
	inline size_t get_sizeclass(size_t size){return sizeclass_lookup[size];}
	inline SizeClassData* get_sizeclass_by_idx(size_t idx){return &sizeclasses[idx];}
};
namespace ralloc{
	extern SizeClass sizeclass;
}

// size class data, from jemalloc 5.0
// block_size = 1<<lg_grp + ndelta<<lg_delta
// size classes 2, 4, and 6 lead to non-16-byte alignment, so are excluded
#define SIZE_CLASSES \
  /* index, lg_grp, lg_delta, ndelta, psz, bin, pgs, lg_delta_lookup, no_cnt */ \
	SC(  0,	  3,		3,	  0,  no, yes,   1,  3, 0) \
	SC(  1,	  3,		3,	  1,  no, yes,   1,  3, 0) \
	SC(  2,	  3,		3,	  2,  no,  no,   3,  3, 1) \
	SC(  3,	  3,		3,	  3,  no, yes,   1,  3, 1) \
														 \
	SC(  4,	  5,		3,	  1,  no,  no,   5,  3, 2) \
	SC(  5,	  5,		3,	  2,  no, yes,   3,  3, 2) \
	SC(  6,	  5,		3,	  3,  no,  no,   7,  3, 3) \
	SC(  7,	  5,		3,	  4,  no, yes,   1,  3, 3) \
														 \
	SC(  8,	  6,		4,	  1,  no, yes,   5,  4, 3) \
	SC(  9,	  6,		4,	  2,  no, yes,   3,  4, 3) \
	SC( 10,	  6,		4,	  3,  no, yes,   7,  4, 3) \
	SC( 11,	  6,		4,	  4,  no, yes,   1,  4, 3) \
														 \
	SC( 12,	  7,		5,	  1,  no, yes,   5,  5, 3) \
	SC( 13,	  7,		5,	  2,  no, yes,   3,  5, 3) \
	SC( 14,	  7,		5,	  3,  no, yes,   7,  5, 3) \
	SC( 15,	  7,		5,	  4,  no, yes,   1,  5, 3) \
														 \
	SC( 16,	  8,		6,	  1,  no, yes,   5,  6, 3) \
	SC( 17,	  8,		6,	  2,  no, yes,   3,  6, 3) \
	SC( 18,	  8,		6,	  3,  no, yes,   7,  6, 3) \
	SC( 19,	  8,		6,	  4,  no, yes,   1,  6, 3) \
														 \
	SC( 20,	  9,		7,	  1,  no, yes,   5,  7, 3) \
	SC( 21,	  9,		7,	  2,  no, yes,   3,  7, 3) \
	SC( 22,	  9,		7,	  3,  no, yes,   7,  7, 3) \
	SC( 23,	  9,		7,	  4,  no, yes,   1,  7, 3) \
														 \
	SC( 24,	 10,		8,	  1,  no, yes,   5,  8, 3) \
	SC( 25,	 10,		8,	  2,  no, yes,   3,  8, 3) \
	SC( 26,	 10,		8,	  3,  no, yes,   7,  8, 3) \
	SC( 27,	 10,		8,	  4,  no, yes,   1,  8, 3) \
														 \
	SC( 28,	 11,		9,	  1,  no, yes,   5,  9, 3) \
	SC( 29,	 11,		9,	  2,  no, yes,   3,  9, 3) \
	SC( 30,	 11,		9,	  3,  no, yes,   7,  9, 3) \
	SC( 31,	 11,		9,	  4, yes, yes,   1,  9, 3) \
														 \
	SC( 32,	 12,	   10,	  1,  no, yes,   5, no, 3) \
	SC( 33,	 12,	   10,	  2,  no, yes,   3, no, 3) \
	SC( 34,	 12,	   10,	  3,  no, yes,   7, no, 3) \
	SC( 35,	 12,	   10,	  4, yes, yes,   2, no, 3) \
														 \
	SC( 36,	 13,	   11,	  1,  no, yes,   5, no, 3) \
	SC( 37,	 13,	   11,	  2, yes, yes,   3, no, 3) \
	SC( 38,	 13,	   11,	  3,  no, yes,   7, no, 3) \
	SC( 39,	 13,	   11,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 40,	 14,	   12,	  1, yes,  no,   0, no, 3) \
	SC( 41,	 14,	   12,	  2, yes,  no,   0, no, 3) \
	SC( 42,	 14,	   12,	  3, yes,  no,   0, no, 3) \
	SC( 43,	 14,	   12,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 44,	 15,	   13,	  1, yes,  no,   0, no, 3) \
	SC( 45,	 15,	   13,	  2, yes,  no,   0, no, 3) \
	SC( 46,	 15,	   13,	  3, yes,  no,   0, no, 3) \
	SC( 47,	 15,	   13,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 48,	 16,	   14,	  1, yes,  no,   0, no, 3) \
	SC( 49,	 16,	   14,	  2, yes,  no,   0, no, 3) \
	SC( 50,	 16,	   14,	  3, yes,  no,   0, no, 3) \
	SC( 51,	 16,	   14,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 52,	 17,	   15,	  1, yes,  no,   0, no, 3) \
	SC( 53,	 17,	   15,	  2, yes,  no,   0, no, 3) \
	SC( 54,	 17,	   15,	  3, yes,  no,   0, no, 3) \
	SC( 55,	 17,	   15,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 56,	 18,	   16,	  1, yes,  no,   0, no, 3) \
	SC( 57,	 18,	   16,	  2, yes,  no,   0, no, 3) \
	SC( 58,	 18,	   16,	  3, yes,  no,   0, no, 3) \
	SC( 59,	 18,	   16,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 60,	 19,	   17,	  1, yes,  no,   0, no, 3) \
	SC( 61,	 19,	   17,	  2, yes,  no,   0, no, 3) \
	SC( 62,	 19,	   17,	  3, yes,  no,   0, no, 3) \
	SC( 63,	 19,	   17,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 64,	 20,	   18,	  1, yes,  no,   0, no, 3) \
	SC( 65,	 20,	   18,	  2, yes,  no,   0, no, 3) \
	SC( 66,	 20,	   18,	  3, yes,  no,   0, no, 3) \
	SC( 67,	 20,	   18,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 68,	 21,	   19,	  1, yes,  no,   0, no, 3) \
	SC( 69,	 21,	   19,	  2, yes,  no,   0, no, 3) \
	SC( 70,	 21,	   19,	  3, yes,  no,   0, no, 3) \
	SC( 71,	 21,	   19,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 72,	 22,	   20,	  1, yes,  no,   0, no, 3) \
	SC( 73,	 22,	   20,	  2, yes,  no,   0, no, 3) \
	SC( 74,	 22,	   20,	  3, yes,  no,   0, no, 3) \
	SC( 75,	 22,	   20,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 76,	 23,	   21,	  1, yes,  no,   0, no, 3) \
	SC( 77,	 23,	   21,	  2, yes,  no,   0, no, 3) \
	SC( 78,	 23,	   21,	  3, yes,  no,   0, no, 3) \
	SC( 79,	 23,	   21,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 80,	 24,	   22,	  1, yes,  no,   0, no, 3) \
	SC( 81,	 24,	   22,	  2, yes,  no,   0, no, 3) \
	SC( 82,	 24,	   22,	  3, yes,  no,   0, no, 3) \
	SC( 83,	 24,	   22,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 84,	 25,	   23,	  1, yes,  no,   0, no, 3) \
	SC( 85,	 25,	   23,	  2, yes,  no,   0, no, 3) \
	SC( 86,	 25,	   23,	  3, yes,  no,   0, no, 3) \
	SC( 87,	 25,	   23,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 88,	 26,	   24,	  1, yes,  no,   0, no, 3) \
	SC( 89,	 26,	   24,	  2, yes,  no,   0, no, 3) \
	SC( 90,	 26,	   24,	  3, yes,  no,   0, no, 3) \
	SC( 91,	 26,	   24,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 92,	 27,	   25,	  1, yes,  no,   0, no, 3) \
	SC( 93,	 27,	   25,	  2, yes,  no,   0, no, 3) \
	SC( 94,	 27,	   25,	  3, yes,  no,   0, no, 3) \
	SC( 95,	 27,	   25,	  4, yes,  no,   0, no, 3) \
														 \
	SC( 96,	 28,	   26,	  1, yes,  no,   0, no, 3) \
	SC( 97,	 28,	   26,	  2, yes,  no,   0, no, 3) \
	SC( 98,	 28,	   26,	  3, yes,  no,   0, no, 3) \
	SC( 99,	 28,	   26,	  4, yes,  no,   0, no, 3) \
														 \
	SC(100,	 29,	   27,	  1, yes,  no,   0, no, 3) \
	SC(101,	 29,	   27,	  2, yes,  no,   0, no, 3) \
	SC(102,	 29,	   27,	  3, yes,  no,   0, no, 3) \
	SC(103,	 29,	   27,	  4, yes,  no,   0, no, 3) \
														 \
	SC(104,	 30,	   28,	  1, yes,  no,   0, no, 3) \
	SC(105,	 30,	   28,	  2, yes,  no,   0, no, 3) \
	SC(106,	 30,	   28,	  3, yes,  no,   0, no, 3) \
	SC(107,	 30,	   28,	  4, yes,  no,   0, no, 3) \
														 \
	SC(108,	 31,	   29,	  1, yes,  no,   0, no, 3) \
	SC(109,	 31,	   29,	  2, yes,  no,   0, no, 3) \
	SC(110,	 31,	   29,	  3, yes,  no,   0, no, 3) \
	SC(111,	 31,	   29,	  4, yes,  no,   0, no, 3) \
														 \
	SC(112,	 32,	   30,	  1, yes,  no,   0, no, 3) \
	SC(113,	 32,	   30,	  2, yes,  no,   0, no, 3) \
	SC(114,	 32,	   30,	  3, yes,  no,   0, no, 3) \
	SC(115,	 32,	   30,	  4, yes,  no,   0, no, 3) \
														 \
	SC(116,	 33,	   31,	  1, yes,  no,   0, no, 3) \
	SC(117,	 33,	   31,	  2, yes,  no,   0, no, 3) \
	SC(118,	 33,	   31,	  3, yes,  no,   0, no, 3) \
	SC(119,	 33,	   31,	  4, yes,  no,   0, no, 3) \
														 \
	SC(120,	 34,	   32,	  1, yes,  no,   0, no, 3) \
	SC(121,	 34,	   32,	  2, yes,  no,   0, no, 3) \
	SC(122,	 34,	   32,	  3, yes,  no,   0, no, 3) \
	SC(123,	 34,	   32,	  4, yes,  no,   0, no, 3) \
														 \
	SC(124,	 35,	   33,	  1, yes,  no,   0, no, 3) \
	SC(125,	 35,	   33,	  2, yes,  no,   0, no, 3) \
	SC(126,	 35,	   33,	  3, yes,  no,   0, no, 3) \
	SC(127,	 35,	   33,	  4, yes,  no,   0, no, 3) \
														 \
	SC(128,	 36,	   34,	  1, yes,  no,   0, no, 3) \
	SC(129,	 36,	   34,	  2, yes,  no,   0, no, 3) \
	SC(130,	 36,	   34,	  3, yes,  no,   0, no, 3) \
	SC(131,	 36,	   34,	  4, yes,  no,   0, no, 3) \
														 \
	SC(132,	 37,	   35,	  1, yes,  no,   0, no, 3) \
	SC(133,	 37,	   35,	  2, yes,  no,   0, no, 3) \
	SC(134,	 37,	   35,	  3, yes,  no,   0, no, 3) \
	SC(135,	 37,	   35,	  4, yes,  no,   0, no, 3) \
														 \
	SC(136,	 38,	   36,	  1, yes,  no,   0, no, 3) \
	SC(137,	 38,	   36,	  2, yes,  no,   0, no, 3) \
	SC(138,	 38,	   36,	  3, yes,  no,   0, no, 3) \
	SC(139,	 38,	   36,	  4, yes,  no,   0, no, 3) \
														 \
	SC(140,	 39,	   37,	  1, yes,  no,   0, no, 3) \
	SC(141,	 39,	   37,	  2, yes,  no,   0, no, 3) \
	SC(142,	 39,	   37,	  3, yes,  no,   0, no, 3) \
	SC(143,	 39,	   37,	  4, yes,  no,   0, no, 3) \
														 \
	SC(144,	 40,	   38,	  1, yes,  no,   0, no, 3) \
	SC(145,	 40,	   38,	  2, yes,  no,   0, no, 3) \
	SC(146,	 40,	   38,	  3, yes,  no,   0, no, 3) \
	SC(147,	 40,	   38,	  4, yes,  no,   0, no, 3) \
														 \
	SC(148,	 41,	   39,	  1, yes,  no,   0, no, 3) \
	SC(149,	 41,	   39,	  2, yes,  no,   0, no, 3) \
	SC(150,	 41,	   39,	  3, yes,  no,   0, no, 3) \
	SC(151,	 41,	   39,	  4, yes,  no,   0, no, 3) \
														 \
	SC(152,	 42,	   40,	  1, yes,  no,   0, no, 3) \
	SC(153,	 42,	   40,	  2, yes,  no,   0, no, 3) \
	SC(154,	 42,	   40,	  3, yes,  no,   0, no, 3) \
	SC(155,	 42,	   40,	  4, yes,  no,   0, no, 3) \
														 \
	SC(156,	 43,	   41,	  1, yes,  no,   0, no, 3) \
	SC(157,	 43,	   41,	  2, yes,  no,   0, no, 3) \
	SC(158,	 43,	   41,	  3, yes,  no,   0, no, 3) \
	SC(159,	 43,	   41,	  4, yes,  no,   0, no, 3) \
														 \
	SC(160,	 44,	   42,	  1, yes,  no,   0, no, 3) \
	SC(161,	 44,	   42,	  2, yes,  no,   0, no, 3) \
	SC(162,	 44,	   42,	  3, yes,  no,   0, no, 3) \
	SC(163,	 44,	   42,	  4, yes,  no,   0, no, 3) \
														 \
	SC(164,	 45,	   43,	  1, yes,  no,   0, no, 3) \
	SC(165,	 45,	   43,	  2, yes,  no,   0, no, 3) \
	SC(166,	 45,	   43,	  3, yes,  no,   0, no, 3) \
	SC(167,	 45,	   43,	  4, yes,  no,   0, no, 3) \
														 \
	SC(168,	 46,	   44,	  1, yes,  no,   0, no, 3) \
	SC(169,	 46,	   44,	  2, yes,  no,   0, no, 3) \
	SC(170,	 46,	   44,	  3, yes,  no,   0, no, 3) \
	SC(171,	 46,	   44,	  4, yes,  no,   0, no, 3) \
														 \
	SC(172,	 47,	   45,	  1, yes,  no,   0, no, 3) \
	SC(173,	 47,	   45,	  2, yes,  no,   0, no, 3) \
	SC(174,	 47,	   45,	  3, yes,  no,   0, no, 3) \
	SC(175,	 47,	   45,	  4, yes,  no,   0, no, 3) \
														 \
	SC(176,	 48,	   46,	  1, yes,  no,   0, no, 3) \
	SC(177,	 48,	   46,	  2, yes,  no,   0, no, 3) \
	SC(178,	 48,	   46,	  3, yes,  no,   0, no, 3) \
	SC(179,	 48,	   46,	  4, yes,  no,   0, no, 3) \
														 \
	SC(180,	 49,	   47,	  1, yes,  no,   0, no, 3) \
	SC(181,	 49,	   47,	  2, yes,  no,   0, no, 3) \
	SC(182,	 49,	   47,	  3, yes,  no,   0, no, 3) \
	SC(183,	 49,	   47,	  4, yes,  no,   0, no, 3) \
														 \
	SC(184,	 50,	   48,	  1, yes,  no,   0, no, 3) \
	SC(185,	 50,	   48,	  2, yes,  no,   0, no, 3) \
	SC(186,	 50,	   48,	  3, yes,  no,   0, no, 3) \
	SC(187,	 50,	   48,	  4, yes,  no,   0, no, 3) \
														 \
	SC(188,	 51,	   49,	  1, yes,  no,   0, no, 3) \
	SC(189,	 51,	   49,	  2, yes,  no,   0, no, 3) \
	SC(190,	 51,	   49,	  3, yes,  no,   0, no, 3) \
	SC(191,	 51,	   49,	  4, yes,  no,   0, no, 3) \
														 \
	SC(192,	 52,	   50,	  1, yes,  no,   0, no, 3) \
	SC(193,	 52,	   50,	  2, yes,  no,   0, no, 3) \
	SC(194,	 52,	   50,	  3, yes,  no,   0, no, 3) \
	SC(195,	 52,	   50,	  4, yes,  no,   0, no, 3) \
														 \
	SC(196,	 53,	   51,	  1, yes,  no,   0, no, 3) \
	SC(197,	 53,	   51,	  2, yes,  no,   0, no, 3) \
	SC(198,	 53,	   51,	  3, yes,  no,   0, no, 3) \
	SC(199,	 53,	   51,	  4, yes,  no,   0, no, 3) \
														 \
	SC(200,	 54,	   52,	  1, yes,  no,   0, no, 3) \
	SC(201,	 54,	   52,	  2, yes,  no,   0, no, 3) \
	SC(202,	 54,	   52,	  3, yes,  no,   0, no, 3) \
	SC(203,	 54,	   52,	  4, yes,  no,   0, no, 3) \
														 \
	SC(204,	 55,	   53,	  1, yes,  no,   0, no, 3) \
	SC(205,	 55,	   53,	  2, yes,  no,   0, no, 3) \
	SC(206,	 55,	   53,	  3, yes,  no,   0, no, 3) \
	SC(207,	 55,	   53,	  4, yes,  no,   0, no, 3) \
														 \
	SC(208,	 56,	   54,	  1, yes,  no,   0, no, 3) \
	SC(209,	 56,	   54,	  2, yes,  no,   0, no, 3) \
	SC(210,	 56,	   54,	  3, yes,  no,   0, no, 3) \
	SC(211,	 56,	   54,	  4, yes,  no,   0, no, 3) \
														 \
	SC(212,	 57,	   55,	  1, yes,  no,   0, no, 3) \
	SC(213,	 57,	   55,	  2, yes,  no,   0, no, 3) \
	SC(214,	 57,	   55,	  3, yes,  no,   0, no, 3) \
	SC(215,	 57,	   55,	  4, yes,  no,   0, no, 3) \
														 \
	SC(216,	 58,	   56,	  1, yes,  no,   0, no, 3) \
	SC(217,	 58,	   56,	  2, yes,  no,   0, no, 3) \
	SC(218,	 58,	   56,	  3, yes,  no,   0, no, 3) \
	SC(219,	 58,	   56,	  4, yes,  no,   0, no, 3) \
														 \
	SC(220,	 59,	   57,	  1, yes,  no,   0, no, 3) \
	SC(221,	 59,	   57,	  2, yes,  no,   0, no, 3) \
	SC(222,	 59,	   57,	  3, yes,  no,   0, no, 3) \
	SC(223,	 59,	   57,	  4, yes,  no,   0, no, 3) \
														 \
	SC(224,	 60,	   58,	  1, yes,  no,   0, no, 3) \
	SC(225,	 60,	   58,	  2, yes,  no,   0, no, 3) \
	SC(226,	 60,	   58,	  3, yes,  no,   0, no, 3) \
	SC(227,	 60,	   58,	  4, yes,  no,   0, no, 3) \
														 \
	SC(228,	 61,	   59,	  1, yes,  no,   0, no, 3) \
	SC(229,	 61,	   59,	  2, yes,  no,   0, no, 3) \
	SC(230,	 61,	   59,	  3, yes,  no,   0, no, 3) \
	SC(231,	 61,	   59,	  4, yes,  no,   0, no, 3) \
														 \
	SC(232,	 62,	   60,	  1, yes,  no,   0, no, 3) \
	SC(233,	 62,	   60,	  2, yes,  no,   0, no, 3) \
	SC(234,	 62,	   60,	  3, yes,  no,   0, no, 3)

#endif // __SIZE_CLASSES_H
