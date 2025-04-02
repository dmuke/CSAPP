#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

int tag_v = 0;
int s,E,b,S,B;
typedef __uint64_t uint64_t;
typedef struct{
	int valid;
	int tag;
	int time_stamp;
}cache_line;
cache_line** cache;
enum Category {HIT,MISS,EVICTION};
unsigned int result[3];
char* str[3]={"hit","miss","evction"};
int T = 0;

FILE* option(int argc,char* argv[]);
void init();
void setResult(cache_line* group,enum Category category,int tag,int index,char* verbose);
void findCache(uint64_t tag,int group_index,char* verbose);

int main(int argc,char* argv[])
{
	FILE* file;
	file = option(argc,argv);
	init();
	int mask_b = (1 << b) - 1;
	int mask_s = ((1 << (s+b)) -1) ^ mask_b;
	char oper[2];
	uint64_t address;
	long int size = 0;
	char c;
	while (fscanf(file,"%s%lx%c%lu",oper,&address,&c,&size) != EOF){
		if (oper[0] == 'I') continue;
		int group_index = (address & mask_s) >> b;
		uint64_t tag = address >> (b+s);
		char verbose[20];
		memset(verbose,0,sizeof(verbose));
		T++;
		findCache(tag,group_index,verbose);
		if (oper[0] == 'M') findCache(tag,group_index,verbose);
		if (tag_v) fprintf(stdout,"%s %lx %ld %s \n",oper,address,size,verbose);
	}	
    printSummary(result[0],result[1], result[2]);
	free(cache);
    return 0;
}

void init(){
	cache = (cache_line**)malloc(sizeof(cache_line*) * S);
	for (int i = 0;i < S;i++){
		cache[i] = (cache_line*)malloc(sizeof(cache_line)*E);
		for (int j=0;j < E;j++){
			cache[i][j].time_stamp = 0;
			cache[i][j].valid = 0;
			cache[i][j].tag = 0; }
	}
}

void setResult(cache_line* group,enum Category category,int tag,int index,char* verbose){
	++result[category];
	group[index].valid = 1;
	group[index].time_stamp = T;
	group[index].tag = tag;
	if (tag_v) strcat(verbose,str[category]);
}

void findCache(uint64_t tag,int group_index,char* verbose){
	cache_line* group = cache[group_index];
	int min_index = 0,empty = -1;
	for (int i = 0; i < E;i++){
		cache_line line = group[i];
		if (!line.valid)	empty = i;
		else{
			if (line.tag == tag){
				setResult(group,HIT,tag,i,verbose);
				return;
			}if (group[min_index].time_stamp > line.time_stamp)	min_index = i;
		}
	}setResult(group,MISS,tag,empty,verbose);
	if (empty == -1)
		setResult(group,EVICTION,tag,min_index,verbose);
}

FILE* option(int argc,char* argv[])
{
	FILE* tracefile;
	char * tracefile_name;
	int opt = 0;
	while ((opt = getopt(argc,argv,"sEbt:v")) != -1){
		switch(opt){
			case 'v':
				tag_v = 1;
				break;
			case 's':
				s = atoi(argv[optind]);
				S = 1 << s;
				break;
			case 'E':
				E = atoi(argv[optind]);
				break;
			case 'b':
				b = atoi(argv[optind]);
				B = 1 << b;
				break;
			case 't':
				tracefile_name = argv[optind-1];
				tracefile = fopen(tracefile_name,"r");
				if (tracefile == NULL){
					perror(tracefile_name);
					printf("FAILURE");
					exit(EXIT_FAILURE);
				}break;

			default:
				exit(EXIT_FAILURE);
		}
	}
	return tracefile;
}
