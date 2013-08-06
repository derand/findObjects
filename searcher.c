/*
 *
 *		searcher.c
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

#include <dirent.h>

#include "utils.h"
#include "settings.h"
#include "searcher.h"
#include "fastvec.h"
#include "one_position.h"
#include "report.h"


int searcher_test_name(char *name, settings_t* sttngs);
enum hit_e searcher_test_position(position_t* pos, settings_t *sttngs);
//int searcher_test_file(char *name, settings_t *sttngs, FILE *result_short, FILE *result_full);
int searcher_test_file(char *name, settings_t *sttngs, report_t *report_short, report_t *report_full);


int searcher_test_name(char *name, settings_t* sttngs)
{
	char *str = strdup(name);
	char *ext = strchr(str,'.');
	int rv = 0;
	if (ext!=0)
	{
		ext[0] = '\0';
		ext++;
		upper_case(ext);
		if (strcmp("TXT", ext)==0)
		{
			if (settings_test_object_name(sttngs, str))
			{
				goto exit001;
			}
			for (int i=0; i<strlen(str)-1; i++)
			{
				if (str[i]<'0' || str[i]>'9')
					goto exit001;
			}
			rv = 1;
		}
	}

exit001:	
	free(str);
	return rv;
}

enum hit_e searcher_test_position(position_t* pos, settings_t *sttngs)
{
	enum hit_e rv = hit_done;
	if (pos->timestamp < sttngs->start)
	{
		rv = hit_start_ephemerides;
	} else if (pos->timestamp >= sttngs->stop)
	{
		rv = hit_end_ephemerides;
	} else if (pos->shadow)
	{
		rv = hit_in_shadow;
	} else if (pos->um < sttngs->um_min)
	{
		rv = hit_low_height;
	} else if (pos->mag > sttngs->mag_min)
	{
		rv = hit_low_mag;
	} else if (pos->ra < sttngs->ra_rate_min || pos->ra > sttngs->ra_rate_max)
	{
		rv = hit_fast;
	} else if (pos->decl < sttngs->decl_rate_min || pos->decl > sttngs->decl_rate_max)
	{
		rv = hit_fast;
	} else if ((sttngs->ra_position_min < sttngs->ra_position_max && pos->__ra > sttngs->ra_position_min && pos->__ra < sttngs->ra_position_max) ||
	           (sttngs->ra_position_max < sttngs->ra_position_min && (pos->__ra > sttngs->ra_position_min || pos->__ra < sttngs->ra_position_max)))
	{
		rv = hit_in_moon;
	}
	return rv;
}


int searcher_test_file(char *name, settings_t *sttngs, report_t *report_short, report_t *report_full)
{
	char *full_path = malloc(sizeof(char)*(strlen(name)+strlen(sttngs->dir)+1));
	memcpy(full_path, sttngs->dir, strlen(sttngs->dir));
	memcpy(&full_path[strlen(sttngs->dir)], name, strlen(name));
	full_path[strlen(name)+strlen(sttngs->dir)] = '\0';
	$msg("test: %s", full_path);
	
	int rv = 0;
	FILE *f;
	int lines=0;
	position_t* pos = 0;
	position_t* last_pos = 0;
	if (f = fopen(full_path, "r"))
	{
		enum hit_e last_hit = hit_start_ephemerides;
		enum hit_e hit;
		struct searcher_result_t *sr;
		vec_init((void**)&sr, sizeof(struct searcher_result_t), 0, 3);
		struct searcher_result_t sr_tmp;
		struct searcher_result_t sr_max_mag;

		char *buff = malloc(sizeof(char)*1024);
		int first=1;
//		char* ch;
		while (!feof(f))
		{
			read_line(f, buff);
			if(buff[0]=='2')
			{
				pos = position_from_str(buff);
				if (pos)
				{
					if ((hit=searcher_test_position(pos, sttngs))!=last_hit)
					{
						$msg("----: %d %s", hit, pos->time_str);
						if (hit==hit_done)
						{
							sr_tmp.in = 0;
							sr_tmp.pos = (struct position_t *)position_cpy(pos);
							sr_tmp.hit = last_hit;
							vec_add((void **)&sr, (void *)&sr_tmp);

							sr_max_mag.pos = (struct position_t *)position_cpy(pos);
							sr_max_mag.hit = hit_done;
						} else if (last_hit==hit_done)
						{
							vec_add((void **)&sr, (void *)&sr_max_mag);

							sr_tmp.in = 0;
							sr_tmp.pos = (struct position_t *)position_cpy(last_pos);
							sr_tmp.hit = hit;
							vec_add((void **)&sr, (void *)&sr_tmp);
						}
					}
					if (hit==hit_done)
					{
						if (first)
						{
//							fprintf(result_full, "%s\n", name);
//							fprintf(result_full, "   Date      UTC        RA         DECL      RARate       DeclRate     HourAng      Az          Um       Phase  Range   Mag\n");
							report_add_title(report_full, name);
							first = 0;
						} else if (last_hit!=hit_done)
						{
//							fprintf(result_full, "%s\n", hit_pref[last_hit]);
							report_add_hit(report_full, last_hit);
						}
						sr_tmp.pos = pos;
						sr_tmp.hit = hit;
						report_add_entry(report_full, &sr_tmp);

						if (sr_max_mag.pos->mag > pos->mag)
						{
							position_free(sr_max_mag.pos);
							sr_max_mag.pos = (struct position_t *)position_cpy(pos);
						}
/*
						ch = time2str(pos->timestamp, 1);
						fprintf(result_full, "%19s %11s %10s %12.2f %12.2f %11s %8.3f %13.3f %6.2f %6d %7.2f\n", pos->time_str, pos->ra_s, pos->decl_s, pos->ra, pos->decl, pos->hour_ang, pos->az, pos->um, pos->phase, pos->range, pos->mag);
						if(ch)
						{
							free(ch);
						}
*/
					}
					last_hit = hit;
					position_free(last_pos);
					last_pos = pos;
				}
				lines++;
			}
		}
		$msg("----: %d/%d", last_hit, hit);
		if (!first)
		{
//			fprintf(result_full, "\n\n");
			report_add_footer(report_full);
		}
		position_free(last_pos);
		if (vec_count(sr))
		{
//			fprintf(result_short, "%s\t(%zd)\n", name, vec_count(sr));
//			fprintf(result_short, "      UTC      DECL       RARete     DECLRate    HourAng          Um       Mag\n");
			report_add_title(report_short, name);
//			char *ch; 
			for(int i=0; i<vec_count(sr); i++)
			{
				report_add_entry(report_short, &sr[i]);
//				pos = (position_t *)sr[i].pos;
//				ch = time2str(pos->timestamp, 0);
//				fprintf(result_short, "%2s %8s %9s %12.2f %12.2f %10s %13.3f %6.2f\n", hit_pref[sr[i].hit], &pos->time_str[10], pos->decl_s, pos->ra, pos->decl, pos->hour_ang, pos->um, pos->mag);
//				if(ch) 
//					free(ch);
			}
			if (lines)
			{
//				fprintf(result_short, "\n\n");
				report_add_footer(report_short);
			}

			for(int i=0; i<vec_count(sr); i++)
			{
				position_free((position_t *)sr[i].pos);
			}
		}
		vec_free(sr);
		
		free(buff);
		fclose(f);
	}
	else
	{
		$msg("Can't read file \"%s\"", full_path);
		rv = 1;
	}
	
	free(full_path);
	return rv;
}

