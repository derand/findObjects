/*
 *
 *		one_position.c
 *
 *  Created by Andrey Derevyagin on 12/08/13.
 *  Copyright 2013 Andrey Derevyagin. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "utils.h"
#include "orb_position.h"
#include "fastvec.h"

orb_position_t *orb_position_init()
{
	orb_position_t *rv = malloc(sizeof(orb_position_t));
	if (rv)
	{
		rv->name = 0;
		rv->vgi = 0;
	}
	return rv;
}

void orb_position_free(orb_position_t *pos)
{
	if (pos)
	{
		sfree(pos->name);
		free(pos);
	}
}

orb_position_t* orb_position_from_str(const char *str)
{
	if (strlen(str) < 1)
	{
		return 0;
	}
	orb_position_t *rv = orb_position_init();
	char *line = str_strip(str);
	
	char **as_array;
	vec_init((void **)&as_array, sizeof(char *), 0, 10);
	int len = strlen(line);
	char *tmp = line;
	for(int i=0; i<len; i++)
	{
		if (line[i]=='|' || line[i]==' ' || line[i]=='\t')
		{
			line[i]='\0';
			if (strlen(tmp) || vec_count(as_array))
			{
				vec_add((void **)&as_array, (void *)&tmp);
			}
			while (++i<len && (line[i]==' ' || line[i]=='\t' || line[i]=='|')) ;
			tmp = &line[i];
		}
	}
	if (strlen(tmp))
	{
		vec_add((void **)&as_array, (void *)&tmp);
	}

	int k = 0;
	int err = 0;
	if (vec_count(as_array) == 23)
	{
		rv->name = strdup(as_array[k]);
		k = 2;
		rv->vgi = _atoi(as_array[k], &err);
		if (err) goto _err01;
	}
	else
	{
		orb_position_free(rv);
		rv=0;
	}
	goto _ext01;
	
_err01:
	$log("Error in str\"%s\", can't parse position %d \"%s\"", str, k, as_array[k]);
	orb_position_free(rv);
	rv=0;

_ext01:
//	position_dump(rv);
	vec_free(as_array);	
	free(line);
	return rv;
}

void orb_position_dump(orb_position_t* pos)
{
	if (pos)
	{
		$log("name:%s vgi:%d", pos->name, pos->vgi);
	}
	else
	{
		$log("%s", "-none-");
	}
}

orb_position_t* orb_position_cpy(const orb_position_t *pos)
{
	orb_position_t *rv = orb_position_init();
	if (rv)
	{
		memcpy(rv, pos, sizeof(orb_position_t));
		if (rv->name) rv->name = strdup(pos->name);
	}
	return rv;
}


