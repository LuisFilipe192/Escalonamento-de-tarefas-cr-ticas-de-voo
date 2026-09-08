#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task{
    char nome[50];
    int periodo;
    int deadline;
    int burst;
}task;

int main(int argc, char *argv[]){
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
    else{

    }


    

    fclose(arquivo);

    return 0;
}