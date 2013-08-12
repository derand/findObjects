/*
 *
 *		one_position.h
 *
 *  Created by Andrey Derevyagin on 12/08/13.
 *  Copyright 2013 Andrey Derevyagin. All rights reserved.
 *
 */

// if you added void* change "orb_position_cpy" function
typedef struct orb_position_t
{
	char *name;
	int vgi;
} orb_position_t;

orb_position_t *orb_position_init();
void orb_position_free(orb_position_t *pos);

orb_position_t* orb_position_from_str(const char *str);
void orb_position_dump(orb_position_t* pos);
orb_position_t* orb_position_cpy(const orb_position_t *pos);
