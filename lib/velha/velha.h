#ifndef _velha_H_
#define _velha_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// Sistemas operacionais compativeis
#define UNIX_LINUX 0
#define WINDOWS 1

// Selecione o sistema operacional alvo
#define OPERATING_SYSTEM WINDOWS

// Limitado em -> 2 <= VELHA_QTDE_LINHAS_E_COLUNAS <= (2^8)-1
#define VELHA_QTDE_LINHAS_E_COLUNAS 4

typedef enum
{
    VELHA_JOGADOR_VAZIO = 0,
    VELHA_JOGADOR_1,
    VELHA_JOGADOR_2,
    VELHA_JOGADOR_LENGHT
} velha_jogador_type;

typedef enum
{
    VELHA_STATUS_OK = 0,
    VELHA_STATUS_ERROR = 0xFF
} velha_status_type;

typedef struct
{
    uint8_t grafico[VELHA_QTDE_LINHAS_E_COLUNAS][VELHA_QTDE_LINHAS_E_COLUNAS];
    uint8_t ganhador;
    uint8_t rodada;
} velha_t;

// publics functions
void velha_init(velha_t *velha);
void velha_main(velha_t *velha);

#endif