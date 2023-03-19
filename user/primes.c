#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int rd)
{
    int nums[50];
    int p[2];
    memset(nums,0,50);
    int index=0;
    while(read(rd,&nums[index++],4)!=0);
    close(rd);
    if(nums[1]==0)
    {
        printf("prime %d\n",nums[0]);
        return;
    }
    if(pipe(p)<0) return;
    int pid=fork();
    if(pid>0)
    {
        close(p[0]);
        printf("prime %d\n",nums[0]);
        for(int i=1;nums[i]!=0;i++)
        {
            if(nums[i]%nums[0]!=0) write(p[1],&nums[i],4);
        }
        close(p[1]);
    }
    else if(pid==0)
    {
        close(p[1]);
        primes(p[0]);
    }
    wait(0);
}

int main(int arvc,char* argv[])
{
    int initp[2];
    if(pipe(initp)<0)exit(1);
    int num=0;
    for(int i=2;i<=35;i++)
    {
        num=i;
        write(initp[1],&num,4);
    }
    close(initp[1]);
    primes(initp[0]);
    close(initp[0]);
    exit(0);
}
