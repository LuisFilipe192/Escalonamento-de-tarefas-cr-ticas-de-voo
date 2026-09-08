#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task{
    char nome[50];
    int periodo;
    int deadline;
    int burst;

    int restante;
    int proxima_chegada;
    int deadline_absoluto;

    int completas;
    int perdidas;
    int killed;

    int ativa;

    struct task *next;
}task;

int main(int argc, char *argv[]){
    task *head = NULL;

    int tempo_total;

    if(argc != 3){
        fprintf(stderr,"Erro: número incorreto de argumentos.\n");
        return 1;
    }

    if (strcmp(argv[1],"rate") != 0 && strcmp(argv[1],"edf") != 0) {
        fprintf(stderr,"Erro: algoritmo inválido.\n");
        return 1;
    }
    
    FILE *arquivo;

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


    while(1){
        task *novo = malloc(sizeof(task));

        if(novo == NULL){
            fprintf(stderr,"Erro: falha ao alocar memória.\n");
            fclose(arquivo);
            return 1;
        }

        int resultado = fscanf(arquivo,"%49s %d %d %d", novo->nome,&novo->periodo,&novo->deadline,&novo->burst);

        if(resultado == EOF){
            free(novo);
            break;
        }

        if(resultado != 4){
            fprintf(stderr,"Erro: arquivo malformado.\n");
            free(novo);
            fclose(arquivo);
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

        novo->restante = novo->burst;
        novo->proxima_chegada = 0;
        novo->deadline_absoluto = novo->deadline;

        novo->completas = 0;
        novo->perdidas = 0;
        novo->killed = 0;

        novo->ativa = 1;

        novo->next = NULL;

        if(head == NULL){
            head = novo;
        }
        else{
            task *atual = head;

            while(atual->next != NULL){
                atual = atual->next;
            }

            atual->next = novo;
        }
    }


    FILE *saida;

    if(strcmp(argv[1], "rate") == 0){
        saida =fopen("rate_lfass.out","w");
    }
    else{
        saida =fopen("edf_lfass.out","w");
    }

    if(saida == NULL){
        fprintf(stderr,"Erro: não foi possível criar o arquivo de saída.\n");
        fclose(arquivo);
        return 1;
    }

    if(strcmp(argv[1],"rate") == 0){
        fprintf(saida,"EXECUTION BY RATE\n");
    } else {
        fprintf(saida,"EXECUTION BY EDF\n");
    }
    
    int tempo = 0;

    task *em_execucao = NULL;
    int unidades_execucao = 0;

    int unidades_idle = 0;


    while(tempo <tempo_total){

        task *atual = head;

        while(atual != NULL){
            if(tempo == atual->proxima_chegada){
                atual->restante = atual->burst;
                atual->deadline_absoluto = atual->proxima_chegada + atual->deadline;
                atual->ativa = 1;
            }

            atual = atual->next;
        }
        task *escolhida = NULL;

        atual = head;

        while(atual != NULL){

            if(atual->ativa == 1){
                if(escolhida == NULL){
                    escolhida = atual;
                }
                else if(strcmp(argv[1],"rate") == 0){
                    if(atual->periodo < escolhida->periodo){
                        escolhida = atual;
                    }
                }

                else if(strcmp(argv[1],"edf") == 0){
                    if(atual->deadline_absoluto < escolhida->deadline_absoluto){
                        escolhida = atual;
                    }
                }
            }
            atual = atual->next;
        }

        if(escolhida == NULL){
            unidades_idle++;
        }

        if(escolhida != em_execucao){

            if(unidades_idle > 0){
                fprintf(saida,"idle for %d units\n", unidades_idle);
                unidades_idle = 0;
            }

            if(em_execucao != NULL){
                fprintf(saida,"[%s] for %d units - H\n",em_execucao->nome,unidades_execucao);
            }

            em_execucao = escolhida;
            unidades_execucao =0;
        }

        if(escolhida != NULL){
            unidades_execucao++;
        }

        if(escolhida != NULL){
            escolhida->restante--;
        }

        if (escolhida != NULL && escolhida->restante == 0) {
            fprintf(saida, "[%s] for %d units - F\n",escolhida->nome, unidades_execucao);

            escolhida->completas++;
            escolhida->ativa = 0;
            escolhida->proxima_chegada += escolhida->periodo;

            em_execucao = NULL;
            unidades_execucao = 0;
        }

        atual = head;

        while(atual != NULL){
            if(atual->ativa == 1 && atual->restante > 0 && tempo + 1 == atual->deadline_absoluto){

                if(atual == em_execucao){
                    fprintf(saida, "[%s] for %d units - L\n",atual->nome,unidades_execucao);

                    em_execucao = NULL;
                    unidades_execucao = 0;
                }

                atual->perdidas++;
                atual->ativa = 0;
                atual->restante = 0;
                atual->proxima_chegada += atual->periodo;
            }

            atual = atual->next;
        }

        tempo++;
    }

    if(unidades_idle > 0){
        fprintf(saida,"idle for %d units\n",unidades_idle);
        unidades_idle = 0;
    }

    task *atual = NULL;

    atual = head;

    while(atual != NULL){
        if(atual->ativa == 1 && atual->restante > 0){
            atual->killed++;
        }

        atual = atual->next;
    }

    

    fprintf(saida,"LOST DEADLINES\n");

    atual = head;

    while(atual != NULL){
        fprintf(saida,"[%s] %d\n", atual->nome, atual->perdidas);
        atual = atual->next;
    }

    fprintf(saida,"COMPLETE EXECUTION\n");
    atual = head;

    while(atual != NULL){
        fprintf(saida,"[%s] %d\n", atual->nome, atual->completas);
        atual = atual->next;
    }

    fprintf(saida,"KILLED\n");
    atual = head;

    while(atual != NULL){
        fprintf(saida,"[%s] %d\n", atual->nome, atual->killed);
        atual = atual->next;
    }

    atual = head;

    while(atual != NULL){
        task *proxima = atual->next;
        free(atual);
        atual = proxima;
    }


    fclose(arquivo);
    fclose(saida);

    return 0;
}