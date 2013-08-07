/*
 *
 *		report.c
 *
 *  Created by Andrey Derevyagin on 20/05/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <stdint.h>
#include <time.h>

#include "settings.h"
#include "report.h"
#include "searcher.h"
#include "one_position.h"
#include "utils.h"
#include "fastvec.h"


static char* hit_pref[]= {"--","НЗ","СЛ","БС","ТТ","НН","КК","ЛУ"};

#define TIME_STEP_SIZE			(60*10)
#define TIME_STEP_CHARS_COUNT	2


report_t *report_init(const char *file_name, enum report_type_e rt, report_style_e rs, struct settings_t *sttngs)
{
	report_t *rv = malloc(sizeof(report_t));
	if (rv)
	{
		rv->type = rt;
		rv->style = rs;
		rv->sttngs = sttngs;

		if (!(rv->rf=fopen(file_name, "w+")))
		{
			printf("Can't create file:\"%s\"\n", file_name);
			exit(1);
		}
	}
	return rv;
}

void report_free(report_t *report)
{
	if(report)
	{
		fclose(report->rf);
		free(report);
	}
}



int report_add_txt_short_entry(report_t *report, struct searcher_result_t *sr)
{
	position_t *pos = sr->pos;
	fprintf(report->rf, "%2s %8s %9s %12.2f %12.2f %10s %13.3f %6.1f\r\n", hit_pref[sr->hit], &pos->time_str[10], pos->decl_s, pos->ra, pos->decl, pos->hour_ang, pos->um, pos->mag);
	return 0;
}

int report_add_txt_full_entry(report_t *report, struct searcher_result_t *sr)
{
	position_t *pos = sr->pos;
	fprintf(report->rf, "%19s %11s %10s %12.2f %12.2f %11s %8.3f %13.3f %6.2f %6d %7.1f\r\n", pos->time_str, pos->ra_s, pos->decl_s, pos->ra, pos->decl, pos->hour_ang, pos->az, pos->um, pos->phase, pos->range, pos->mag);
	return 0;
}

int report_add_html_short_entry(report_t *report, struct searcher_result_t *sr)
{
	position_t *pos = sr->pos;
	fprintf(report->rf, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%.2f</td><td>%.2f</td><td>%s</td><td>%.3f</td><td>%.1f</td></tr>\n", hit_pref[sr->hit], &pos->time_str[10], pos->decl_s, pos->ra, pos->decl, pos->hour_ang, pos->um, pos->mag);
	return 0;
}

int report_add_html_full_entry(report_t *report, struct searcher_result_t *sr)
{
	position_t *pos = sr->pos;
	fprintf(report->rf, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%.2f</td><td>%.2f</td><td>%s</td><td>%.3f</td><td>%.3f</td><td>%.2f</td><td>%d</td><td>%.1f</td></tr>\n", pos->time_str, pos->ra_s, pos->decl_s, pos->ra, pos->decl, pos->hour_ang, pos->az, pos->um, pos->phase, pos->range, pos->mag);
	return 0;
}


int report_add_entry(report_t *report, struct searcher_result_t *sr)
{
	int rv;
	switch (report->type)
	{
		case report_type_txt:
				if (report->style==report_style_short || report->style==report_style_custom)
				{
					rv = report_add_txt_short_entry(report, sr);
				} else if (report->style==report_style_full)
				{
					rv = report_add_txt_full_entry(report, sr);
				}
				break;
		case report_type_html:
				if (report->style==report_style_short || report->style==report_style_custom)
				{
					rv = report_add_html_short_entry(report, sr);
				} else if (report->style==report_style_full)
				{
					rv = report_add_html_full_entry(report, sr);
				}
				break;
	}
	return rv;
}



int report_add_txt_short_title(report_t *report, const char *name)
{
	fprintf(report->rf, "%s\r\n", name);
	fprintf(report->rf, "      UTC      DECL       RARate     DECLRate    HourAng          Um       Mag\r\n");
	return 0;
}

int report_add_txt_full_title(report_t *report, const char *name)
{
	fprintf(report->rf, "%s\r\n", name);
	fprintf(report->rf, "   Date      UTC        RA         DECL      RARate       DeclRate     HourAng      Az          Um       Phase  Range   Mag\r\n");
	return 0;
}

int report_add_html_short_title(report_t *report, const char *name)
{
	fprintf(report->rf, "<br><p>%s</p>\n", name);
	fprintf(report->rf, "<table border=\"1\"><tr><td></td><td>UTC</td><td>DECL</td><td>RARate</td><td>DECLRate</td><td>HourAng</td><td>Um</td><td>Mag</td></tr>\n");
	return 0;
}

int report_add_html_full_title(report_t *report, const char *name)
{
	fprintf(report->rf, "<br><p>%s</p>\n", name);
	fprintf(report->rf, "<table border=\"1\"><tr><td>Time(UTC)</td><td>RA</td><td>DECL</td><td>RARate</td><td>DeclRate</td><td>HourAng</td><td>Az</td><td>Um</td><td>Phase</td><td>Range</td><td>Mag</td></tr>\n");
	return 0;
}

int report_add_title(report_t *report, const char *name)
{
	int rv;
	switch (report->type)
	{
		case report_type_txt:
				if (report->style==report_style_short || report->style==report_style_custom)
				{
					rv = report_add_txt_short_title(report, name);
				} else if (report->style==report_style_full)
				{
					rv = report_add_txt_full_title(report, name);
				}
				break;
		case report_type_html:
				if (report->style==report_style_short || report->style==report_style_custom)
				{
					rv = report_add_html_short_title(report, name);
				} else if (report->style==report_style_full)
				{
					rv = report_add_html_full_title(report, name);
				}
				break;
	}
	return rv;
}

int report_add_footer(report_t *report)
{
	int rv;
	switch (report->type)
	{
		case report_type_txt:
				fprintf(report->rf, "\r\n\r\n");
				rv = 0;
				break;
		case report_type_html:
				fprintf(report->rf, "</table><br>\n");
				if (report->style==report_style_short)
				{
				} else if (report->style==report_style_full)
				{
				}
				break;
	}
	return rv;
}

int report_add_hit(report_t *report, enum hit_e hit)
{
	int rv;
	switch (report->type)
	{
		case report_type_txt:
				if (report->style==report_style_full)
				{
					fprintf(report->rf, "%s\r\n", hit_pref[hit]);
					rv=0;
				}				
				break;
		case report_type_html:
				if (report->style==report_style_full)
				{
					fprintf(report->rf, "<tr><td colspan=\"11\" align=\"center\">%s</td></tr>\n", hit_pref[hit]);
				}
				break;
	}
	return rv;
}

int report_add_html_full_file_title(report_t *report)
{
	fprintf(report->rf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>Short report</title>");
	fprintf(report->rf, "<style type=\"text/css\">\n");
	fprintf(report->rf, "body, table {font-size: %s;}\n", report->sttngs->report_html_font_size);
//	fprintf(report->rf, "div.code * {font-family: courier;}\n");
	fprintf(report->rf, "</style>\n");
	fprintf(report->rf, "</head><body>\n");
	return 0;
}

int report_add_html_short_file_title(report_t *report)
{
	return report_add_html_full_file_title(report);
}



int report_add_file_title(report_t *report)
{
	int rv=0;
	switch (report->type)
	{
		case report_type_txt:
				if (report->style==report_style_full)
				{
//					fprintf(report->rf, "");
				}				
				break;
		case report_type_html:
				if (report->style==report_style_short || report->style==report_style_custom)
				{
					rv = report_add_html_short_file_title(report);
				} else if (report->style==report_style_full)
				{
					rv = report_add_html_full_file_title(report);
				}
				break;
	}
	return rv;
}

int *_observation_time_scale(report_t *report, struct searcher_result_t *sr_vec, int *buff, int steps)
{
	time_t tm = report->sttngs->start;
	int j;
	int val;
	for (int st=0; st<steps; st++)
	{
		for (j=0; j<vec_count(sr_vec); j++)
		{
			if (tm <= sr_vec[j].pos->timestamp)
			{
				break;
			}
		}
		if (j==vec_count(sr_vec))
		{
			val = sr_vec[j-1].hit==hit_done;
		}
		else if (j==0)
		{
			val = sr_vec[0].hit==hit_done;
		}
		else
		{
			val = sr_vec[j-1].hit==hit_done || sr_vec[j].hit==hit_done;
		}
		buff[st] = val;
		tm += TIME_STEP_SIZE;
	}
	return buff;
}

int report_add_txt_custom_file_footer(report_t *report, struct searcher_file_result_t *sfr)
{
	char *tmp_str;
	char *tmp;
	size_t steps = (report->sttngs->stop-report->sttngs->start)/TIME_STEP_SIZE;

	tmp_str = malloc(sizeof(char)*steps*TIME_STEP_CHARS_COUNT+1);
	tmp_str[sizeof(char)*steps*TIME_STEP_CHARS_COUNT] = '\0';
	time_t tm = report->sttngs->start;
	char ch=' ';
	int j;
	for (int st=0; st<steps; st++)
	{
		struct tm *tminfo = gmtime(&tm);
		if (tminfo->tm_min == 0)
		{
			sprintf(&tmp_str[st*TIME_STEP_CHARS_COUNT], "%02d", tminfo->tm_hour);
		}
		else
		{
			for (j=0; j<TIME_STEP_CHARS_COUNT; j++)
			{
				tmp_str[st*TIME_STEP_CHARS_COUNT+j] = ch;
			}
		}
		tm += TIME_STEP_SIZE;
	}
	fprintf(report->rf, "\t\t%s\n", tmp_str);
	free(tmp_str);

	for (int i=0; i<vec_count(sfr); i++)
	{
		tmp_str = strdup(sfr[i].filename);
		if((tmp = strstr(tmp_str, ".")) != NULL)
		{
			//tmp[0] = '\0';
		}
		fprintf(report->rf, "%s\t", tmp_str);
		free(tmp_str);

		int *buff = malloc(sizeof(int)*steps);
		buff = _observation_time_scale(report, sfr[i].sr_vec, buff, steps);

		tmp_str = malloc(sizeof(char)*steps*TIME_STEP_CHARS_COUNT+1);
		tmp_str[sizeof(char)*steps*TIME_STEP_CHARS_COUNT] = '\0';
		for (int st=0; st<steps; st++)
		{
			for (j=0; j<TIME_STEP_CHARS_COUNT; j++)
			{
				tmp_str[st*TIME_STEP_CHARS_COUNT+j] = buff[st]?'*':'-';
			}
		}
		free(buff);
		fprintf(report->rf, tmp_str);
		free(tmp_str);

		fprintf(report->rf, "\n");
	}

	return 0;
}

int report_add_html_custom_file_footer(report_t *report, struct searcher_file_result_t *sfr)
{
	char *tmp_str;
	char *tmp;
	size_t steps = (report->sttngs->stop-report->sttngs->start)/TIME_STEP_SIZE;

	time_t tm = report->sttngs->start;
	int j;
	fprintf(report->rf, "<div style=\"font-family: courier;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
	for (int st=0; st<steps; st++)
	{
		struct tm *tminfo = gmtime(&tm);
		if (tminfo->tm_min == 0)
		{
			fprintf(report->rf, "%02d", tminfo->tm_hour);
		}
		else
		{
			for (j=0; j<TIME_STEP_CHARS_COUNT; j++)
			{
				fprintf(report->rf, "&nbsp;");
			}
		}
		tm += TIME_STEP_SIZE;
	}
	fprintf(report->rf, "<br>\n");

	for (int i=0; i<vec_count(sfr); i++)
	{
		tmp_str = strdup(sfr[i].filename);
		if((tmp = strstr(tmp_str, ".")) != NULL)
		{
			//tmp[0] = '\0';
		}
		fprintf(report->rf, "%s&nbsp;&nbsp;", tmp_str);
		free(tmp_str);

		int *buff = malloc(sizeof(int)*steps);
		buff = _observation_time_scale(report, sfr[i].sr_vec, buff, steps);

		tmp_str = malloc(sizeof(char)*steps*TIME_STEP_CHARS_COUNT+1);
		tmp_str[sizeof(char)*steps*TIME_STEP_CHARS_COUNT] = '\0';
		for (int st=0; st<steps; st++)
		{
			for (j=0; j<TIME_STEP_CHARS_COUNT; j++)
			{
				tmp_str[st*TIME_STEP_CHARS_COUNT+j] = buff[st]?'*':'-';
			}
		}
		free(buff);
		fprintf(report->rf, tmp_str);
		free(tmp_str);

		fprintf(report->rf, "<br>\n");
	}
	fprintf(report->rf, "</div>\n</body></html>\n");

	return 0;
}

int report_add_file_footer(report_t *report, struct searcher_file_result_t *sfr)
{
	int rv=0;
	switch (report->type)
	{
		case report_type_txt:
				if (report->style==report_style_custom)
				{
					rv = report_add_txt_custom_file_footer(report, sfr);
				}				
				break;
		case report_type_html:
				if (report->style==report_style_short)
				{
					fprintf(report->rf, "</body></html>\n");
				} else if (report->style==report_style_custom)
				{
					rv = report_add_html_custom_file_footer(report, sfr);
				} else if (report->style==report_style_full)
				{
					fprintf(report->rf, "</body></html>\n");
				}
				break;
	}
	return rv;
}

