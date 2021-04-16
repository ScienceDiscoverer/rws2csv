#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct Reading
{
	unsigned int dt;
	unsigned int rd;
};

int main(int argc, char** argv)
{	
	if(argc != 2)
	{
		printf("Usage: rws2csv <youre_rws_file_name>.rws\n");
		return 1;
	}
	
	FILE* f = fopen(argv[1], "rb");
	int sl = strlen(argv[1]);
	argv[1][sl-1] = 'v';
	argv[1][sl-2] = 's';
	argv[1][sl-3] = 'c';
	FILE* fo = fopen(argv[1], "w");
	
	while(1)
	{
		Reading r;
		int res = fread(&r, sizeof(Reading), 1, f);
		if(res <= 0)
		{
			break;
		}
		
		int co2 = r.rd >> 19;
		int humd = (r.rd & 0x7F000) >> 12;
		float temp = ((r.rd & 0xFF0) >> 4) + (r.rd & 0xF)/10.0f;
		
		// fprintf(fo, "%d,%d,%d,%3.1f\n", r.dt, co2, humd, temp); // If you only want plain UNIX time
		
		struct tm s = *localtime((time_t*)&r.dt);
		fprintf(fo, "%d.%02d.%02d %02d:%02d:%02d,%d,%d,%3.1f\n", s.tm_year + 1900, s.tm_mon + 1,
		s.tm_mday, s.tm_hour, s.tm_min, s.tm_sec, co2, humd, temp);
	}
	
	fclose(f);
	fclose(fo);
	
	return 0;
}