//#include<stdio.h>
//#include<string.h>
#include"TokenStack.hpp"

char buff[1000];
char sys[1000];
char var[100][20];
int vars=0;

int load_file(char* name)
{
	FILE* arch = fopen(name,"r");
	int i=0,j=0;
	char a,d;

	while(fscanf(arch,"%c",&a)!=EOF)
	{
		if(a==' ' || a=='\n' || a=='\t')
			continue;
		if(a=='(' || a=='{')
		{
			j--;
			buff[i++]='(';
		}
		else if(a==')' || a=='}')
		{
			j++;
			buff[i++]=')';
		}
		else
			buff[i++]=a;
	}
	buff[i]='\0';

	fclose(arch);

	return j;
}

int check(int i)
{
	if(buff[i]==';' && (buff[i+1]==')' || buff[i+1]=='\0'))
		return 10;

	if(buff[i]=='t' && buff[i+1]=='h' && buff[i+2]=='e' && buff[i+3]=='n')
		return 6;
	if(buff[i]=='t' && buff[i+1]=='i' && buff[i+2]=='m' && buff[i+3]=='e' &&
	buff[i+4]=='s')
                return 7;
	if(buff[i]=='>' && buff[i+1]=='>')
		return 4;
	if(buff[i]=='<' && buff[i+1]=='<')
                return 5;
	if(buff[i]=='+' || buff[i]=='-' || buff[i]=='*' || buff[i]=='/'
	|| buff[i]=='<' || buff[i]=='>' || buff[i]==';' || buff[i]==',' ||
	buff[i]=='(' ||  buff[i]==')')
		return 0;
	if(buff[i]=='!' && buff[i+1]=='=')
		return 1;
	if(buff[i]=='=')
	{
		if(buff[i+1]=='=')
	                return 2;
		else
			return 3;
	}
	if(buff[i]>='a' && buff[i]<='z')
		return 8;
	if(buff[i]>='0' && buff[i]<='9')
                return 9;
	return 11;
}

void search(char name[])
{
	int i,j=-1;
	for(i=0;i<vars;i++)
		if(strcmp(name,&(var[i][0]))==0)
		{
			j=i;
			break;
		}
	
	if(j==-1)
	{
		strcpy(&(var[vars][0]),name);
		j=vars;
		vars++;
	}
	
	sprintf(name,"%i",j);
}

int load_buff()
{
	int i,j,k;
	i=0;
	j=0;
	char mini[20];

	while(buff[i]!='\0')
	switch(check(i))
	{
		case 0:
			sys[j++]=buff[i++];
			break;
		case 1:
			sys[j++]='!';
			i+=2;
			break;
		case 2:
			sys[j++]='=';
                        i+=2;
                        break;
		case 3:
                        sys[j++]=':';
                        i+=1;
                        break;
		case 4:
			i+=2;
			sys[j++]='a';
			sys[j++]='-';
			sys[j++]='1';
			sys[j++]='.';
			sys[j++]='%';
			break;
		case 5:
                        i+=2;
                        sys[j++]='a';
                        sys[j++]='1';
                        sys[j++]='.';
                        sys[j++]='%';
                        break;
		case 6:
			i+=4;
			sys[j++]='?';
			break;
		case 7:
			i+=5;
			sys[j++]='@';
			break;
		case 8:
			sys[j++]='x';
			k=0;
                        while(buff[i]>='a' && buff[i]<='z')
                                 mini[k++]=buff[i++];
                        mini[k]='\0';
			search(mini);
			k=0;
			while(mini[k]!='\0')
				sys[j++]=mini[k++];
			sys[j++]='.';
			break;
		case 9:
			sys[j++]='a';
			while(buff[i]>='0' && buff[i]<='9')
				 sys[j++]=buff[i++];
			sys[j++]='.';
			break;
		case 10:
			i++;
			break;
		default:
			sys[j]='\0';
			return 0;
	}
	sys[j]='\0';
	return 1;
}


int main(int argc, char *argv[])
{
	int a;
	Parser *b;

	if((a=load_file(argv[1]))!=0)
	{
		if(a<0)
			printf("Falta un ).\n");
		else
			printf("Falta un (.\n");
	}
	else if((a=load_buff()))
	{
		b=new Parser(sys);
		b->run();
	}
	else
		printf("Bad syntax.\n");
}
