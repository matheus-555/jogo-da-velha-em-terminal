#include "velha.h"

// ---------------------------------
// Constants macros
// ---------------------------------
#define VELHA_MAX_LIN VELHA_QTDE_LINHAS_E_COLUNAS
#define VELHA_MAX_COL VELHA_QTDE_LINHAS_E_COLUNAS
#define VELHA_MAX_RODADA ((VELHA_MAX_LIN * VELHA_MAX_COL))

// ---------------------------------
// Functions macros
// ---------------------------------
#define VELHA_CONVERTER_INT_TO_CHAR(val) (val + 48)
#define VELHA_CONVERTER_CHAR_TO_INT(val) (val - 48)
#define VELHA_NOVA_LINHA() printf("\n\n")

// ---------------------------------
// enums
// ---------------------------------
typedef enum
{
    VELHA_CARACTERE_O = 0,
    VELHA_CARACTERE_X,
    VELHA_CARACTERE_LENGHT
} velha_caractere_typedef;

typedef enum
{
    VELHA_VERIFICA_JOGO_GANHO_LINHAS = 0,
    VELHA_VERIFICA_JOGO_GANHO_COLUNAS,
    VELHA_VERIFICA_JOGO_GANHO_DIAG_PRINC,
    VELHA_VERIFICA_JOGO_GANHO_DIAG_SECUN,
    VELHA_VERIFICA_JOGO_GANHO_LENGTH
} velha_verificad_jogo_ganho_typedef;

// ---------------------------------
// tipos privados
// ---------------------------------
typedef struct
{
    uint8_t lin;
    uint8_t col;
    velha_status_type ret;
} velha_lin_col_t;

// ---------------------------------
// interfaces
// ---------------------------------
typedef velha_jogador_type (*velha_verifica_jogo_ganho_t)(velha_t *);

// ---------------------------------
//   Metodos privados abstract
// ---------------------------------
static void velha_set_valor(velha_t *velha, uint8_t lin, uint8_t col, uint8_t valor);
static uint8_t velha_get_valor(velha_t *velha, uint8_t lin, uint8_t col);
static void velha_imprime(velha_t *velha);
static void velha_kernel(velha_t *velha);
static bool velha_is_jogada_valida(velha_t *velha, uint8_t lin, uint8_t col);
static bool velha_is_jogo_finalizado(velha_t *velha);
static void velha_imprime_resultado_final(velha_t *velha);
static velha_lin_col_t velha_get_lin_and_col(uint8_t *jogada_str);
static velha_jogador_type velha_get_ganhador(velha_t *velha);
static velha_jogador_type velha_get_jogador_atual(velha_t *velha);
static velha_caractere_typedef velha_get_caractere_jogador(velha_jogador_type jogador);
static void velha_set_caractere_jogador(velha_jogador_type jogador, velha_caractere_typedef caractere);
static velha_jogador_type velha_get_jogador_by_caractere(velha_caractere_typedef caractere);
static bool velha_is_caractere_jogador(velha_t *velha, uint8_t lin, uint8_t col);
static velha_jogador_type velha_verifica_linhas(velha_t *velha);
static velha_jogador_type velha_verifica_colunas(velha_t *velha);
static velha_jogador_type velha_verifica_diagonal_principal(velha_t *velha);
static velha_jogador_type velha_verifica_diagonal_secundaria(velha_t *velha);
static void velha_zera_array(uint8_t *array, uint16_t length);
static int velha_converter_char_to_int(uint8_t *str);
static uint16_t velha_get_str_len(uint8_t *str);

// ---------------------------------
// Variables constant
// ---------------------------------
static const uint8_t CARACTERES_DISPONIVEIS_PARA_JOGADOR[VELHA_CARACTERE_LENGHT] = {
    [VELHA_CARACTERE_O] = 'O',
    [VELHA_CARACTERE_X] = 'X',
};

static const velha_verifica_jogo_ganho_t verifica_jogo_ganho_callback[VELHA_VERIFICA_JOGO_GANHO_LENGTH] = {
    [VELHA_VERIFICA_JOGO_GANHO_LINHAS] = &velha_verifica_linhas,
    [VELHA_VERIFICA_JOGO_GANHO_COLUNAS] = &velha_verifica_colunas,
    [VELHA_VERIFICA_JOGO_GANHO_DIAG_PRINC] = &velha_verifica_diagonal_principal,
    [VELHA_VERIFICA_JOGO_GANHO_DIAG_SECUN] = &velha_verifica_diagonal_secundaria};

