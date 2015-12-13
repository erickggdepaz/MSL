#include<stdio.h>
#include<stdlib.h>
#include<string.h>

class Token
{
public:
	char type;
	long int value;
	Token *next;

	Token(char type_arg,Token* next_arg,long int value_arg=0)
	{
		type=type_arg;
		next=next_arg;
		value=value_arg;
	}

	static int precedence(char type)
	{
		switch(type)
		{
		case ';':
		case ',':
			return 1;
		case '?':
		case '@':
			return 2;
		case ':':
		case '%':
			return 3;
		case '=':
		case '!':
			return 2;
		case '>':
		case '<':
			return 2;
		case '+':
		case '-':
			return 6;
		case '*':
		case '/':
			return 7;
		case '(':
			return -1;
		case ')':
			return -1;
		}
		return 0;
	}

	
};

class TokenStack
{

public:
	Token* TOP;

	TokenStack(Token* top=NULL)
	{
		TOP=top;
	}

	void setn(char type, int value=0)
	{
		TOP = new Token(type,TOP,value);
	}

	int set(char type, int value=0)
	{
		if(TOP && Token::precedence(TOP->type)>Token::precedence(type)
			&& type!='(')
			return 0;
		else
		{
			TOP = new Token(type,TOP,value);
			return 1;
		}
	}
	
	Token* top()
	{
		return TOP;
	}

	void change(Token* top)
	{
		TOP=top;
	}

	Token* pop()
	{
		Token *aux=TOP;
		TOP=TOP->next;
		return aux;
	}

	void setcollection(TokenStack* ini)
	{
		Token* aux;

		while(ini->top() && (ini->top())->type!='(')
		{
			aux = ini->pop();
			TOP = new Token(aux->type,TOP,aux->value);
			delete aux;
		}

		if(ini->top() && (ini->top())->type=='(')
			delete ini->pop();
	
	}

	


	~TokenStack()
	{
	}
	
};

class Parser
{

	TokenStack* TOP;
	int var[100];


	int eval(TokenStack *in_st, int flag=0)
	{
		Token *in = in_st->pop(),*aux;
		TokenStack *auxSt;
		int a,b,f;

		if(!in)
		{
			printf("Fatal error: Verify pre-Parser syntax.\n");
			return 0;
		}
		
		if(flag)
		switch(in->type)
		{
			case '+':
			case '-':
			case '*':
			case '/':
			case '<':
			case '>':
			case '=':
			case '!':
			case ':':
			case '%':
			case '?':
			case '@':
			case ';':
			case ',':
				b = eval(in_st,1);
                                a = eval(in_st,1);
				return 0;
			default:
				return 0;

		}
		

		switch(in->type)
		{
			case 'a':
				return in->value;
			case 'x':
				return var[in->value];
			case '+':
				b = eval(in_st);
				a = eval(in_st);
				return a+b;
			case '-':
				b = eval(in_st);
                                a = eval(in_st);
                                return a-b;
			case '*':
				b = eval(in_st);
                                a = eval(in_st);
                                return a*b;
			case '/':
                                b = eval(in_st);
                                a = eval(in_st);
                                return a/b;
			case ';':
			case ',':
				aux = in_st->top();
				auxSt = new TokenStack(aux);
				b = eval(auxSt,1);
                                a = eval(auxSt);
				delete auxSt;
				b = eval(in_st);
				a = eval(in_st,1);
                                return b;
			case '=':
				b = eval(in_st);
                                a = eval(in_st);
				return (a==b);
			case '!':
                                b = eval(in_st);
                                a = eval(in_st);
                                return (a!=b);
			case '<':
                                b = eval(in_st);
                                a = eval(in_st);
                                return (a<b);
			case '>':
                                b = eval(in_st);
                                a = eval(in_st);
                                return (a>b);
			case ':':
				b = eval(in_st);
				aux = in_st->top();
				if(aux && aux->type=='x')
				{
					aux->type='a';
                                	a = eval(in_st);
					var[a]=b;
					aux->type='x';
				}
				else
					printf("Error: It's not possible assign a value for a constant.\n");
				return b;
			case '%':
				aux = in_st->top();
                                if(aux && aux->type=='x')
                                {
                                        aux->type='a';
	                                b = eval(in_st);
					aux->type='x';
					f=1;
				}
				else
				{
					b = eval(in_st);
					f=0;
				}
				a = eval(in_st);

				if(f)
				{
 	                        	if(a<0)
						scanf("%i",&var[b]);
					else
						printf("%i\n",var[b]);
				}
				else
					printf("%i\n",b);

				return b;
				
			case '@':
				aux=in_st->top();
				auxSt = new TokenStack(aux);
				b = eval(auxSt,1);
				a = eval(auxSt);
				in_st->change(auxSt->top());
				delete auxSt;
				while(a)
				{
					auxSt = new TokenStack(aux);
					b = eval(auxSt);
					a = eval(auxSt);
					delete auxSt;
				}
				return 0;
			case '?':
                                aux=in_st->top();
                                auxSt = new TokenStack(aux);
                                b = eval(auxSt,1);
                                a = eval(auxSt);
                                in_st->change(auxSt->top());
                                delete auxSt;
                                if(a)
                                {
                                        auxSt = new TokenStack(aux);
                                        b = eval(auxSt);
                                        delete auxSt;
                                }
                                return 0;
		}

		printf("Fatal error: Symbol |%c| is unknown.\n",in->type);
		return 0;
	}

public:
	Parser(char in[])
	{
		TokenStack *simb = new TokenStack;
		TokenStack *out = new TokenStack; 

		int i,j;


		while(in[i]!='\0')
		{
			if(Token::precedence(in[i])==0)
			{	
				j=i+1;
				while(in[j]!='.')
					j++;
				in[j]='\0';
				out->setn(in[i],atoi(&(in[i+1])));
				i=j+1;
				continue;
			}
			else if(!simb->set(in[i]))
			{
				out->setcollection(simb);
				if(in[i]!=')')
					simb->setn(in[i]);

			}
			i++;
		}
		out->setcollection(simb);
		delete simb;

		TOP=out;

		Token *aux = out->top();
	
		while(aux)
		{
			printf("%c",aux->type);
			aux=aux->next;
		}
		printf("\n");

	}

	void run()
	{
		eval(TOP);
	}

	~Parser()
	{
		delete TOP;
	}


};

/*
int main()
{
	char in[] = "x0.:a0.;x1.:a0.;(x0.<a4.)@(x0.:x0.+a1.;x1.:x1.+a2.);a1.%x1.";

	//"a-1.%a0.;a-1.%a1.;x2.:(x0.+x1.);a1.%a2.";

	// x0.:a0.;(x0.<a4.)@x0.:x0.+a1.;x0.

	Parser *a = new Parser(in);
	a->run();
	delete a;

	return 0;
}
*/
