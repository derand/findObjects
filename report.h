/*
 *
 *		report.h
 *
 *  Created by Andrey Derevyagin on 20/05/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */

struct position_t;
struct searcher_result_t;
enum hit_e;
//enum report_type_e;
struct settings_t;
struct searcher_file_result_t;
struct orb_position_t **orb;

typedef enum
{
	report_style_short=0,
	report_style_full=1,
	report_style_custom=2
} report_style_e;


typedef struct 
{
	enum report_type_e  type;
	report_style_e style;
	FILE *rf;
	
	struct settings_t *sttngs;
} report_t;

report_t *report_init(const char *file_name, enum report_type_e rt, report_style_e rs, struct settings_t *sttngs);
void report_free(report_t *);

int report_add_entry(report_t *, struct searcher_result_t *);
int report_add_title(report_t *, const char *);
int report_add_footer(report_t *);
int report_add_hit(report_t *, enum hit_e );
int report_add_file_title(report_t *);
int report_add_file_footer(report_t *, struct searcher_file_result_t *, struct orb_position_t **orb);
