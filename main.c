/*
 *
 *		main.c
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
#include "settings.h"
#include "searcher.h"
#include "one_position.h"

int bad_params()
{
	printf("\tError in params. Type -h or --help to help.\r\n");
	return 1;
}

int main(int argc, char *argv[])
{
	printf("Find Objects version 0.16\r\n");
	char *fn = "config.txt";
	if (argc==2)
	{
		if (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "--help")==0)
		{
			printf("\r\n\t\"Find Objects\" appllication for check observation sattelites.\r\nFor work need \"config.txt\" file on current directory or you can set as first param of command. Every line on config file\r\nlook like \"<key>=<value>\". If line in config file starts from '#' charset\r\nthen it's line is commented.\r\nFull list of keys in config file:\r\n");
			printf("  EFEM_DIR          - this parametr set directory with efemerid files\r\n");
			printf("                      name of files looks like \"34234.txt\"\r\n");
			printf("  DAY               - day of observation (if not set getted current day)\r\n");
			printf("  START_OBSERVATION - start observation time\r\n");
			printf("  END_OBSERVATION   - end observation time\r\n");
			printf("  MAG               - minimal magnitude\r\n");
			printf("  UM                - minimal elevations\r\n");
			printf("  RA_RATE_MIN       - minimal ra_rate\r\n");
			printf("  RA_RATE_MAX       - maximal ra_rate\r\n");
			printf("  DECL_RATE_MIN     - minimal decl rate\r\n");
			printf("  DECL_RATE_MAX     - maximal decl rate\r\n");
			printf("  BLACK_LIST        - list of objects exclude from observation\r\n");
			printf("\r\nAUTHOR\r\n");
			printf("  Writen by Andrey Derevyagin spesial for Aza Klabukova");
			printf("\r\nCOPYRIGHT\r\n");
			printf("  Copyright © 2010-2013 Andrey Derevuagin");
			printf("\r\nBUGS\r\n");
			printf("  If you feel you have found a bug in \"Find Objects\", please email me 2derand+findobjects@gmail.com\r\n");
			printf("\r\n");
			printf("If you want have source codes of application email me - 2derand+findobjects@gmail.com\r\n");
			printf("\r\n");
			return 0;
		} else
		{
			FILE *f = fopen(argv[1], "r");
			if (f == NULL)
			{
				printf("Can't read file %s\r\n", argv[1]);
				return 2;
			} else
			{
				fn = argv[1];
				fclose(f);
			}

		}
	} else if (argc>1)
	{
		return bad_params();
	}
//	position_from_str("2010-02-01  23:30:00  08 49 40.3  +03 24 08     <<SHADOW>>        01 29 38.7  211.243 42.798  13.84  39645 ----");
//	position_from_str("2010-02-01  16:00:00  01 35 17.9  \t -12 48 29  +000953.2 +000143.8  01 12 47.3  200.289 28.576 113.83  37634 16.7");
//	position_from_str("2010-02-01  16:00:00  01 35 17.9  -12 48 29  +000953.2 +000143.8  01 12 47.3  200.289 28.576 113.83  37634 16.7\r\n");


	settings_t* sttngs = settings_read_from_file(fn);
	settings_dump(sttngs);
	
	searcher_main_loop(sttngs);
	
	settings_free(sttngs);
	return 0;
}