// ---------------------------------
// Variables private
// ---------------------------------

// guarda o caracrete do jogador no seu respectivo indice
static uint8_t caractere_do_jogador[VELHA_JOGADOR_LENGHT];

// ---------------------------------
//   Metodos publicos
// ---------------------------------

//
//
void velha_main(velha_t *velha)
{
    while (!velha_is_jogo_finalizado(velha))
    {
        velha_imprime(velha);
        velha_kernel(velha);
    }

    velha_imprime(velha);
    velha_imprime_resultado_final(velha);
}

//
//
void velha_init(velha_t *velha)
{
    register int l, c, i;

    for (l = 0, i = 1; l < VELHA_MAX_LIN; ++l)
        for (c = 0; c < VELHA_MAX_COL; ++c, ++i)
            velha_set_valor(velha, l, c, i);

    velha_set_caractere_jogador(VELHA_JOGADOR_1, VELHA_CARACTERE_O);
    velha_set_caractere_jogador(VELHA_JOGADOR_2, VELHA_CARACTERE_X);

    velha->ganhador = 0;
    velha->rodada = 0;
}

// ---------------------------------
//   Metodos privados implementacao
// ---------------------------------
static void velha_kernel(velha_t *velha)
{
    uint8_t flag = false;
    uint8_t valorJogada[VELHA_MAX_RODADA];
    uint8_t caractere_jogada;
    velha_lin_col_t linha_coluna_selecionada;

    do
    {
        printf("Jogador %d ", velha_get_jogador_atual(velha));

        printf("Escolha sua jogada: ");
        scanf("%s", valorJogada);

        linha_coluna_selecionada = velha_get_lin_and_col(valorJogada);

        if (linha_coluna_selecionada.ret != VELHA_STATUS_ERROR)
            flag = velha_is_jogada_valida(velha, linha_coluna_selecionada.lin, linha_coluna_selecionada.col);

        if (!flag)
        {
            printf("\n\nJOGADA INVALIDA\n\n");

            #if OPERATING_SYSTEM == WINDOWS
                system("pause");
            #elif OPERATING_SYSTEM == UNIX_LINUX
                system("read -p 'Aperte qualquer tecla para continuar'");
            #endif

            velha_imprime(velha);
        }
    } while (!flag);

    caractere_jogada = velha_get_caractere_jogador(velha_get_jogador_atual(velha));

    velha_set_valor(velha, linha_coluna_selecionada.lin, linha_coluna_selecionada.col, caractere_jogada);

    if (++velha->rodada > VELHA_MAX_LIN)
        velha->ganhador = velha_get_ganhador(velha);
}

//
//
static void velha_imprime(velha_t *velha)
{
    register int l, c, i;

    #if OPERATING_SYSTEM == WINDOWS
        system("cls");
    #elif OPERATING_SYSTEM == UNIX_LINUX
        system("clear");
    #endif

    for (l = 0; l < VELHA_MAX_LIN - 1; ++l)
    {
        for (c = 0; c < VELHA_MAX_COL - 1; ++c)
            if (velha_is_caractere_jogador(velha, l, c))
                printf(" %c  |", velha_get_valor(velha, l, c));
            else
                printf(" %.2d |", velha_get_valor(velha, l, c));

        if (velha_is_caractere_jogador(velha, l, c))
            printf(" %c  ", velha_get_valor(velha, l, c));
        else
            printf(" %.2d ", velha_get_valor(velha, l, c));

        printf("\n");
        for (i = 0; i < (VELHA_MAX_RODADA + 5); ++i)
            printf("-");
        printf("\n");
    }

    for (; l < VELHA_MAX_LIN; ++l)
    {
        for (c = 0; c < VELHA_MAX_COL - 1; ++c)
            if (velha_is_caractere_jogador(velha, l, c))
                printf(" %c  |", velha_get_valor(velha, l, c));
            else
                printf(" %.2d |", velha_get_valor(velha, l, c));

        if (velha_is_caractere_jogador(velha, l, c))
            printf(" %c  ", velha_get_valor(velha, l, c));
        else
            printf(" %.2d ", velha_get_valor(velha, l, c));
    }

    VELHA_NOVA_LINHA();
}

