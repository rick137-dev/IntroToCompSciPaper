#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct mattone{
	int d1; 
	int d2; 
	char height[2]; 
	char color[30];
	
};
 char colors[7][20] = {"rosso","giallo","verde","blu","nero","bianco","arancione"};
struct mattone *file_function(FILE *f,int *lines){
  
int i =0;
int dim = 4;
char buf[25];
struct mattone *bricklist,*brick_redim;
bricklist = malloc(dim*sizeof(*bricklist));
if(bricklist==NULL){
	*lines =0;
	return NULL;
}
while((fgets(buf,sizeof(buf),f))!=NULL){
	sscanf(buf,"%dx%d %c %s",&bricklist[i].d1,&bricklist[i].d2,bricklist[i].height,bricklist[i].color);
	i++;
	if(i>=dim){
		dim = dim*2;
		brick_redim =realloc(bricklist,dim*(sizeof(*bricklist)));
		if(brick_redim ==NULL){
			return NULL;
		}
		bricklist = brick_redim;
	}
}
*lines = i;
bricklist = realloc(bricklist,i*sizeof(*bricklist));
return bricklist;

	
}
int max_list(int list[],int size){
	int i =0;
	int m =0;
	for(i=0;i<size;i++){
		if(list[i]>m){
			m=list[i];
		}
	}
	return m;
}

int massimo_colore(struct mattone *brick,int lines){
  int frequencies[7];
  int i =0;
  for(i=0;i<7;i++){
  	frequencies[i]=0;
  }
  for(int j =0;j<7;j++){
  for(i=0;i<lines;i++){
  	if(strcmp(brick[i].color,colors[j])==0){
  		frequencies[j]++;
  	}
  }
  }
  
  

  	return max_list(frequencies,7);
}
int total_height(struct mattone *brick,int lines){
	int i =0;
	char check[2] ="A";
	double total=0;
	for(i=0;i<lines;i++){
	if(brick[i].d1!=brick[i].d2){
	
		if(*brick[i].height==check[0]){
			total = total+1;
		
		}
		else{
			total = total +0.333333;
			
		}
	}
		
	}
	
   	return round(total);//needs linking to math.h with -lm
}
int max_two(int a, int b){
	if(a>=b){
		return a;
	}else{
		return b;
	}
	
}
int total_length(struct mattone *brick,int lines){
int length =0;
for(int i =0;i<lines;i++){
	length = length +max_two(brick[i].d1,brick[i].d2);
}
return length;
}	
int piramidi(struct mattone *brick,int lines){
return brick[0].d1+lines;
	
}



int main(int argc, char *argv[]){
argc = argc+0; //delete this once done
    int lines;
	FILE *myfile;
	myfile = fopen(argv[1],"r");
	if(myfile==NULL){
		printf("ERRORE AD APRIRE FILE\n");
		return 1;
	}
	struct mattone *mattoni;
	mattoni = file_function(myfile,&lines);
	
    printf("[MAX-PER-COLORE]\n%d\n",massimo_colore(mattoni,lines));
    printf("[ALTEZZA-NON-QUADRATI]\n%d\n",total_height(mattoni,lines));
    printf("[LUNGHEZZA]\n%d\n",total_length(mattoni,lines));
    printf("[PIRAMIDI]\n%d\n",piramidi(mattoni,lines));
  
	
}
