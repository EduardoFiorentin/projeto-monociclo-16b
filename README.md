# Controlador Programável de 16 bits - Arquitetura Monociclo

## Introdução

Este projeto é um controlador programável baseado na arquitetura de CPU monociclo de 16 bits, desenvolvido como projeto de estudo em paralelo com a disciplina de Organização de Computadores. O principal objetivo é simular uma CPU capaz de executar um conjunto básico de instruções aritméticas e lógicas, além de operações de controle de fluxo e manipulação de memória.

O projeto está dividido em dois componentes principais:

- **Assembler**: Implementado na linguagem 'cpp' (arquivo `assembler.cpp`), o Assembler converte as instruções escritas em um arquivo de texto em código de máquina executável pela CPU simulada. O código pode ser compilado e executado para gerar a sequência de instruções que será carregada no circuito.

- **Circuito**: O circuito principal está descrito no arquivo `circuit.dig`, que deve ser aberto no software **Digital**. Este circuito simula o comportamento da CPU monociclo com componentes descritos em VHDL. Os componentes principais são: controlador (`controll.vhd`), o gerador de constantes (`imm_gen.vhd`), o decodificador de instruções (`instruction_decoder.vhd`), e a unidade lógica e aritmética (ULA) (`ula.vhd`). Para rodar o circuito, o **Digital** deve estar configurado para utilizar o **GHDL**, que permite a simulação correta dos componentes em VHDL.

Este projeto oferece uma oportunidade de explorar de forma prática os conceitos teóricos de arquitetura de computadores, utilizando tanto a programação de baixo nível quanto a simulação de hardware.


## Arquitetura Monociclo

Uma arquitetura baseada em monociclo significa que cada instrução é executada em um único ciclo de clock. Todas as operações – desde a busca da instrução até a execução e gravação de resultados – são realizadas dentro de um único ciclo, o que implica que o tempo de execução é constante, mas pode variar conforme a complexidade de cada instrução.

Este controlador possui:
- **16 bits de largura de instrução**, com suporte a operações aritméticas, controle de fluxo e acesso à memória.
- **16 registradores** (r0 a r15), permitindo armazenamento temporário e manipulação de dados durante a execução.
- **Memória de instruções** com endereçamento de 8 bits, capaz de armazenar até 256 instruções.
- **Memória de dados** com endereçamento de 16 bits, permitindo acessar até 65.536 posições de memória.


## Características do Projeto

O conjunto de instruções foi desenhado para ser simples, permitindo operações básicas como soma, subtração, multiplicação, divisão, e controle de fluxo (branching). A memória de instrução e os registradores podem ser facilmente manipulados para visualizar as execuções.

O projeto é composto por dois principais componentes:
1. **O Circuito Simulado**: Construído no software **Digital**.
2. **O Assembler**: Um montador que traduz programas assembly para código de máquina compatível com a arquitetura.

---

## Como Usar

### Circuito

