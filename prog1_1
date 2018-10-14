#include<stdio.h>
int main(int c,char**v){
	FILE*f;
	for(unsignedi=c>1,k,r,x;i^c;)
		if(f=i++?fopen(*++v,"rb"):stdin){
			for(k=r=~0;++k&7||(r^=x=fgetc(f),x<~x);r=r%2*0xEDB88320^r/2);
			ferror(f)||printf("%08x%s%s\n",x^~r,"\t"+(c<3),c>2?*v:"");
			fclose(f);
		}
}