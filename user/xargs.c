#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char* readline()
{
    char* buf=malloc(MAXARG*6);
    char* p=&buf[0];
    while(read(0,p,1)!=0)
    {
        if(*p=='\n'||*p=='\0')
        {
            *p='\0';
            return buf;
        }
        p++;
    }
    //if(p!=buf) return buf;
    free(buf);
    return 0;
}

int main(int argc,char* argv[])
{
    if(argc<2) 
    {
        printf("error!\n");
        exit(1);
    }
    char* exarg;
    argv++;
    char* nargv[MAXARG];
    int index=0;
    char** pa=argv;
    while(*pa!=0) nargv[index++]=*(pa++);
    while((exarg=readline())!=0)
    {
        char* p=exarg; --p;
        char* buf=malloc(20);
        char* bp=buf;
        int nargc=argc-1;
        while(*(++p) != 0)
        {
            if(*p==' '&&buf!=bp)
            {
                nargv[nargc++]=buf;
                memset(buf,0,sizeof(buf));
                bp=buf;
            }
            else *(bp++) = *p;
        }
        if(buf!=bp) nargv[nargc++]=buf;
        nargv[nargc]=0;

        int pid=fork();
        if(pid==0) exec(nargv[0],nargv);
        else wait(0);
    }
    exit(0);
}