1. **Abrir o arquivo "circuit.dig"**:
   - Utilize o software **Digital**. Certifique-se de que o **GHDL** está configurado para rodar no Digital.
      - [Digital](https://github.com/hneemann/Digital)
      - [GHDL](https://github.com/ghdl/ghdl)   
   - Talvez sejam necessários ajustes nas rotas da inclusão dos componentes vhdl. Para isso, basta clicar com o botão direito em cada componente e selecionar o seu arquivo .vhd correspondente.

2. **Montagem e Carregamento de Instruções**:
   - Escreva o programa desejado em um arquivo de texto.
   - Use o assembler para converter esse arquivo em código de máquina:
     ```bash
     assembler arquivo.txt
     ```
   - O assembler irá gerar um arquivo chamado `instructions` na pasta `assembler`. Este arquivo contém o código de máquina que deve ser carregado na memória de instruções.

3. **Carregar Instruções no Circuito**:
   - Inicie a simulação no **Digital**.
   - Copie as instruções geradas pelo assembler.
   - Clique com o botão esquerdo na **memória de instrução**, selecione todos os slots e pressione `ctrl + v` para carregar as instruções na memória.
   
OBS: Não é necessário selecionar os slots da memória até o final, apenas a quantidade necessária de slots para que caibam todas as instruções, porém é importante que as linhas selecionadas sejam por completo para garantir a integridade da ordem das instruções. 

4. **Executar o Programa**:
   - Para iniciar a execução das instruções, defina a entrada **start** como alta (high).
   - O controlador executará as instruções até atingir a instrução `end` (adicionada automaticamente pelo montador ao final do programa), momento em que o **contador de programa** (PC) irá parar de incrementar.

5. **Visualizar Resultados**:
   - Durante a execução, os valores dos **registradores** e **memória de dados** podem ser visualizados clicando com o botão esquerdo no banco de registradores e memória.

6. **Reiniciar a Simulação**:
   - Para rodar um novo programa, reinicie a simulação e repita os passos acima.

---

## Conjunto de Instruções

O controlador suporta dois tipos de instruções:

### Tipo 1 (Aritméticas e Memória)
As instruções do Tipo 1 seguem o formato:
*aaaa-bbbb-cccc-dddd*
- **a**: Código da instrução (4 bits).
- **b**: Registrador rs1 (4 bits).
- **c**: Registrador rs2 (4 bits).
- **d**: Registrador rd (4 bits).

Exemplo: `add rs1, rs2, rd` (Soma os valores em rs1 e rs2 e armazena o resultado em rd).

### Tipo 2 (Controle de Fluxo e Carregamento de constante)
As instruções de controle de fluxo usam por padrão os registradores `zero` e `um`,e seguem o seguinte formato:
*aaaa-ffffffff-dddd*
- **a**: Código da instrução (4 bits).
- **f**: Valor imediato (imm8) (8 bits).
- **d**: Registrador rd (4 bits).

Exemplo: `beq imm8` (Desvia a execução se r0 for igual a r1, incrementando o PC com o valor imediato).

### Lista de Instruções Suportadas

| Código | Instrução  | Sintaxe               | Semântica                          | Tipo |
|--------|------------|-----------------------|------------------------------------|------|
| 0000     | `add`      | `add rs1, rs2, rd`    | `rd <- rs1 + rs2`                 | 1    |
| 0010     | `sub`      | `sub rs1, rs2, rd`    | `rd <- rs1 - rs2`                 | 1    |
| 0011     | `mul`      | `mul rs1, rs2, rd`    | `rd <- rs1 * rs2`                 | 1    |
| 0100     | `div`      | `div rs1, rs2, rd`    | `rd <- rs1 / rs2`                 | 1    |
| 0101     | `lw`       | `lw rs1, rd`          | `rd <- (rs1)`                     | 1    |
| 0110     | `sw`       | `sw ro, rs1`          | `ro -> (rs1)`                     | 1    |
| 0111     | `beq`      | `beq imm8`            | Se `r0 == r1`, `PC = PC + imm8`   | 2    |
| 1000    | `bne`      | `bne imm8`            | Se `r0 != r1`, `PC = PC + imm8`   | 2    |
| 1001   | `bge`      | `bge imm8`            | Se `r0 >= r1`, `PC = PC + imm8`   | 2    |
| 1010     | `blt`      | `blt imm8`            | Se `r0 < r1`, `PC = PC + imm8`    | 2    |
| 1011     | `jal`      | `jal imm8`            | `PC <- PC + imm8`                 | 2    |
| 1101     | `li`       | `li imm8, rd`         | `rd <- imm8`                      | 2    |
---

## Estrutura do Projeto

O projeto está organizado em dois componentes principais: o **Assembler** e o **Circuito**. A seguir estão os arquivos incluídos e suas respectivas funções:

### Diretório Raiz

No diretório raiz do projeto, você encontrará os principais arquivos de simulação e código em VHDL que compõem o controlador programável:

- **`circuit.dig`**: Arquivo base do circuito para ser aberto no software **Digital**. Ele contém a arquitetura completa da CPU monociclo e seus componentes interligados. É importante garantir que o **Digital** esteja configurado para utilizar o **GHDL** como compilador, pois ele é responsável por criar os componentes escritos em VHDL do projeto.

- **Componentes VHDL**:
  - **`controll.vhd`**: Implementa o controle principal da CPU, determinando o fluxo das instruções e a lógica de controle do ciclo de execução.
  - **`imm_gen.vhd`**: Componente responsável por gerar e extrair os valores constantes (imediatos) a partir das instruções, facilitando operações que envolvem valores numéricos fixos.
  - **`instruction_decoder.vhd`**: Decodifica as instruções, separando seus campos em componentes essenciais como código de operação (opcode), registradores (`rs1`, `rs2`, `rd`), e valores imediatos (`imm`).
  - **`ula.vhd`**: Unidade Lógica e Aritmética (ULA) que executa as operações aritméticas e lógicas (como soma, subtração, multiplicação, comparação, entre outras) necessárias para a execução das instruções.

### Assembler

O Assembler é o responsável por converter os programas escritos em linguagem assembly para código de máquina, que pode ser carregado na memória de instruções do circuito. O Assembler é implementado em C++:

- **`assembler.cpp`**: Código-fonte do Assembler. Ele deve ser compilado para gerar o executável que realiza a conversão de assembly para código de máquina. Para compilar e executar o Assembler, use o seguinte comando:
OBS: É necessário ter o G++ instalado e configurado para o processo de compilação.
```
  g++ assembler.cpp -o assembler
```
Após compilar, o montador pode ser executado da seguinte forma: 
```  
assembler codigo.txt [instrucoes.txt]
```
**codigo.txt**: Arquivo com o assembly escrito
**instrucoes.txt**: Arquivo com as instruções traduzidas para codigo de maquina. Este parametro é opcional, e caso não seja fornecido o programa salva as instruções automaticamente em um arquivo **instructions.txt**
