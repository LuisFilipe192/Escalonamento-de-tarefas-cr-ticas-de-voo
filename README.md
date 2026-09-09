# Escalonamento de Tarefas Críticas de Voo

## Descrição do projeto

O Escalonamento de Tarefas Críticas de Voo é um simulador desenvolvido em C. Seu objetivo é simular o escalonamento de tarefas críticas utilizando dois algoritmos diferentes: Rate e EDF.

Cada tarefa possui um nome, um período, um deadline e um burst. Todas as tarefas chegam pela primeira vez no instante 0 e podem possuir novas instâncias de acordo com seus respectivos períodos.

A simulação utiliza execução preemptiva. Isso significa que uma tarefa que está sendo executada pode ser interrompida quando outra tarefa com maior prioridade precisa utilizar o processador.

No algoritmo Rate, a prioridade é determinada pelo menor período. No algoritmo Earliest Deadline First, a prioridade é determinada pelo menor deadline absoluto.

Em ambos os algoritmos, quando ocorre empate, é utilizada a ordem em que as tarefas aparecem no arquivo de entrada.

O programa também identifica tarefas que perderam seus deadlines, tarefas que terminaram completamente e tarefas que permanecem em execução quando o tempo total da simulação chega ao fim.

O resultado da simulação é armazenado em um arquivo de saída específico para cada algoritmo.

O projeto foi desenvolvido em C para ambiente Linux.

## Estrutura dos arquivos

O projeto está organizado dessa forma:

- `src/scheduler.c` — responsável pela implementação dos algoritmos de escalonamento Rate Monotonic e Earliest Deadline First.
- `Makefile` — utilizado para compilar e limpar os arquivos compilados.
- `evidencias.log` — arquivo utilizado para registrar os testes realizados durante o desenvolvimento.
- `scripts/evidencias.sh` — script utilizado para auxiliar no registro das evidências dos testes.
- `README.md` — documentação do projeto.
- `.gitignore` — define arquivos que não devem ser enviados para o repositório.
- `rate_lfass.out` — arquivo de saída gerado quando o programa é executado utilizando Rate Monotonic.
- `edf_lfass.out` — arquivo de saída gerado quando o programa é executado utilizando Earliest Deadline First.

## Como compilar

Para compilar o projeto, basta executar o comando:

```
make
```

Para remover os arquivos compilados, pode ser utilizado:
```
make clean
```
## Como executar

Depois de compilar o projeto, o programa pode ser executado utilizando o seguinte formato:

```
./scheduler [algoritmo] [arquivo_de_entrada]
```
O algoritmo deve ser rate ou edf.

Por exemplo:
```
./scheduler rate entrada.txt
```
ou:
```
./scheduler edf entrada.txt
```
O resultado será gravado em:
```
rate_lfass.out
```
ou:
```
edf_lfass.out
```
A execução normal não apresenta mensagens na saída padrão.

## Formato do arquivo de entrada

A primeira linha do arquivo deve conter o tempo total da simulação.

As linhas seguintes devem conter as informações de cada tarefa no seguinte formato:
```
[NOME] [PERÍODO] [DEADLINE] [BURST]
```
Por exemplo:
```
100
ATT 20 12 8
NAV 50 30 15
```
Os valores utilizados para período, deadline e burst devem ser inteiros positivos.

Também devem ser respeitadas as seguintes condições:
```
BURST <= DEADLINE <= PERÍODO
```
Todas as tarefas chegam pela primeira vez no instante 0.

Após a primeira execução, novas instâncias da tarefa chegam de acordo com seu período.

## Algoritmos de escalonamento
```
Rate Monotonic
```


O algoritmo Rate determina a prioridade das tarefas utilizando seus períodos.

A tarefa com o menor período possui maior prioridade.

Por exemplo:
```
Tarefa A → período 20
Tarefa B → período 50
```
Nesse caso, a tarefa A possui maior prioridade que a tarefa B.

O algoritmo utiliza execução preemptiva. Portanto, uma tarefa de menor prioridade pode ser interrompida quando uma tarefa de maior prioridade estiver pronta para executar.

O resultado é armazenado no arquivo:
```
rate_lfass.out
```
## Earliest Deadline First

O algoritmo Earliest Deadline First determina a prioridade das tarefas utilizando seus deadlines absolutos.

A tarefa com o menor deadline absoluto possui maior prioridade.

Por exemplo:
```
Tarefa A → deadline absoluto 12
Tarefa B → deadline absoluto 30
```
Nesse caso, a tarefa A possui maior prioridade que a tarefa B.

Assim como no Rate Monotonic, a execução é preemptiva.

O resultado é armazenado no arquivo:
```
edf_lfass.out
```
## Desempate entre tarefas

Quando duas ou mais tarefas possuem a mesma prioridade, o desempate é realizado utilizando a ordem em que as tarefas aparecem no arquivo de entrada.

