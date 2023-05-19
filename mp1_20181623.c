#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int trash, char** str)
{
    clock_t start;
    start = clock();
    double duration;
    int n,m,ti,tj,k,i,j,ver,fini,finj,thissum,maxsum;
    char filename[100];
    strcpy(filename,str[1]);//콘솔창에서 인풋파일의 이름을 받아온다
    ver = atoi(str[2]);//콘솔창에서 index를 정해준다
    if(ver != 1 && ver != 2 && ver != 3)
    {
        printf("input correct ver!!!(1,2,3)\n");
        return 1;
    }//예외처리
    char outfile[100]="result_";
    FILE* fp =fopen(filename,"r");
    if(fp == NULL)
    {
        printf("open error!!!\n");
        return 1;
    }
    fscanf(fp,"%d %d",&m,&n);//파일에서 matrix의 사이즈를 받아온다.
    int** array = (int**)malloc(sizeof(int*)*m);
    for(j=0; j<m; j++)
        array[j] = (int*)malloc(sizeof(int)*n);//행렬 동적할당중
    for(j=0; j<m; j++)
        for(i=0; i<n; i++)
            fscanf(fp,"%d",&array[j][i]);//파일 입출력을 통해 값을 받아온다.
    fclose(fp);
    maxsum = array[0][0];//우선 최대값을 0,0의 값으로 지정한다.
    if(ver == 3)//시간복잡도가 O(n^3)인 경우
    {
        int* thisarr1 = (int*)malloc(sizeof(int*)*m);
        for(i=0; i<n; i++)
        {
            for(j=0; j<m; j++)
                thisarr1[j] = 0;//가로로 한칸씩 갈 때 마다 thisarr1을 0으로 초기화
            for(k=i; k<n; k++)
            {
                for(j=0; j<m; j++)
                    thisarr1[j] = thisarr1[j] + array[j][k];//array의 j-th row에 있는 값들이 thisarr1에 더해진다
                thissum = 0;
                for(j=0; j<m; j++)
                {
                    thissum = thissum + thisarr1[j];//밑으로 나아가며 하나하나씩 더해준다.
                    if(thissum > maxsum)//그러다가 maxsum의 값을 넘으면 maxsum을 thissum으로 바꿔준다.
                        maxsum = thissum;
                    else if(thissum < 0)//만약 thissum이 0보다 작아진다면 지금의 thissum에다가 다음것을 더하는 것보다 그냥 다음것만 있는게 더 클 것이므로 thissum을 0으로 만들어준다.
                        thissum = 0;
                }
            }
        }
    }
    else if(ver == 2)//시간복잡도가 O(n^4)인 경우
    {
        int** thisarr = (int**)malloc(sizeof(int*)*m);
        for(j=0; j<m; j++)
            thisarr[j] = (int*)malloc(sizeof(int)*n);
        for(j=0; j<m; j++)
            for(i=0; i<n; i++)
            {
                for(finj=j; finj<m; finj++)
                    for(fini=i; fini<n; fini++)
                        thisarr[finj][fini] = 0;//시작점을 이동할 때마다 thisarr를 0으로 초기화 해준다.
                for(finj=j; finj<m; finj++)
                    for(fini=i; fini<n; fini++)//종착점 이동
                    {
                        if(fini==i)
                            thisarr[finj][fini] = array[finj][fini];
                        else
                            thisarr[finj][fini] = thisarr[finj][fini-1] + array[finj][fini];
                    }//시작점의 행부터 마지막행까지 각 행마다 시작점의 열부터 마지막 열까지 이전 열까지의 합 + 자기자신을 thisarr에 저장한다. 그러므로 각행의 마지막 열에는 그 행의 시작 열부터 마지막 열까지의 원소의 합이 저장된다.
                for(fini=i; fini<n; fini++)//종착점 이동
                {
                    thissum = 0;//열이 바뀔때마다 thissum을 0으로 만들어준다.
                    for(finj=j; finj<m; finj++)
                    {
                        thissum = thissum + thisarr[finj][fini];//행별로 저장한 합을 열별로 한번더 합쳐준다
                        if(thissum>maxsum)//더해주는 와중에 thissum이 최대값인 maxsum보다 커질경우 maxsum을 바꿔준다.
                            maxsum = thissum;
                    }
                }
            }
    }
    else if(ver == 1)//시간복잡도가 O(n^6)인 경우
    {
        for(j=0; j<m; j++)
            for(i=0; i<n; i++)//시작점 이동
            {
                for(finj=j; finj<m; finj++)
                    for(fini=i; fini<n; fini++)//종착점 이동
                    {
                        thissum = 0;//종착점을 이동할 떄마다 thissum을 0으로 초기화 해준다.
                        for(tj=j; tj<finj+1; tj++)
                            for(ti=i; ti<fini+1; ti++)
                                thissum = thissum + array[tj][ti];//시작점과 종착점이 이루는 직사각형 안에 있는 원소들을 다 더해준다.
                        if(thissum>maxsum)
                            maxsum = thissum;//thissum이 amxsum보다 커질경우 maxsum을 바꿔준다.
                    }
            }
    }
    duration =1000*((double)(clock() - start))/CLOCKS_PER_SEC;//지금 시각에서 시작한 시각을 빼준다.
    //duration = 1000*((double)(clock() - start))/CLOCKS_PER_SEC;//지금 시각에서 시작한 시각을 빼준다.
    strcat(outfile,filename);//result_ + input file name
    fp = fopen(outfile,"w");
    if(fp == NULL)
    {
        printf("open error!!\n");
        return 1;
    }
    fprintf(fp,"%s\n%d\n%d\n%d\n%d\n%f\n",filename,ver,m,n,maxsum,duration);
    fclose(fp);
    return 0;
}
