#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//processlerin max. ihtiya� duyduklar�,tahsis edilen,kalan ihtiya�lar� ve i�lem bitme durumlar� 
//struct i�indeki de�i�kenlerle tan�mlanm��t�r.
typedef struct {
	int *NEED,*ALLOCATED,*stillNEED,finished;
}PROCESS;

// Processlerin h�la ihtiya� duyduklar� kaynaklar ile eldeki kaynaklar�n kar��la�t�r�lmas� yap�lacakt�r.
void check(PROCESS *process,int n,int p,int *res,int *totalUse,int *available)
{
	int k,l,sayac=0,ax=0,psayac=0;
	while(ax<p*p) // Kontrol mukabilinden n*n kadar tarama yap�lmaktad�r.
	{	
		for(l=0;l<p;l++)
		{//E�er process tamamlanmam��sa, eldeki kaynaklar processin ihtiyac�n� kar��l�yor mu diye kontrol edilmektedir!
			if(process[l].finished!=1)
			{		
				for (k=0;k<n;k++)
				{
					if(available[k]>=process[l].stillNEED[k]) //processin t�m kaynak ihtiya�lar�n� sorgula
					sayac++;			
				}
				if(sayac!=n) //t�m kaynaklar tim ihtiya�lara cevap veriyor mu?
				{
					sayac=0;
					continue;
				} 
				else //kaynaklar yetiyorsa �nce ihtiya� duyulan kaynaklar� tahsis et sonra t�m tahsis edilenleri al..
				{
					for(k=0;k<n;k++) 
					{
						available[k]-=process[l].stillNEED[k];
						available[k]+=process[l].NEED[k];						
					}
					process[l].finished=1;
					printf("Process %d finished in safe!\n",l);
					sayac=0;
				}			
			}
			else
			{
				ax++;
				continue;
			}			
		}	ax++;
	}
	for(l=0;l<p;l++) //t�m processler g�venli olark tamamlanm�� m�?
	{
		if(process[l].finished==1)
		{		
			psayac+=1;
		}
	}
	if(psayac==p) printf("\nSAFE STATE!");
	else printf("\nUNSAFE STATE!");	
}
int main(void)
{
	int i,n,p,j,*res;
	printf("Total System Resources: "); // Toplam sistem kaynaklar� ka� adet? Kullan�c�ya ba�l�!
	scanf("%d",&n);
	res=(int*)malloc(sizeof(int)*n);
	char ch='A';
	for(i=0;i<n;i++) //kaynak giri�leri
	{
		printf("%c resource: ",ch++);
		scanf("%d",&res[i]);
	}
	printf("How many process: "); //Ka� proses olacak
	scanf("%d",&p);
	PROCESS *process;
	process=(PROCESS*)malloc(sizeof(PROCESS)*p);
	for(i=0;i<p;i++) //her proses i�in max. ihtiya�,tahsis edilen, hala ihtiya� duyulan kaynak say�s�na g�re dinamik memory allocation.. 
	{
		process[i].NEED=(int*)malloc(sizeof(int)*n);
		process[i].ALLOCATED=(int*)malloc(sizeof(int)*n);
		process[i].stillNEED=(int*)malloc(sizeof(int)*n);
	}	
	printf("What process need (max) in order to work\n"); //her prosesin ihtiya� duydu�u max kaynak..
	ch='A';
	for(i=0;i<p;i++)
	{
		for(j=0;j<n;j++)
		{		
		printf("P%d %c need: ",i,ch++);
		scanf("%d",&process[i].NEED[j]);
		}
		printf("\n");
		ch='A';
	}
	printf("Currently Allocated\n"); //her prosese tahsis edilen kaynak..
	ch='A';
	for(i=0;i<p;i++)
	{
		for(j=0;j<n;j++)
		{		
		printf("P%d %c allocated: ",i,ch++);
		scanf("%d",&process[i].ALLOCATED[j]);
		}
		printf("\n");
		ch='A';
	}
	printf("AVAILABLE RESOURCE NOW\n"); //Hesaplanan eldeki ula��labilir kaynaklar..Dizide saklanmaktad�r.
	int totalUse[n],available[n],x=0;	
	memset(totalUse, 0, sizeof(totalUse));
	for(i=0;i<n;i++)
	{				
		for(j=0;j<p;j++)
		{			
			totalUse[x]+=process[j].ALLOCATED[i];			
		}
		x++;		
	}
	ch='A';
	for(i=0;i<n;i++)
	{	
		available[i]=res[i]-totalUse[i];		
		printf("Available %c: %d\n",ch++,res[i]-totalUse[i]);
	}
	i=0;
	
	while(i<n) //E�er kullan�lmak istenen kaynaklardan fazla ise yetersi kaynak uyar�s�..
	{	
		if(available[i]<0)
		{		
			printf("NOT ENOUGH RESOURCES!");
			exit(1);
		}
		i++;
	}	
	printf("\n"); //Hala ihtiya� duyulan kaynak say�s�...
	ch='A';
	for(i=0;i<p;i++)
	{				
		for(j=0;j<n;j++)
		{
			process[i].stillNEED[j]=process[i].NEED[j]-process[i].ALLOCATED[j];
			if(process[i].stillNEED[j]<=0) process[i].stillNEED[j]=0;
			printf("P%d still needs %c resource : %d\n",i,ch++,process[i].stillNEED[j]);
		}
	ch='A';
	printf("\n");			
	}
	//Eldeki kaynaklara g�re deadlock olu�up olu�mad��� kontrol edilecek..
	check(process,n,p,res,totalUse,available);
	getch();	
}
