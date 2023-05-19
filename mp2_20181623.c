#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int *arr, *temp, *stack;
int num, top = -1;
void swap(int*, int*);
void bubble();
int partition(int, int);
void quick(int, int);
void mergepart(int, int);
void merge(int, int);

int main(int gesu, char** str)
{
    int ver = atoi(str[2]);
    char filename[1000];
    clock_t start;
    double duration;
    strcpy(filename, str[1]);
    if(ver<1 || ver>4)
    {
        printf("ibryeok jalmotham!!!!(ex: 1,2,3,4)\n");
        return 1;
    }
    FILE* fp = fopen(filename,"r");
    if(fp == NULL)
    {
        printf("open error!\n");
        return 1;
    }
    int right, left, il, ee, sam, sa, pivot, i, j, s, t, l, ls, lf, r, rs, rf, tt;
    fscanf(fp,"%d",&num);
    arr = (int*)malloc(sizeof(int)*num);//배열을 읽어서 저장하는 역할
    temp = (int*)malloc(sizeof(int)*num);//merge에서 사용
    stack = (int*)malloc(sizeof(int)*num);//stack
    for(i=0; i<num; i++)
        fscanf(fp,"%d",arr+i);
    fclose(fp);
    if(ver == 1)//bubble sort
    {
        start = clock();
        bubble();
        duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    }
    else if(ver == 2)//quick sort
    {
        start = clock();
        quick(0,num-1);
        duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    }
    else if(ver == 3)//merge sort
    {
        start = clock();
        merge(0, num-1);
        duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    }
    else if(ver == 4)//hyomin sort 시간복잡도 줄일려고 함수호출 안하고 main에다가 씀
    {
        start = clock();
        stack[++top] = 0;//push함수를 호출하지 않고 main에서 바로 push해줌
        stack[++top] = num - 1;
        while(top != -1)
        {
            right = stack[top--];//pop함수를 호출하지 않고 main에서 바로 pop해줌
            left = stack[top--];//stack에서 시작점과 끝점을 빼온다
            il = left + 1;
            ee = (left+right)/2;
            sam = ee + 1;
            sa = right - 1;
            if((arr[il]>arr[ee])&&(arr[ee]>arr[sam])&&(arr[sam]>arr[sa]))//내림차순 판단
                merge(left, right);//내림차순이면 quick에서 merge로 전환
            else
            {
                pivot = left;//partition함수 사용하지 않고 직접 
                for(i=left; i<right; i++)
                    if(arr[i]<arr[right])//피봇으로 정한 값보다 작은 수라면 피봇위치에 있는 값과 스왑해주고 피봇을 한칸 뒤로
                    {
                        tt = arr[i];//swap함수도 사용하지 않고 직접
                        arr[i] = arr[pivot];
                        arr[pivot++] = tt;
                    }
                tt = arr[pivot];
                arr[pivot] = arr[right];
                arr[right] = tt;//제일 마지막에 피봇의 위치에 있는 값과 제일 오른쪽에 있는 값을 바꿔주므로 피봇으로 정한값이 피봇 위치에 오게된다.

                if(left < pivot-1)
                {
                    stack[++top] = left;
                    stack[++top] = pivot - 1;
                }
                if(right > pivot+1)
                {
                    stack[++top] = pivot + 1;
                    stack[++top] = right;
                }
				//새로운 시작점과 끝점을 스택에다가 넣어준다.
            }
        }
        duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    }
    char out[1000] = "result_";
    out[7] = ver + '0';
    out[8] = '_';
    out[9] = '\0';
    strcat(out,filename);
    fp = fopen(out,"w");
    if(fp == NULL)
    {
        printf("open error!!\n");
        return 1;
    }
    fprintf(fp,"%s\n",filename);
    fprintf(fp,"%d\n%d\n%f\n",ver,num,duration);
    for(i=0; i<num; i++)
        fprintf(fp,"%d ",arr[i]);
    fprintf(fp,"\n");
    fclose(fp);
    free(arr);
    free(temp);
    free(stack);
    return 0;
}

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void bubble()
{
    int i,j;
    for(i=0; i<num-1; i++)
        for(j=1; j<num-i; j++)
            if(arr[j-1]>arr[j])
                swap(arr+j-1,arr+j);
}

int partition(int left, int right)
{
    int i, pivot = left;
    for(i=left; i<right; i++)
        if(arr[i]<arr[right])
            swap(arr+i, arr+(pivot++));
    swap(arr+pivot, arr+right);
    return pivot;
}

void quick(int left, int right)
{
    int pivot;
    if(right-left > 0)
    {
        pivot = partition(left, right);
        quick(left, pivot-1);
        quick(pivot+1, right);
    }
}

void mergepart(int left, int right)
{
    int i,tl,tr,mid = (left+right)/2;
    for(i=left; i<=right; i++)
        temp[i] = arr[i];
    tl = left;
    tr = mid+1;
    i = left;

    while((tl<=mid) && (tr<=right))
    {
        if(temp[tl]<temp[tr])
            arr[i++] = temp[tl++];
        else
            arr[i++] = temp[tr++];
    }
    while(tl<=mid)
        arr[i++] = temp[tl++];
    while(tr<=right)
        arr[i++] = temp[tr++];
}

void merge(int left, int right)
{
    int mid = (left+right)/2;;
    if(left < right)
    {
        merge(left, mid);
        merge(mid+1, right);
        mergepart(left, right);
    }
}
