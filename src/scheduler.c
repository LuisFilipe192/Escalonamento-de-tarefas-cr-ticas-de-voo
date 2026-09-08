#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task{
    char nome[50];
    int periodo;
    int deadline;
    int burst;
    struct task *next;
}task;

int main(int argc, char *argv[]){
    task *head = NULL;

    int tempo_total;

    FILE *arquivo;

    if(argc != 3){
        fprintf(stderr,"Erro: número incorreto de argumentos.\n");
        return 1;
    }

    if (strcmp(argv[1],"rate") != 0 && strcmp(argv[1],"edf") != 0) {
        fprintf(stderr,"Erro: algoritmo inválido.\n");
        return 1;
    }

    arquivo = fopen(argv[2],"r");

    if(arquivo == NULL){
        fprintf(stderr,"Erro: não foi possível abrir o arquivo.\n");
        return 1;
    }

    if(fscanf(arquivo,"%d",&tempo_total) != 1){
        fprintf(stderr,"tempo total inválido.\n");
        return 1;
    }

    if(tempo_total<=0){
        fprintf(stderr,"tempo total deve ser positivo.\n");
        return 1;
    }


    task *novo = (task*)malloc(sizeof(task));

    if(novo == NULL){
        fprintf(stderr, "Erro: falha ao alocar memória.\n");
        fclose(arquivo);
        return 1;
    }

    if (fscanf(arquivo,"%49s %d %d %d", novo->nome, &novo->periodo,&novo->deadline,&novo->burst) != 4){
        fprintf(stderr,"Erro: arquivo com argumentos inválidos.\n");
        fclose(arquivo);
        free(novo);
        return 1;
    }

    if(novo->periodo <= 0 || novo->deadline <= 0 || novo->burst <= 0){
        fprintf(stderr,"Erro: os valores da tarefa devem ser positivos.\n");
        free(novo);
        fclose(arquivo);
        return 1;
    }

    if(novo->burst > novo->deadline){
        fprintf(stderr,"Erro: burst não pode ser maior que o deadline.\n");
        free(novo);
        fclose(arquivo);
        return 1;
    }

    if(novo->deadline > novo->periodo){
        fprintf(stderr,"Erro: deadline não pode ser maior que o período.\n");
        free(novo);
        fclose(arquivo);
        return 1;
    }

    novo->next = NULL;
    head = novo;


    fclose(arquivo);

    return 0;
}