//
//
static void velha_set_valor(velha_t *velha, uint8_t lin, uint8_t col, uint8_t valor)
{
    velha->grafico[lin][col] = valor;
}

//
//
static uint8_t velha_get_valor(velha_t *velha, uint8_t lin, uint8_t col)
{
    return velha->grafico[lin][col];
}

//
//
static bool velha_is_jogada_valida(velha_t *velha, uint8_t lin, uint8_t col)
{
    int i;

    for (i = 0; i < VELHA_CARACTERE_LENGHT; ++i)
        if (velha->grafico[lin][col] == CARACTERES_DISPONIVEIS_PARA_JOGADOR[i])
            return false;

    return true;
}

//
//
static velha_lin_col_t velha_get_lin_and_col(uint8_t *jogada_str)
{
    register int i, y;
    velha_lin_col_t ret;
    uint16_t valor_jogada = velha_converter_char_to_int(jogada_str);

    if (valor_jogada > 0)
        for (i = y = 1; i <= VELHA_QTDE_LINHAS_E_COLUNAS; ++i, y += VELHA_MAX_COL)
            if (valor_jogada <= (i * VELHA_QTDE_LINHAS_E_COLUNAS))
            {
                ret.lin = i - 1;
                ret.col = valor_jogada - y;
                ret.ret = VELHA_STATUS_OK;
                return ret;
            }

    ret.ret = VELHA_STATUS_ERROR;

    return ret;
}

// @ Retorna o jogador que ganhou
// retorna VELHA_JOGADOR_VAZIO se nao houve ganhador
static velha_jogador_type velha_get_ganhador(velha_t *velha)
{
    int i;
    velha_jogador_type ret;

    for (i = 0; i < VELHA_VERIFICA_JOGO_GANHO_LENGTH; ++i)
    {
        ret = verifica_jogo_ganho_callback[i](velha);

        if (ret != VELHA_JOGADOR_VAZIO)
            break;
    }

    return ret;
}

//
//
static bool velha_is_jogo_finalizado(velha_t *velha)
{
    return (velha->ganhador != VELHA_JOGADOR_VAZIO) || (velha->rodada == VELHA_MAX_RODADA);
}

//
//
static void velha_imprime_resultado_final(velha_t *velha)
{
    if (velha_is_jogo_finalizado(velha))
    {
        if (velha->ganhador)
            printf("JOGADOR %d GANHOU!!\n\n", velha->ganhador);
        else
            printf("JOGO EMPATADO!!\n\n");
    }
}

//
//
static velha_jogador_type velha_get_jogador_atual(velha_t *velha)
{
    return (velha->rodada % 2) == 0 ? VELHA_JOGADOR_1 : VELHA_JOGADOR_2;
}

//
//
static velha_caractere_typedef velha_get_caractere_jogador(velha_jogador_type jogador)
{
    return caractere_do_jogador[jogador];
}

//
//
static void velha_set_caractere_jogador(velha_jogador_type jogador, velha_caractere_typedef caractere)
{
    caractere_do_jogador[jogador] = CARACTERES_DISPONIVEIS_PARA_JOGADOR[caractere];
}

//
//
static velha_jogador_type velha_get_jogador_by_caractere(velha_caractere_typedef caractere)
{
    register int indice_jogador;

    for (indice_jogador = VELHA_JOGADOR_VAZIO + 1; indice_jogador < VELHA_JOGADOR_LENGHT; ++indice_jogador)
        if (caractere_do_jogador[indice_jogador] == caractere)
            return indice_jogador;

    return VELHA_JOGADOR_VAZIO;
}

//
//
static bool velha_is_caractere_jogador(velha_t *velha, uint8_t lin, uint8_t col)
{
    register int i;

    for (i = 0; i < VELHA_CARACTERE_LENGHT; ++i)
        if (velha->grafico[lin][col] == CARACTERES_DISPONIVEIS_PARA_JOGADOR[i])
            return true;

    return false;
}