Essa regra é utilizada tanto no Rate Monotonic quanto no Earliest Deadline First.

## Perda de deadlines


Quando uma instância de uma tarefa não consegue terminar até o seu deadline absoluto, ela é considerada perdida.

O restante da execução dessa instância é descartado.

A tarefa volta a participar da simulação somente quando sua próxima instância chegar, de acordo com seu período.

As quantidades de deadlines perdidos são apresentadas na seção:
```
LOST DEADLINES
```
Por exemplo:
```
LOST DEADLINES
[ATT] 0
[NAV] 1
```
Nesse caso, nenhuma instância da tarefa ATT perdeu o deadline, enquanto uma instância da tarefa NAV perdeu o deadline.

## Execução completa


Ao final da simulação, o programa apresenta a quantidade de instâncias que terminaram completamente para cada tarefa.

Essa informação é apresentada na seção:
```
COMPLETE EXECUTION
```
Por exemplo:
```
COMPLETE EXECUTION
[ATT] 5
[NAV] 1
```
## Tarefas mortas


Quando o tempo total da simulação chega ao fim e uma instância ainda possui tempo de execução restante, ela é considerada morta.

A quantidade de instâncias nessa situação é apresentada na seção:
```
KILLED
```
Por exemplo:
```
KILLED
[ATT] 0
[NAV] 1
```
## Formato da saída


A saída apresenta a sequência de execução do processador durante a simulação.

Cada trecho de execução de uma tarefa possui o seguinte formato:
```
[NOME] for [UNIDADES] units - [STATUS]
```
Os status utilizados são:
```
F — a tarefa terminou sua execução.
H — a tarefa foi interrompida por preempção.
L — a tarefa perdeu seu deadline enquanto estava sendo executada.
```
Quando nenhuma tarefa está pronta para executar, o período de ociosidade é apresentado como:
```
idle for [UNIDADES] units
```
Ao final da execução são apresentadas as três informações:
```
LOST DEADLINES
COMPLETE EXECUTION
KILLED
```
## Exemplo de execução

Utilizando o arquivo:
```
100
ATT 20 12 8
NAV 50 30 15
```
A execução utilizando Rate Monotonic produz:
```
EXECUTION BY RATE
[ATT] for 8 units - F
[NAV] for 12 units - H
[ATT] for 8 units - F
[NAV] for 2 units - L
idle for 10 units
[ATT] for 8 units - F
idle for 2 units
[NAV] for 10 units - H
[ATT] for 8 units - F
[NAV] for 5 units - F
idle for 7 units
[ATT] for 8 units - F
idle for 12 units
LOST DEADLINES
[ATT] 0
[NAV] 1
COMPLETE EXECUTION
[ATT] 5
[NAV] 1
KILLED
[ATT] 0
[NAV] 0
```

## Tratamento de erros


O programa realiza a validação dos argumentos e dos dados de entrada antes de iniciar a simulação.

São tratados erros como:
```
quantidade incorreta de argumentos;
algoritmo inválido;
arquivo inexistente ou que não pode ser aberto;
tempo total inválido;
arquivo malformado;
campo faltando;
valor não numérico;
valores menores ou iguais a zero;
burst maior que o deadline;
deadline maior que o período;
falha na alocação de memória;
falha na criação do arquivo de saída.
```
As mensagens de erro são exibidas utilizando stderr, enquanto a execução normal não apresenta mensagens na saída padrão.

Quando ocorre um erro de entrada, o programa encerra com código diferente de zero e não cria o arquivo de saída da simulação.

## Gerenciamento de memória

As tarefas são armazenadas dinamicamente em uma lista encadeada.

Durante o carregamento do arquivo, cada tarefa é criada utilizando alocação dinâmica de memória.

Ao final da execução, todas as tarefas armazenadas na lista são liberadas utilizando free.

Dessa forma, a memória utilizada pela lista de tarefas é liberada após o término da simulação.

## Registro das evidências


Durante o desenvolvimento do projeto, os testes e evidências das funcionalidades implementadas são registrados no arquivo:

```
evidencias.log
```
O registro contém os testes realizados durante o desenvolvimento dos algoritmos e das validações do programa.

Entre os testes realizados estão:
```
validação dos argumentos;
validação do algoritmo;
leitura e validação das tarefas;
escalonamento Rate Monotonic;
escalonamento Earliest Deadline First;
preempção;
desempate entre tarefas;
períodos de ociosidade;
perda de deadlines;
conclusão de tarefas;
tarefas mortas ao final da simulação;
chegada de novas instâncias;
tratamento de entradas inválidas.
Sistema operacional utilizado
```
O projeto foi desenvolvido e testado no seguinte ambiente:

Sistema operacional: Linux (Ubuntu)
Linguagem: C
Compilador: GCC
Padrão utilizado: C11