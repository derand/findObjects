/*
 *
 *		utils.c
 *
 *  Created by Andrey Derevyagin on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "utils.h"

void upper_case(char *str)
{
	for (int i=0; i<strlen(str); i++)
	{
		str[i] = toupper(str[i]);
	}
}

char* str_strip(const char* str)
{
	int f=0;
	int e=strlen(str)-1;
	while (str[f]== ' ' || str[f]== '\t' || str[f]== '\n' || str[f]== '+' || str[f]== 0x0D || str[f]== 0x0A)
		if (++f>=e)
			break;
	while (str[e]==' ' || str[e]=='\t' || str[e]=='\n' || str[e]== 0x0D || str[e]== 0x0A)
		if (f>--e)
			break;
	if (f>e)
		return 0;
	e++;
	char* rv = malloc(sizeof(char)*(e-f+1));
	memcpy(rv, &str[f], e-f);
	rv[e-f]='\0';
	return rv;
}

void read_line(FILE* f, char *buff)
{
	char ch;
	int i=0;
	while( 1 )
	{
		ch = fgetc(f);
		if (ch=='\n' || feof(f))
			break;
		buff[i++] = ch;
	}
	buff[i]='\0';
}

void sfree(void *var)
{
	if (var)
		free(var);
}

float _atof(const char* str, int* err)
{
	char* tmp = str_strip(str);
	for (int i=0; i<strlen(tmp); i++)
	{
		if ((tmp[i]<'0' || tmp[i]>'9') && tmp[i]!='.' && tmp[i]!=',' && tmp[i]!='-' && tmp[i]!='+')
		{
			if (err)
				*err=i+1;
			free(tmp);
			return 0.0;
		}
	}
	float rv = atof(tmp);
	sfree(tmp);
	return rv;
}

int _atoi(const char *str, int *err)
{
	char *tmp = str_strip(str);
	for (int i=0; i<strlen(tmp); i++)
	{
		if (((tmp[i]<'0' || tmp[i]>'9') && tmp[i]!='-' && tmp[i]!='+') || (tmp[i]=='-' && i>0))
		{
			if (err)
				*err=i+1;
			free(tmp);
			return 0;
		}
	}
	int rv = atoi(tmp);
	sfree(tmp);
	return rv;
}

time_t _atodate(const char* str, int* err)
{
	int year;
	int month;
	int day;
	char* tmp = str_strip(str);
	*err = sscanf(tmp, "%d-%d-%d", &year, &month, &day)-3;
	free(tmp);
	struct tm *tm = malloc(sizeof(struct tm));
	tm->tm_year = year-1900;
	tm->tm_mon = month-1;
	tm->tm_mday = day;
	tm->tm_hour = 0;
	tm->tm_min = 0;
	tm->tm_sec = 0;
//	tm->tm_isdst = 0;
	time_t rv = my_timegm(tm);
	free(tm);
	return rv;
//	return year*10000+month*100+day;
}

time_t _str2time(const char* str, int* err, const char *format)
{
	int hour=0;
	int min=0;
	int sec=0;
	char* tmp = str_strip(str);
	*err = sscanf(tmp, format, &hour, &min, &sec)-3;
	free(tmp);
	return hour*60*60+min*60+sec;
}

time_t _atotime(const char* str, int* err)
{
	return _str2time(str, err, "%d:%d:%d");
}

char *time2str(time_t tm, int type)
{
	struct tm *tminfo = gmtime(&tm);
	char *rv = malloc(sizeof(char)*128);
//	sprintf(rv, "%d-%d-%d", tminfo->tm_year+1900, tminfo->tm_mon, tminfo->tm_mday);
	if (type==0)
	{
		strftime(rv, 127, "%H-%M-%S", tminfo);
	} else if (type==1)
	{
		strftime(rv, 127, "%Y-%m-%d %H-%M-%S", tminfo);
	}
	return rv;
}

time_t tz_diff()
{
	time_t tm1 = time(0);
	time_t tm2;
	struct tm *t2;
    t2 = gmtime(&tm1);
    tm2 = mktime(t2);
    return tm1-tm2;
}


time_t my_timegm(struct tm* _tm)
{
	//return mktime(_tm)+tz_diff();
	time_t ret;
	char *tz;

	tz = getenv("TZ");
	if (tz)
	{
		tz = strdup(tz);
	}
	setenv("TZ", "", 1);
	tzset();
	ret = mktime(_tm);
	if (tz)
	{
		setenv("TZ", tz, 1);
		free(tz);
	}
	else
	{
		unsetenv("TZ");
	}
	tzset();
	return ret;
}

float _ra(const char* str, int* err)
{
	int hour=0;
	int min=0;
	float sec=0;
	char* tmp = str_strip(str);
	*err = sscanf(tmp, "%d %d %f", &hour, &min, &sec)-3;
	if (*err)
	{
		*err = sscanf(tmp, "%d:%d:%f", &hour, &min, &sec)-3;
	}
	free(tmp);
	return hour*10000+min*100+sec;
}