//
//
static velha_jogador_type velha_verifica_linhas(velha_t *velha)
{
    register int lin, col, i;
    uint8_t contador_caractere[VELHA_CARACTERE_LENGHT] = {
        0,
    };

    // Verifica linhas
    for (lin = 0; lin < VELHA_MAX_LIN; ++lin)
    {
        for (col = 0; col < VELHA_MAX_COL; ++col)
            for (i = 0; i < VELHA_CARACTERE_LENGHT; ++i)
            {
                if (velha->grafico[lin][col] == CARACTERES_DISPONIVEIS_PARA_JOGADOR[i])
                    contador_caractere[i]++;

                if (contador_caractere[i] == VELHA_MAX_LIN)
                    return velha_get_jogador_by_caractere(CARACTERES_DISPONIVEIS_PARA_JOGADOR[i]);
            }

        velha_zera_array(contador_caractere, VELHA_CARACTERE_LENGHT);
    }

    return VELHA_JOGADOR_VAZIO;
}

//
//
static velha_jogador_type velha_verifica_colunas(velha_t *velha)
{
    register int lin, col, i;
    uint8_t contador_caractere[VELHA_CARACTERE_LENGHT] = {
        0,
    };

    // Verifica colunas
    for (col = 0; col < VELHA_MAX_COL; ++col)
    {
        for (lin = 0; lin < VELHA_MAX_LIN; ++lin)
            for (i = 0; i < VELHA_CARACTERE_LENGHT; ++i)
            {
                if (velha->grafico[lin][col] == CARACTERES_DISPONIVEIS_PARA_JOGADOR[i])
                    contador_caractere[i]++;

                if (contador_caractere[i] == VELHA_MAX_LIN)
                    return velha_get_jogador_by_caractere(CARACTERES_DISPONIVEIS_PARA_JOGADOR[i]);
            }
        velha_zera_array(contador_caractere, VELHA_CARACTERE_LENGHT);
    }
    return VELHA_JOGADOR_VAZIO;
}

//
//
static velha_jogador_type velha_verifica_diagonal_principal(velha_t *velha)
{
    register int lin, col, i;
    uint8_t contador_caractere[VELHA_CARACTERE_LENGHT] = {
        0,
    };

    // // Verifica diagonal principal
    for (lin = col = 0; lin < VELHA_MAX_LIN; ++lin, ++col)
        for (i = 0; i < VELHA_CARACTERE_LENGHT; ++i)
        {
            if (velha->grafico[lin][col] == CARACTERES_DISPONIVEIS_PARA_JOGADOR[i])
                contador_caractere[i]++;

            if (lin == VELHA_MAX_LIN - 1)
                if (contador_caractere[i] == VELHA_MAX_LIN)
                    return velha_get_jogador_by_caractere(CARACTERES_DISPONIVEIS_PARA_JOGADOR[i]);
        }

    return VELHA_JOGADOR_VAZIO;
}

//
//
static velha_jogador_type velha_verifica_diagonal_secundaria(velha_t *velha)
{
    register int lin, col, i;
    uint8_t contador_caractere[VELHA_CARACTERE_LENGHT] = {
        0,
    };

    // Verifica diagonal secundaria
    for (lin = 0, col = VELHA_MAX_COL - 1; lin < VELHA_MAX_LIN; ++lin, --col)
        for (i = 0; i < VELHA_CARACTERE_LENGHT; ++i)
        {
            if (velha->grafico[lin][col] == CARACTERES_DISPONIVEIS_PARA_JOGADOR[i])
                contador_caractere[i]++;

            if (lin == VELHA_MAX_LIN - 1)
                if (contador_caractere[i] == VELHA_MAX_LIN)
                    return velha_get_jogador_by_caractere(CARACTERES_DISPONIVEIS_PARA_JOGADOR[i]);
        }

    return VELHA_JOGADOR_VAZIO;
}

//
//
static void velha_zera_array(uint8_t *array, uint16_t length)
{
    register int i;

    for (i = 0; i < length; ++i)
        array[i] = 0;
}

//
//
static int velha_converter_char_to_int(uint8_t *str)
{
    const int LENGTH = velha_get_str_len(str);
    int ret = 0;
    int i;

    for (i = 0; i < LENGTH; i++)
        ret = (ret * 10) + VELHA_CONVERTER_CHAR_TO_INT(str[i]);

    return ret;
}

//
//
static uint16_t velha_get_str_len(uint8_t *str)
{
    uint16_t count = 0;
    uint8_t *ptr = str;

    for (; *ptr++ != '\0'; ++count);

    return count;
}