void searcher_main_loop(settings_t* sttngs)
{
	DIR *dp;
	struct dirent *dirp;
	if((dp=opendir(sttngs->dir)) == 0)
	{
		printf("can't open dir\n");
		exit(1);
    }

	char **files;
	vec_init((void**)&files, sizeof(char *), 0, 10);
	char *el;
	while ((dirp = readdir(dp))!=0)
	{
		if (searcher_test_name(dirp->d_name, sttngs))
		{
			el = strdup(dirp->d_name);
			vec_add((void **)&files, (void *)&el);
		}
	}
	closedir(dp);
/*	
	char* file_name = "./result_short.txt";
	FILE* res_short;
	if (!(res_short=fopen("./result_short.txt", "w+")))
	{
		printf("Can't create file:\"%s\"\n", file_name);
		exit(1);
	}
	file_name = "./result_full.txt";
	FILE* res_full;
	if (!(res_full=fopen("./result_full.txt", "w+")))
	{
		printf("Can't create file:\"%s\"\n", file_name);
		exit(1);
	}
*/
	char *report_short_file_name = "./result_short.txt";
	char *report_full_file_name = "./result_full.txt";
	if (sttngs->report_type==report_type_html)
	{
		report_short_file_name = "./result_short.html";
		report_full_file_name = "./result_full.html";
	}
	report_t *report_short = report_init(report_short_file_name, sttngs->report_type, report_style_short, (struct settings_t *)sttngs);
	report_t *report_full = report_init(report_full_file_name, sttngs->report_type, report_style_full, (struct settings_t *)sttngs);
	report_add_file_title(report_short);
	report_add_file_title(report_full);
	for(int i=0; i<vec_count(files); i++)
	//for(int i=5; i<7; i++)
	{
//		searcher_test_file(files[i], sttngs, res_short, res_full);
		searcher_test_file(files[i], sttngs, report_short, report_full);
	}
	report_add_file_footer(report_short);
	report_add_file_footer(report_full);
	
	report_free(report_full);
	report_free(report_short);

//	fclose(res_full);
//	fclose(res_short);

	for(int i=0; i<vec_count(files); i++)
	{
		free(files[i]);
	}
	vec_free(files);
}
