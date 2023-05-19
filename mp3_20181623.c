#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

typedef struct NODE{
    int freq;
    short int isleaf;
    char data;
    struct NODE* left;
    struct NODE* right;
    struct NODE* parent;
    char key[150];
    int level;
}node;

void apchuk();//파일을 압축한다
void pulgi();//압축된 파일을 복구한다
void freepo(node*);//tree를 free해준다
char filein[100];
char fileout[100];
int ascii[150] = {0};
node* maker[150];
node* maker2[150]={0};

int main(int trash, char** argv)
{
    strcpy(filein,argv[2]);
    strcpy(fileout,argv[2]);
    if(strcmp("-c",argv[1])==0)
        apchuk();
    else if(strcmp("-d",argv[1])==0)
        pulgi();
    else
    {
        printf("option error!!!\n");
        return 1;
    }
    return 0;
}

void freepo(node* head)
{
    if(head)
    {
        freepo(head->left);
        freepo(head->right);
        free(head);
    }
}

void iter(node* ptr)
{
    char temp[150];
    int i, j, level;
    node* nd;
    for(i=0; i<128; i++)
    {
        maker[i] = maker2[i];
        if(maker2[i]!=NULL)
        {
            level = 0;
            temp[0] = '\0';
            for(nd=maker2[i]; nd->parent!=nd; nd=nd->parent)//트리의 leaf노드에서부터 head노드로 이동한다
            {
                level++;//한단계 올라갈때마다 level을 높여준다
                if(nd->parent->left==nd)
                    strcat(temp,"0");
                else if(nd->parent->right==nd)
                    strcat(temp,"1");
            }
            for(j=0; j<level; j++)
                maker2[i]->key[j] = temp[level-1-j];//저장된 경로를 뒤집어준다
            maker2[i]->key[level] = '\0';
            maker2[i]->level = level;
        }
    }
}

int paltoil(char* str)//8글자의 2진수로 보이는 캐릭터 배열이 들어가면 그에 해당하는 십진수를 리턴해준다
{
    int i, e=1;
    int dab=0;
    for(i=0; i<8; i++)
    {
        dab = dab + e*(str[7-i]-'0');
        e = 2*e;
    }
    return dab;
}

char* iltopal(int c)//십진수 하나를 받아와서 그에 해당하는 이진수 배열을 만들어 리턴한다
{
    char* dab = (char*)malloc(sizeof(char)*10);
    int i,e=128,t=c;
    for(i=0; i<8; i++)
    {
        dab[i] = t/e + '0';
        t = t%e;
        e = e/2;
    }
    dab[8] = '\0';
    return dab;
}

