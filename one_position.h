/*
 *
 *		one_position.h
 *
 *  Created by Andrey Derevyagin on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */

// if you added void* change "position_cpy" function
typedef struct position_t
{
	time_t timestamp;
	char *ra_s;
	char *decl_s;
	float ra;
	float decl;
	char *hour_ang;
	float az;
	float um;
	float phase;
	int range;
	float mag;
	int shadow;
	
	char *time_str;
	float __ra;
} position_t;

position_t *position_init();
void position_free(position_t *pos);

position_t* position_from_str(const char *str);
void position_dump(position_t* pos);
position_t* position_cpy(position_t *pos);
