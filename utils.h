/*
 *
 *		utils.h
 *
 *  Created by Andrey Derevyagin on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */


#define $msg(s,...)	fprintf(stdout,"\t" s "\n", __VA_ARGS__)
#define $log(s,...)	fprintf(stderr,"#log %s <%s:%u>:\t" s "\n", __func__,__FILE__,__LINE__,__VA_ARGS__)

void upper_case(char *str);
char* delete_spaces(const char* str);
void read_line(FILE* f, char *buff);


float _atof(const char* str, int* err);
time_t _atodate(const char* str, int* err);
time_t _atotime(const char* str, int* err);

void sfree(void *var);
char *time2str(time_t tm, int type);

time_t my_timegm(struct tm* _tm);

float _ra(const char* str, int* err);
