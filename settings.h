/*
 *
 *		settings.h
 *
 *  Created by Andrey Derevyagin on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */

 
typedef enum report_type_e
{
	report_type_txt=0,
	report_type_html=1
} report_type_e;


//! settings structure definition
typedef struct settings_t
{
	time_t start;
	time_t stop;
	float mag_min;
	float um_min;
	float ra_rate_min;
	float ra_rate_max;
	float decl_rate_min;
	float decl_rate_max;
	char* dir;
	char** black_list;
	float ra_position_min;
	float ra_position_max;
	
	enum report_type_e report_type;
	
	char *report_html_font_size;
} settings_t;

settings_t* settings_init();
void settings_free(settings_t* sttngs);

void settings_dump(settings_t* sttngs);

settings_t* settings_read_from_file(const char* file);
int settings_set_variable(const char* key, const char* val, settings_t* sttngs);

int settings_test_object_name(settings_t* sttngs, const char* oname);