void apchuk()
{
    char il;
    node *temp, *head;
    int buflevel=0, count=0, check=0, total=0, ch, i, min, minmin, mintemp, jong=0;
    char nowkey[150];
    char bufbuf[9];
    char buf[100]={0};
    strcat(fileout,".zz");
    FILE* fpo = fopen(fileout,"w");
    FILE* fpi = fopen(filein,"r");
    if(fpo==NULL||fpi==NULL)
    {
        printf("open error!\n");
        return;
    }
    while((ch=fgetc(fpi))!=EOF)
        (ascii[ch])++;//문자갯수확인
    for(i=0; i<130; i++)
        if(ascii[i]!=0)
            jong++;//문자가 몇종류 오는가 계산
    if(jong==1)//한종류 오는 경우
    {
        for(i=0; i<130; i++)
            if(ascii[i]!=0)
                break;
        fprintf(fpo,"1 %c %d",i,ascii[i]);
        fclose(fpi);
        fclose(fpo);
        return;
    }
    for(ch=0; ch<130; ch++)
    {
        temp = (node*)malloc(sizeof(node));
        temp->freq = ascii[ch];
        temp->data = ch;
        temp->isleaf = TRUE;
        temp->left = NULL;
        temp->right = NULL;
        temp->level = 1;
        maker[ch] = temp;
    }//maker배열 초기화
    while(1)
    {
        if(maker[0]->freq < maker[1]->freq)
        {
            minmin = 0;
            min = 1;
        }
        else
        {
            minmin = 1;
            min = 0;
        }//초기 상황 설정
        for(ch=2; ch<130; ch++)
        {
            if(maker[ch]->freq == 0)//들어오는게 0인경우
                continue;
            if(maker[minmin]->freq==0 && maker[min]->freq==0)
            {
                min = ch;
                continue;
            }//둘다 0인 상태에서 0이 아닌게 들어오는 경우
            else if(maker[minmin]->freq==0)//minmin만 0인 경우
            {
                if(maker[min]->freq > maker[ch]->freq)
                    minmin = ch;
                //ch가 더 작으면 바로 minmin으로 넣어주기
                else
                {
                    minmin = min;
                    min = ch;
                }//ch가 더 크면 하나씩 밀어주기
                continue;
            }
            if(maker[ch]->freq <= maker[minmin]->freq)//0이 없는 경우
            {
                min = minmin;
                minmin = ch;
            }
            else if(maker[ch]->freq <= maker[min]->freq)
            {
                if(maker[ch]->freq == maker[min]->freq)//freq이 같을 때 둘중 level이 낮은 트리를 먼저 붙여준다
                    if(maker[ch]->level < maker[min]->level)
                        min = ch;
                else
                    min = ch;
            }
        }
        if(maker[minmin]->freq==0)//모든 노드를 가지고 tree를 완성하면 한개의 head 노드만 있으므로 이 경우 minmin의 freq이 0이 되므로 이때 종료한다
            break;
        temp = (node*)malloc(sizeof(node));//노드 두개를 자식으로 가지는 새로운 노드를 만든다
        head = temp;
        if(maker[min]->level < maker[minmin]->level)
            temp->level = maker[minmin]->level + 1;
        else
            temp->level = maker[min]->level + 1;
        //바로 위에 있는 노드의 level은 밑에 있는 노드들중 높은 level+1이다
        maker[min]->parent = temp;
        maker[minmin]->parent = temp;//자식노드의 부모를 설정해준다
        temp->freq = maker[min]->freq + maker[minmin]->freq;
        temp->isleaf = FALSE;
        temp->left = maker[minmin];
        temp->right = maker[min];//temp node 정보 초기화
        if(maker[min]->left==NULL && maker[min]->right==NULL)
            maker2[min] = maker[min];
        if(maker[minmin]->left==NULL && maker[minmin]->right==NULL)
            maker2[minmin] = maker[minmin];
        temp->data = maker[min]->data;//임의로 큰쪽 data로 초기화
        maker[min] = temp;
        maker[minmin]->freq = 0;//그리고 작은쪽 freq은 0으로 만들어준다
    }
    head->parent = head;
    fprintf(fpo,"%d\n",jong);
    if(jong>84)//들어오는 문자의 종류가 84개를 기준으로 더 효율적인 적는 방식이 달라진다
        for(i=0; i<128; i++)
            fprintf(fpo,"%d ",ascii[i]);
    else
        for(i=0; i<150; i++)
            if(ascii[i]!=0)
                fprintf(fpo,"%c%d ",i,ascii[i]);//문자의 빈도수를 적어준다.
    fprintf(fpo,"\n");
    iter(head);//tree의 leaf에 key를 설정해주는 함수이다
    fclose(fpi);
    fpi = fopen(filein,"r");
    while((ch=fgetc(fpi))!=EOF)
    {
        strcat(buf,maker[ch]->key);//우선은 버퍼에다가 key를 이어붙여 적는다
        buflevel = buflevel + maker[ch]->level;//그리고 그만큼 버퍼의 level을 늘려준다
        while(buflevel>8)//만약 레벨이 8이 넘어가면 8보다 작아질때까지 
        {
            strncpy(bufbuf,buf,8);//앞에서 8개의 문자를 버퍼의 버퍼에 옮긴다
            buflevel = buflevel - 8;//그리고 그만큼 level을 빼준다
            bufbuf[8] = '\0';
            count++;//총 문자의 개수인 count를 +1 해준다
            strcpy(buf,buf+8);//그러고 전체 버퍼를 앞으로 8칸 땡긴다
        }
    }
    if(buflevel>0)//EOF를 만나서 종료되고 버퍼에 남은 친구들
    {
        for(i=0; i<8-buflevel; i++)
            bufbuf[i] = '0';//패딩처리를 해준다
        strncpy(bufbuf+i,buf,buflevel);
        bufbuf[8] = '\0';
        count++;
    }
    fprintf(fpo,"%d %d\n",count,buflevel);//문자의 총 개수와 마지막 문자의 패딩의 정보
    fclose(fpi);
    fpi = fopen(filein,"r");
    buf[0]='\0';
    buflevel = 0;
    check = 0;
    count = 0;
    while((ch=fgetc(fpi))!=EOF)
    {
        strcat(buf,maker[ch]->key);
        buflevel = buflevel + maker[ch]->level;
        while(buflevel>8)
        {
            strncpy(bufbuf,buf,8);
            buflevel = buflevel - 8;
            bufbuf[8] = '\0';
            fprintf(fpo,"%c",paltoil(bufbuf));//바로 위에 있는 while문의 알고리즘과 똑같이 돌아간다. 대신 count를+1 해주지말고 압축된 문자를 아웃풋 파일에 적어준다 
            strcpy(buf,buf+8);
        }
    }
    if(buflevel>0)//위와 같다
    {
        strcpy(bufbuf,buf);
        for(i=buflevel; i<8; i++)
            bufbuf[i] = '0';
        bufbuf[8] = '\0';
        fprintf(fpo,"%c",paltoil(bufbuf));
    }
    fclose(fpi);
    fclose(fpo);
    freepo(head);
}

