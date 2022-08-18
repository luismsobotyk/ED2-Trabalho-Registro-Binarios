#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Código, o Nome e o Saldo da Conta
typedef struct Correntista{
	char nome[20];
	char codigo[10];
	float saldo;
}Correntista;
typedef struct{
	Correntista dado;
	struct no* next;
  	struct no* prev;
}tno;
typedef struct{
  tno* first;
  tno* last;
  int size;
}tList;

void start_list(tList *list);
void add_node(tList *list, Correntista a);
void print_list(tList *list);
int exist(tList *list, char n[10]);
int credito(tList *list, char n[10], float val);
int debito(tList *list, char n[10], float val);

int main(void){
	tList minha_lista;
	start_list(&minha_lista);
	if(&minha_lista == NULL){
		return 1; // Erro na criação da lista. Encerra o programa.
	}
	FILE *bin;
	int  tamanho = 0; 
	if((bin = fopen("data.bin","rb")) == NULL){
        printf("Erro ao abrir arquivo!");
        return 0;
    }
    Correntista correntista_leitura;
    //printf("%d\n\n", sizeof(Correntista));
    if(!feof(bin) && !ferror(bin)){
		while(fread(&correntista_leitura, sizeof(Correntista), 1, bin)==1){
			add_node(&minha_lista, correntista_leitura);
			printf("Correntista %s carregado![%s, %s, %f]\n", correntista_leitura.nome, correntista_leitura.nome, correntista_leitura.codigo, correntista_leitura.saldo);		
			if(feof(bin)) break;
		}
	}
    fclose(bin);
    
    int op=0;
    while(1){
 		do{
 			printf("MENU\n1- Novo Correntista\n2- Registrar Operacao\n3- Listar todos Correntistas\n4- Sair.\n");
 			scanf("%d", &op);
		}while(op<1 || op>4);
	
		switch(op){
			case 1:;
				char codigo[10];
				printf("Codigo[10]: ");
				scanf("%s", &codigo);
				
				if(exist(&minha_lista, codigo)){
					printf("O codigo ja esta em uso.\n");
				}else{
					Correntista correntista;
					char nome[20];
					float saldo;
					printf("Nome[20]: ");
					scanf("%s", &nome);
					printf("Saldo [float]: ");
					scanf("%f", &saldo);
					strcpy(correntista.nome, nome);
					strcpy(correntista.codigo, codigo);
					correntista.saldo= saldo;
					add_node(&minha_lista, correntista);
				}
			break;
			case 2:;
					char codigo2[10];
					printf("Digite o codigo do correntista: ");
					scanf("%s", &codigo2);
					if(exist(&minha_lista, codigo2)){
						int operacao=3;
						do{
							printf("1- Registrar Debito\n2- Registrar Credito\n");
							scanf("%d", &operacao);
						}while(operacao<1 || operacao>2);
						if(operacao==1){
							float valor;
							printf("Valor a ser debitado [float]: R$");
							scanf("%f", &valor);
							if(debito(&minha_lista, codigo2, valor)){
								printf("Valor debitado com sucesso!\n\n");
							}else{
								printf("Erro ao debitar o valor!\n\n");
							}
						}else if(operacao==2){
							float valor;
							printf("Valor a ser creditado [float]: R$");
							scanf("%f", &valor);
							if(credito(&minha_lista, codigo2, valor)){
								printf("Valor creditado com sucesso!");
							}else{
								printf("Erro ao creditar o valor!");
							}
						}else{
							printf("Erro!");
						}
						
					}else{
						printf("O codigo infomardo nao existe.\n");
					}
				
			break;
			case 3:;
				print_list(&minha_lista);
			break;
			case 4:;
				if((bin = fopen("data.bin","wb")) == NULL){
        			printf("Erro ao abrir arquivo para gravacao!");
        			return 0;
    			}
    			if(minha_lista.size==0){
    				fclose(bin);
					return 0;
				}
				
				tno *current;
  				current = minha_lista.first;
  				int i= 0;
				while(i<minha_lista.size) {
    				Correntista corr;
					strcpy(corr.nome, current->dado.nome);
					strcpy(corr.codigo, current->dado.codigo);
					corr.saldo= current->dado.saldo;
					fwrite(&corr, 1, sizeof(corr), bin);
					current = current->next;
  					i++;
  				}
				return 0;
			break;
		}
	} 	
}

void start_list(tList *list) {
  list->first = NULL;
  list->last = NULL;
  list->size = 0;
}
void add_node(tList *list, Correntista a) {
	tno *no = malloc(sizeof(tno));
  	tno *prev;
  	if(no!= NULL){
  		if (list->first == NULL && list->last == NULL) {
			list->first = list->last = no;
    		strcpy(no->dado.nome, a.nome);
			strcpy(no->dado.codigo, a.codigo);
			no->dado.saldo= a.saldo;
			list->size++;
  		} else {
  			no->next = list->first;
			no->prev = list->last;
			strcpy(no->dado.nome, a.nome);
			strcpy(no->dado.codigo, a.codigo);
			no->dado.saldo= a.saldo;
			list->last->next= no;
			list->last = no;
			list->size++;
  		}
  	}else{
  		printf("Erro no malloc");	
	}
}

int exist(tList *list, char n[10]){
	tno *current;
	current = list->first;
	int i=0;
	while(i<list->size){
		if(!strcmp(n, current->dado.codigo)){
			return 1;
		}
		current = current->next;
		i++;
	}	
	return 0;
}

void print_list(tList *lista) {
  printf("Imprimindo Correntistas:\n");
  tno *current;
  current = lista->first;
  int i= 0;
  while(i<lista->size) {
    printf("\tNome: %s\n\tSaldo: R$ %f\n\n", current->dado.nome, current->dado.saldo);
	current = current->next;
  	i++;
  }
}

int credito(tList *list, char n[10], float val){	
	tno *current;
	current = list->first;
	int i=0;
	while(i<list->size){
		if(!strcmp(n, current->dado.codigo)){
			current->dado.saldo+= val;
			return 1;
		}
		current = current->next;
		i++;
	}	
	return 0;
}

int debito(tList *list, char n[10], float val){	
	tno *current;
	current = list->first;
	int i=0;
	while(i<list->size){
		if(!strcmp(n, current->dado.codigo)){
			current->dado.saldo-= val;
			return 1;
		}
		current = current->next;
		i++;
	}	
	return 0;
}
