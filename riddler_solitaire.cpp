#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <iostream>
#include <boost/multiprecision/cpp_bin_float.hpp>
using namespace boost::multiprecision;

#include <stdio.h>

typedef boost::rational<cpp_int> t_score;
//typedef double t_score;

#define n_cards 10
#define n_card_masks (1 << n_cards)
#define all_card_mask (n_card_masks - 1)
#define card_total_value (n_cards * (n_cards + 1) / 2)

#define card_remaining_mask(m) (all_card_mask & ~m)

int pop_count (unsigned x)
{   unsigned p;
	int r;
	p = (x & 0x55555555) + ((x & 0xaaaaaaaa) >> 1);
	p = (p & 0x33333333) + ((p & 0xcccccccc) >> 2);
	p = (p & 0x0f0f0f0f) + ((p & 0xf0f0f0f0) >> 4);
	p = (p & 0x00ff00ff) + ((p & 0xff00ff00) >> 8);
	p = (p & 0x0000ffff) + ((p & 0xffff0000) >> 16);
	r = p;
	return r;
}

int mask_to_score (unsigned m)
{   int i;
	int s;
	s = 0;
	for (i = 0; i < n_cards; i++) {
		if (m & (1 << i)) {
			s += i + 1;
		}
	}
	return s;
}


bool score_algo (t_score s, int level)
{   if (((s + (card_total_value - s) / (n_cards - level)) * ((t_score) n_cards - (t_score) level) / ((t_score) n_cards + 1 - (t_score) level)) > s) {
		return true;
	} else {
		return false;
    }
}


int _tmain(int argc, _TCHAR* argv[]) 
{   t_score best_score [n_cards + 1] [n_card_masks];    /* score [i] [j] with i cards played and bitmask of cards is j */
	bool play_cont [n_cards + 1] [n_card_masks];
	int ilevel;
	int imask;
	int icard;
    int icard_next;
	int card_next_mask;
	t_score play_score;
	t_score this_score;

	for (ilevel = 0; ilevel < n_cards + 1; ilevel++) {
		for (imask = 0; imask < n_card_masks; imask++) {
			best_score [ilevel] [imask] = -9999999;
			play_cont [ilevel] [imask] = false;

		}
	}
	best_score [n_cards] [all_card_mask] = card_total_value;
	for (ilevel = n_cards - 1; ilevel >= 0; ilevel--) {
		for (imask = 0; imask < n_card_masks; imask++) {
			if (pop_count (imask) == ilevel) {
				card_next_mask = card_remaining_mask (imask);
				play_score = 0;
				this_score = mask_to_score (imask);
				printf ("lev %d mask %04x ", ilevel, imask);
				for (icard_next = 0; icard_next < n_cards; icard_next++) {
					if (card_next_mask & (1 << icard_next)) {
						printf ("try %d", icard_next + 1);
						play_score += best_score [ilevel + 1] [imask | (1 << icard_next)];
						std::cout << " " << best_score [ilevel + 1] [imask | (1 << icard_next)] << " ";
					}
				}
				play_score = play_score / (n_cards + 1 - ilevel);
				if (play_score > this_score) {
					best_score [ilevel] [imask] = play_score;
					play_cont [ilevel] [imask] = true;
				} else {
                    best_score [ilevel] [imask] = this_score;
                }
                std::cout << "this " << this_score << " play " << play_score;
				if (play_cont [ilevel] [imask] != score_algo (this_score, ilevel)) {
					printf (" bad");
				}
				printf ("\n");
			}
		}
	}

	return 0;
}
