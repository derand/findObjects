/*
 *
 *		searcher.h
 *
 *  Created by Andrey Derevyagin on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */
 
enum hit_e
{
	hit_done=0, 		// time for observe object
	hit_low_height=1,
	hit_low_mag=2,
	hit_fast=3,
	hit_in_shadow=4,
	hit_start_ephemerides=5,
	hit_end_ephemerides=6,
	hit_in_moon=7
};

struct position_t;

// one row event change hit
struct searcher_result_t
{
	enum hit_e hit;
	int in;
	struct position_t *pos;
};

// all file events chenge hit
struct searcher_file_result_t
{
	struct searcher_result_t *sr_vec;	// vector
	time_t hour_angle;					// hour angle at this event
	char *filename;						// file name without path
};


void searcher_main_loop(settings_t* sttngs);

