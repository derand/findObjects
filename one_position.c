/*
 *
 *		one_position.c
 *
 *  Created by Andrey Derevyagin on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "utils.h"
#include "one_position.h"
#include "fastvec.h"

position_t *position_init()
{
	position_t *rv = malloc(sizeof(position_t));
	if (rv)
	{
		rv->ra_s = 0;
		rv->decl_s = 0;
		rv->hour_ang = 0;
		rv->shadow = 0;
		rv->time_str = 0;
	}
	return rv;
}

void position_free(position_t *pos)
{
	if(pos)
	{
		sfree(pos->time_str);
		sfree(pos->hour_ang);
		sfree(pos->decl_s);
		sfree(pos->ra_s);
		free(pos);
	}
}

position_t* position_from_str(const char *str)
{
	position_t* rv = position_init();
	char* line = str_strip(str);
	
	int k=-2;
	char** as_array;
	vec_init((void**)&as_array, sizeof(char *), 0, 10);
	int len = strlen(line);
	char* tmp=line;
	for(int i=1; i<len; i++)
	{
		if (line[i]==' ' || line[i]=='\t' || line[i]=='\n')
		{
			line[i]='\0';
			vec_add((void **)&as_array, (void *)&tmp);
			while (++i<len &&(line[i]==' ' || line[i]=='\t' || line[i]=='\n')) ;
			tmp = &line[i];
		}
	}
	if (strlen(tmp))
	{
		vec_add((void **)&as_array, (void *)&tmp);
	}
	
	k=-1;
	if (vec_count(as_array)==18 || vec_count(as_array)==17)
	{
		int err = 0;
		rv->timestamp = _atodate(as_array[++k], &err);
		if (err) goto _err01;
		rv->timestamp += _atotime(as_array[++k], &err);
		if (err) goto _err01;
		as_array[k+1][strlen(as_array[k+1])]=' ';
		as_array[k+2][strlen(as_array[k+2])]=' ';
		rv->ra_s = strdup(as_array[k+1]);
		rv->__ra = _ra(rv->ra_s, &err);
		if (err) goto _err01;
		k+=3;
		as_array[k+1][strlen(as_array[k+1])]=' ';
		as_array[k+2][strlen(as_array[k+2])]=' ';
		rv->decl_s = strdup(as_array[k+1]);
		k+=3;
		if (vec_count(as_array)==17)
		{
//			TODO: test text on "<<SHADOW>>"
			rv->shadow=1;
			k++;
		}
		else
		{
			rv->ra = _atof(as_array[++k], &err);
			if (err) goto _err01;
			rv->decl = _atof(as_array[++k], &err);
			if (err) goto _err01;
		}
		as_array[k+1][strlen(as_array[k+1])]=' ';
		as_array[k+2][strlen(as_array[k+2])]=' ';
		rv->hour_ang = strdup(as_array[k+1]);
		rv->_hour_angle_val = _str2time(rv->hour_ang, &err, "%d %d %d");
		if (err) goto _err01;
		k+=3;
		rv->az = _atof(as_array[++k], &err);
		if (err) goto _err01;
		rv->um = _atof(as_array[++k], &err);
		if (err) goto _err01;
		rv->phase = _atof(as_array[++k], &err);
		if (err) goto _err01;
		rv->range = (int)_atof(as_array[++k], &err);
		if (err) goto _err01;
		rv->mag = _atof(as_array[++k], &err);
		if (err) goto _err01;
		
		rv->time_str = malloc(strlen(as_array[0])+strlen(as_array[1])+2);
		sprintf(rv->time_str, "%s %s", as_array[0], as_array[1]);

//		$log("%s %s", str, rv->time_str);
	}

	for(int i=0; i<vec_count(as_array); i++)
	{
//		$log("%d(%d)\t\"%s\"", i, vec_count(as_array), as_array[i]);
	}
	goto _ext01;
	
_err01:
	$log("Error in str\"%s\", can't parse position %d", str, k);
	position_free(rv);
	rv=0;

_ext01:
//	position_dump(rv);
	vec_free(as_array);	
	free(line);
	return rv;
}

void position_dump(position_t* pos)
{
	if (pos)
	{
		$log("(%d)time:%ld ra:(%s) decl:(%s) ra:%.2f decl:%.2f hour_ang:(%s) az:%.2f um:%.2f phase:%.2f range:%d mag:%.2f", pos->shadow, pos->timestamp, pos->ra_s, pos->decl_s, pos->ra, pos->decl, pos->hour_ang, pos->az, pos->um, pos->phase, pos->range, pos->mag);
	}
	else
	{
		$log("%s", "-none-");
	}
}

position_t* position_cpy(position_t *pos)
{
	position_t *rv = position_init();
	if (rv)
	{
		memcpy(rv, pos, sizeof(position_t));
		if (rv->ra_s)		rv->ra_s=strdup(pos->ra_s);
		if (rv->decl_s)		rv->decl_s=strdup(pos->decl_s);
		if (rv->hour_ang)	rv->hour_ang=strdup(pos->hour_ang);
		if (rv->time_str)	rv->time_str=strdup(pos->time_str);
	}
	return rv;
}