void pulgi()
{
    int num, gesu, lastnum, somelevel=0, level, e, count=0, ch, i, jong, min, minmin, total;
    char tempc, check;
    char nowkey[150];
    char chartemp[11];
    char some[180]={'\0'};
    node *new, *tt, *head, *temp;
    strcat(fileout,".yy");
    FILE* fpi = fopen(filein,"r");
    if(fpi==NULL)
    {
        printf("open error!!\n");
        return;
    }
    for(ch=0; ch<130; ch++)
        maker[ch] = NULL;//make배열 초기화
    fscanf(fpi,"%d%*c",&jong);//들어오는 문자의 종류를 받아온다
    if(jong==1)//한종류만 오는 경우
    {
        fscanf(fpi,"%c %d",&tempc,&num);
        FILE* fpt = fopen(fileout,"w");
        for(i=0; i<num; i++)
            fprintf(fpt,"%c",tempc);
        fclose(fpi);
        fclose(fpt);
        return;
    }
    if(jong>84)
        for(i=0; i<128; i++)
            fscanf(fpi,"%d%*c",&ascii[i]);//0부터 127까지의 freq을 받아온다
    else
        for(i=0; i<jong; i++)
        {
            fscanf(fpi,"%c",&tempc);//우선 문자를 읽고
            fscanf(fpi,"%d%*c",&ascii[tempc]);//그에 해당하는 freq을 읽어온다
        }
    for(ch=0; ch<130; ch++)
    {
        temp = (node*)malloc(sizeof(node));
        temp->freq = ascii[ch];
        temp->data = ch;
        temp->isleaf = TRUE;
        temp->left = NULL;
        temp->right = NULL;
        maker[ch] = temp;//maker 배열 초기화
    }//초기화
    while(1)//make tree
    {
        if(maker[0]->freq < maker[1]->freq)
        {
            minmin = 0;
            min = 1;
        }
        else
        {
            minmin = 1;
            min = 0;
        }//초기 상태 설정
        for(ch=2; ch<130; ch++)
        {
            if(maker[ch]->freq == 0)//들어오는게 0인경우
                continue;
            if(maker[minmin]->freq==0 && maker[min]->freq==0)
            {
                min = ch;
                continue;
            }//둘다 0인 상태에서 0이 아닌게 들어오는 경우
            else if(maker[minmin]->freq==0)//minmin만 0인 경우
            {
                if(maker[min]->freq > maker[ch]->freq)
                    minmin = ch;
                //ch가 더 작으면 바로 minmin으로 넣어주기
                else
                {
                    minmin = min;
                    min = ch;
                }//ch가 더 크면 하나씩 밀어주기
                continue;
            }
            if(maker[ch]->freq <= maker[minmin]->freq)
            {
                min = minmin;
                minmin = ch;
            }
            else if(maker[ch]->freq <= maker[min]->freq)
            {
                if(maker[ch]->freq == maker[min]->freq)//freq이 같을 때 둘중 level이 낮은 트리를 먼저 붙여준다
                    if(maker[ch]->level < maker[min]->level)
                        min = ch;
                else
                    min = ch;
            }
        }
        if(maker[minmin]->freq==0)//모든 노드를 가지고 tree를 완성하면 한개의 head 노드만 있으므로 이 경우 minmin의 freq이 0이 되므로 이때 종료한다
            break;
        temp = (node*)malloc(sizeof(node));
        head = temp;
        if(maker[min]->level < maker[minmin]->level)
            temp->level = maker[minmin]->level + 1;
        else
            temp->level = maker[min]->level + 1;
        //바로 위에 있는 노드의 level은 밑에 있는 노드들중 높은 level+1이다
        maker[min]->parent = temp;
        maker[minmin]->parent = temp;//자식노드의 부모를 설정해준다
        temp->freq = maker[min]->freq + maker[minmin]->freq;
        temp->isleaf = FALSE;
        temp->left = maker[minmin];
        temp->right = maker[min];
        if(maker[min]->left==NULL && maker[min]->right==NULL)
            maker2[min] = maker[min];
        if(maker[minmin]->left==NULL && maker[minmin]->right==NULL)
            maker2[minmin] = maker[minmin];
        temp->data = maker[min]->data;//임의로 큰쪽 data로 초기화
        maker[min] = temp;
        maker[minmin]->freq = 0;
    }
    head->parent = head;
    iter(head);
    fscanf(fpi,"%d %d%*c",&gesu, &lastnum);//문자의 전체 개수와 마지막 글자의 패딩에 관한 정보를 읽어온다
    FILE* fpo = fopen(fileout,"w");
    if(fpo==NULL)
    {
        printf("open error!\n");
        return;
    }
    tt = head;
    check = 0;
    for(i=0; i<gesu; i++)    
    {
        ch = fgetc(fpi);
        count++;
        if(count == gesu)//마지막 글자일때
        {
            strcpy(chartemp,iltopal(ch));
            chartemp[lastnum] = '\0';//뒤에 붙어있는 0들을 없애준다
            strcat(some, chartemp);//그러고 some배열에 붙여준다
            somelevel = somelevel + strlen(chartemp);
        }
        else
        {
            strcat(some,iltopal(ch));
            somelevel = somelevel + 8;
        }//들어오 캐릭터에 대한 이진수를 some 배열에 붙여주고 level을 + 해준다
        while(somelevel>100)
        {
            for(level=0; ;level++)
            {
                if(some[level]=='0')
                    tt = tt->left;
                else if(some[level]=='1')
                    tt = tt->right;//head에서 시작해서 0이면 왼쪽으로 읻ㅇ하고 1이면 오른쪽으로 이동하는데
                if(tt->isleaf==TRUE)//그러던중 리프노드를 만나면
                {
                    fprintf(fpo,"%c",tt->data);//그 노드의 데이터를 기록해준다
                    strcpy(some,some+tt->level);
                    somelevel = somelevel - tt->level;
                    tt = head;
                    break;
                }
            }
        }
    }
    while(somelevel>0)//남아있는 some배열의 원소들 처리
    {
        for(level=0; ;level++)
        {
            if(some[level]=='0')
                tt = tt->left;
            else if(some[level]=='1')
                tt = tt->right;//바로 위의 반복문의 알고리즘과 똑같다
            if(tt->isleaf==TRUE)
            {
                fprintf(fpo,"%c",tt->data);
                strcpy(some,&some[tt->level]);
                somelevel = somelevel - tt->level;
                tt = head;
                break;
            }
        }
    }
    fclose(fpo);
    fclose(fpi);
    freepo(head);
}
