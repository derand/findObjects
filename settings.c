/*
 *
 *		settings.c
 *
 *  Created by Andrey Derevyagin on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#include "utils.h"
#include "settings.h"
#include "fastvec.h"

settings_t* settings_init()
{
	settings_t* rv = malloc(sizeof(settings_t));
	if (rv)
	{
		rv->start = 0;
		rv->stop = 0;
		rv->dir = 0;
		vec_init((void**)&rv->black_list, sizeof(char *), 0, 10);
		rv->report_type = report_type_txt;
		rv->use_orb_file = 1;
		rv->orb_file = 0;
	}
	return rv;
}

void settings_free(settings_t* sttngs)
{
	if (sttngs)
	{
		sfree(sttngs->report_html_font_size);
		for(int i=0; i<vec_count(sttngs->black_list); i++)
		{
			free(sttngs->black_list[i]);
		}
		vec_free(sttngs->black_list);

		sfree(sttngs->dir);
		sfree(sttngs->orb_file);
		free(sttngs);
	}
}

//        ----          main        ----
settings_t* settings_read_from_file(const char* file)
{
	settings_t* rv = settings_init();
	
	FILE* f;
	int line_counter= 0;
	int err;
	if (f=fopen(file, "r"))
	{
		char* buff = malloc(sizeof(char)*1025);
		char* val;
		while (!feof(f))
		{
//			fscanf(f, "%s", buff);
			read_line(f, buff);
			line_counter++;
			if (buff[0]=='#' || strlen(buff)<3)
				continue;
			val = strchr(buff,'=');
			if (!val)
			{
				$msg("Error in file \"%s\" at line %d", file, line_counter);
				$msg("line:%s", buff);
				exit(1);
			}
			val[0] ='\0';
			val++;

			if (err = settings_set_variable(buff, val, rv))
			{
				$msg("Error in file \"%s\" at line %d", file, line_counter);
				$msg("Can't convert variable:%s", val);
				exit(1);
			}

		}
		free(buff);
		
		fclose(f);
	}

	if (rv->start<86401)
	{
		time_t now = time(0);
		struct tm* tm = gmtime(&now);
//		$msg("%d-%d-%d", tm->tm_year, tm->tm_mon, tm->tm_mday);
		tm->tm_hour = 0;
		tm->tm_min = 0;
		tm->tm_sec = 0;
		tm->tm_isdst = 0;
		rv->start += my_timegm(tm);
	}
	if (rv->stop<86401)
	{
		time_t now = time(0);
		struct tm* tm = gmtime(&now);
		tm->tm_hour = 0;
		tm->tm_min = 0;
		tm->tm_sec = 0;
		tm->tm_isdst = 0;
		rv->stop += my_timegm(tm);
	}
	while (rv->stop<rv->start)
		rv->stop+=86400;

	// search orb file
	if (rv->use_orb_file && !rv->orb_file)
	{
		DIR *dp;
		struct dirent *dirp;
		if((dp=opendir(".")) == 0)
		{
			printf("can't open current dir\n");
			exit(1);
   		}
   		int idx = 0;
   		int tmp_idx;
		while ((dirp = readdir(dp))!=0)
		{
			if (strlen(dirp->d_name)>3)
			{
				if (toupper(dirp->d_name[0]) == 'O' &&
					toupper(dirp->d_name[1]) == 'R' &&
					toupper(dirp->d_name[2]) == 'B' &&
					dirp->d_name[3] >= '0' && dirp->d_name[3] <= '9')
				{
					tmp_idx = atoi(&(dirp->d_name[3]));
					if (!rv->orb_file || tmp_idx > idx)
					{
						sfree(rv->orb_file);
						rv->orb_file = strdup(dirp->d_name);
						idx = tmp_idx;
					}
				}
			}
		}
		closedir(dp);
	}

	// check orb file exist
	if (rv->use_orb_file)
	{
		if (rv->orb_file)
		{
			if (access(rv->orb_file, F_OK) == -1)
			{
				rv->use_orb_file = 0;
				$msg("Orb file \"%s\" does not exist", rv->orb_file);
			}
		}
		else
		{
			$msg("%s", "Can't find orb file on current directory");
			rv->use_orb_file = 0;
		}
	}


	return rv;
}

void settings_dump(settings_t* sttngs)
{
	char* start_str = time2str(sttngs->start, 1);
	char* end_str = time2str(sttngs->stop, 1);
	printf("settings dump:\n\tstart: %s\n\tstop: %s\n\tmag:%.2f\n\tRA_min:%.2f\n\tRA_max:%.2f\n\tdecl_min:%.2f\n\tdecl_max:%.2f\n\tra_position_min:%f\n\tra_position_max:%f\n\treport_html_font_size:%s\n\tdir:\"%s\"\n\tBlack list:", start_str, end_str, sttngs->mag_min, sttngs->ra_rate_min, sttngs->ra_rate_max, sttngs->decl_rate_min, sttngs->decl_rate_max, sttngs->ra_position_min, sttngs->ra_position_max, sttngs->report_html_font_size, sttngs->dir);
	for (int i=0; i<vec_count(sttngs->black_list); i++)
	{
		printf(" %s", sttngs->black_list[i]);
	}
	printf("\n\tuse_orb_file: %s", sttngs->use_orb_file?"Yes":"No");
	if (sttngs->orb_file)
	{
		printf("\n\torb_file: %s", sttngs->orb_file);
	}
	printf("\n\n");
	free(start_str);
	free(end_str);
}



int settings_set_variable(const char* key, const char* val, settings_t* sttngs)
{
	char* _key = str_strip(key);
	upper_case(_key);
	char* _val = str_strip(val);
	int rv = 0;
	if (strcmp(_key, "START_OBSERVATION")==0)
	{
		sttngs->start += _atotime(_val, &rv);
		if (rv==-1)
			rv=0;
	} else if (strcmp(_key, "END_OBSERVATION")==0)
	{
		sttngs->stop += _atotime(_val, &rv);
		if (rv==-1)
			rv=0;
	} else if (strcmp(_key, "MAG")==0)
	{
		sttngs->mag_min = _atof(_val, &rv);
	} else if (strcmp(_key, "UM")==0)
	{
		sttngs->um_min = _atof(_val, &rv);
	} else if (strcmp(_key, "RA_RATE_MIN")==0)
	{
		sttngs->ra_rate_min = _atof(_val, &rv);
	} else if (strcmp(_key, "RA_RATE_MAX")==0)
	{
		sttngs->ra_rate_max = _atof(_val, &rv);
	} else if (strcmp(_key, "DECL_RATE_MIN")==0)
	{
		sttngs->decl_rate_min = _atof(_val, &rv);
	} else if (strcmp(_key, "DECL_RATE_MAX")==0)
	{
		sttngs->decl_rate_max =  _atof(_val, &rv);
	} else if (strcmp(_key, "DAY")==0)
	{
		sttngs->start += _atodate(_val, &rv);
		sttngs->stop += _atodate(_val, &rv);
	} else if (strcmp(_key, "EFEM_DIR")==0)
	{
		sttngs->dir = strdup(_val);
	} else if (strcmp(_key, "BLACK_LIST")==0)
	{
//		el = strdup(dirp->d_name);
//		vec_add((void **)&files, (void *)&el);
		int i=0;
		char* str = _val;
		char* tmp;
		int len = strlen(_val);
		int flag=1;
		while (++i<len)
		{
			if (_val[i]==' ' || _val[i]=='\t' || _val[i]=='\n')
			{
				if (flag)
				{
					_val[i]='\0';
					tmp = str_strip(str);
					if (strlen(tmp))
					{
						vec_add((void **)&sttngs->black_list, (void *)&tmp);
						str = &(_val[i])+1;
						flag=0;
					}
					else
					{
						free(tmp);
					}
				}
			}
			else
			{
				flag=1;
			}
		}
		tmp = str_strip(str);
		if (strlen(tmp))
		{
			vec_add((void **)&sttngs->black_list, (void *)&tmp);
		}
		else
		{
			free(tmp);
		}
	} else if (strcmp(_key, "RA_POSITION_MIN")==0)
	{
		sttngs->ra_position_min = _ra(_val, &rv);
	} else if (strcmp(_key, "RA_POSITION_MAX")==0)
	{
		sttngs->ra_position_max = _ra(_val, &rv);
	} else if (strcmp(_key, "REPORT_TYPE")==0)
	{
		if (strcmp(_val, "HTML")==0)
		{
			sttngs->report_type = report_type_html;
		}
	} else if (strcmp(_key, "REPORT_HTML_FONT_SIZE")==0)
	{
		sttngs->report_html_font_size = strdup(_val);
	} else if (strcmp(_key, "USE_ORB_FILE")==0)
	{
		sttngs->use_orb_file = (strcmp(_val, "NO")!=0);
	} else if (strcmp(_key, "ORB_FILE")==0)
	{
		sttngs->orb_file = strdup(_val);
	}
	sfree(_val);
	sfree(_key);
	return rv;
}

int settings_test_object_name(settings_t* sttngs, const char* oname)
{
	int rv = 0;
	for (int i=0; i<vec_count(sttngs->black_list); i++)
	{
		if (strcmp(sttngs->black_list[i], oname)==0)
		{
			rv = 1;
			break;
		}
	}
	return rv;
}

