#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include <time.h>     
#include <locale.h>   

// ============================================================================
//                          PROJETO WAR ESTRUTURADO
// ============================================================================

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define TAM_NOME_MAX 30
#define NUM_MISSOES 2
#define COR_JOGADOR "Amarelo" // Cor do exército do jogador para verificação da missão.

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_NOME_MAX];
    char cor_exercito[TAM_NOME_MAX];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int num_territorios);
void inicializarTerritorios(Territorio* mapa, const char* cor_jogador);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio* mapa, int num_territorios);
void exibirMissao(int id_missao);
void limparBufferEntrada(void);

// Funções de lógica principal do jogo:
int sortearMissao(void);
void faseDeAtaque(Territorio* mapa, int num_territorios, const char* cor_jogador);
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* cor_jogador);
int verificarVitoria(const Territorio* mapa, int id_missao, const char* cor_jogador);

// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Define o locale para português.
    srand(time(NULL));                // Inicializa a semente para geração de números aleatórios.

    Territorio* mapa = alocarMapa(NUM_TERRITORIOS); // Aloca a memória para o mapa.
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa. Encerrando.\n");
        return 1;
    }

    inicializarTerritorios(mapa, COR_JOGADOR); // Preenche os territórios com dados iniciais.
    int id_missao = sortearMissao();           // Sorteia a missão secreta.
    int escolha = -1;
    int vitoria = 0;

    printf("====================================================\n");
    printf("         WAR ESTRUTURADO - NÍVEL MESTRE\n");
    printf("====================================================\n");

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        // Exibição do estado atual
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(id_missao);
        exibirMenuPrincipal();

        printf("Sua escolha: ");
        if (scanf("%d", &escolha) != 1) {
            limparBufferEntrada();
            escolha = -1; // Garante que não é uma opção válida
        }
        limparBufferEntrada(); // Limpa o buffer após o scanf

        switch (escolha) {
            case 1: // Atacar
                faseDeAtaque(mapa, NUM_TERRITORIOS, COR_JOGADOR);
                break;
            case 2: // Verificar Missão
                vitoria = verificarVitoria(mapa, id_missao, COR_JOGADOR);
                if (vitoria) {
                    printf("\nPARABÉNS! VOCÊ CUMPRIU SUA MISSÃO SECRETA!\n");
                    printf("O mundo é seu! Fim de jogo.\n");
                } else {
                    printf("\nMissão ainda não cumprida. Continue atacando!\n");
                }
                break;
            case 0: // Sair
                printf("\nEncerrando o jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

        if (vitoria && escolha != 0) { // Sai do loop se a vitória for alcançada
            escolha = 0;
        }

        if (escolha != 0) {
            printf("\nPressione ENTER para continuar...\n");
            getchar(); // Pausa a execução
        }

    } while (escolha != 0);

    // 3. Limpeza:
    liberarMemoria(mapa);
    return 0;
}

// --- Implementação das Funções ---

// Aloca dinamicamente a memória para o vetor de territórios.
Territorio* alocarMapa(int num_territorios) {
    Territorio* mapa = (Territorio*)calloc(num_territorios, sizeof(Territorio));
    return mapa; // Retorna o ponteiro ou NULL
}

// Libera a memória previamente alocada.
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}

// Inicializa os territórios com nomes, exércitos e tropas.
void inicializarTerritorios(Territorio* mapa, const char* cor_jogador) {
    // Inicialização automática para a fase Mestre
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor_exercito, cor_jogador); // Jogador (Amarelo)
    mapa[0].tropas = 3;

    strcpy(mapa[1].nome, "Canada");
    strcpy(mapa[1].cor_exercito, "Vermelho");
    mapa[1].tropas = 2;

    strcpy(mapa[2].nome, "India");
    strcpy(mapa[2].cor_exercito, "Verde"); // Alvo de missão
    mapa[2].tropas = 4;

    strcpy(mapa[3].nome, "Japao");
    strcpy(mapa[3].cor_exercito, cor_jogador); // Jogador (Amarelo)
    mapa[3].tropas = 2;

    strcpy(mapa[4].nome, "Egito");
    strcpy(mapa[4].cor_exercito, "Verde"); // Alvo de missão
    mapa[4].tropas = 1;

    printf("Mapa inicializado com %d territórios.\n", NUM_TERRITORIOS);
}

// Imprime na tela o menu de ações.
void exibirMenuPrincipal(void) {
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair do Jogo\n");
    printf("---------------------\n");
}

// Mostra o estado atual dos territórios (passagem por referência constante).
void exibirMapa(const Territorio* mapa, int num_territorios) {
    printf("\n================ MAPA ATUAL ================\n");
    printf("| ID | Nome               | Cor do Exército | Tropas |\n");
    printf("|----|--------------------|-----------------|--------|\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("| %-2d | %-18s | %-15s | %-6d |\n",
               i + 1, mapa[i].nome, mapa[i].cor_exercito, mapa[i].tropas);
    }
    printf("============================================\n");
}

// Exibe a descrição da missão.
void exibirMissao(int id_missao) {
    printf("\n--- SUA MISSÃO SECRETA ---\n");
    switch (id_missao) {
        case 1:
            printf("ID 1: Conquistar e dominar todos os territórios do exército VERDE.\n");
            break;
        case 2:
            printf("ID 2: Conquistar um total de 3 territórios, não importa a cor.\n");
            break;
        default:
            printf("Missão desconhecida (ID %d).\n", id_missao);
            break;
    }
    printf("--------------------------\n");
}

// Limpa o buffer de entrada.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Sorteia e retorna um ID de missão aleatório.
int sortearMissao(void) {
    return (rand() % NUM_MISSOES) + 1; // Retorna 1 ou 2
}

// Gerencia a interface para a ação de ataque.
void faseDeAtaque(Territorio* mapa, int num_territorios, const char* cor_jogador) {
    int id_atacante, id_defensor;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Digite o ID do Território ATACANTE (1 a %d): ", num_territorios);
    if (scanf("%d", &id_atacante) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Digite o ID do Território DEFENSOR (1 a %d): ", num_territorios);
    if (scanf("%d", &id_defensor) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    // Validações
    if (id_atacante < 1 || id_atacante > num_territorios ||
        id_defensor < 1 || id_defensor > num_territorios ||
        id_atacante == id_defensor) {
        printf("Seleção inválida. IDs devem ser diferentes e dentro do limite.\n");
        return;
    }

    Territorio* atacante = &mapa[id_atacante - 1];
    Territorio* defensor = &mapa[id_defensor - 1];

    if (strcmp(atacante->cor_exercito, cor_jogador) != 0) {
        printf("Erro: Você só pode atacar a partir de um território que você domina (%s).\n", cor_jogador);
        return;
    }

    if (atacante->tropas <= 1) {
        printf("Erro: O território atacante precisa ter pelo menos 2 tropas para atacar (1 fica).\n");
        return;
    }

    simularAtaque(atacante, defensor, cor_jogador);
}

// Executa a lógica da batalha (passagem por referência para modificação).
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* cor_jogador) {
    int dado_ataque = (rand() % 6) + 1; // Dado de 1 a 6
    int dado_defesa = (rand() % 6) + 1;

    printf("\n--- BATALHA: %s vs %s ---\n", atacante->nome, defensor->nome);
    printf("Dado Atacante (%s): %d\n", atacante->nome, dado_ataque);
    printf("Dado Defensor (%s): %d\n", defensor->nome, dado_defesa);

    // Lógica do WAR: Empates favorecem o atacante.
    if (dado_ataque > dado_defesa || (dado_ataque == dado_defesa)) {
        printf("Vitória do ATACANTE! Defensor perde 1 tropa.\n");
        defensor->tropas--;
    } else {
        printf("Vitória do DEFENSOR! Atacante perde 1 tropa.\n");
        atacante->tropas--;
    }

    // Verifica a conquista
    if (defensor->tropas <= 0) {
        printf("CONQUISTA! O território %s foi conquistado por %s!\n",
               defensor->nome, cor_jogador);

        // Atualiza o dono do território
        strcpy(defensor->cor_exercito, cor_jogador);

        // Move 1 tropa do atacante para o novo território conquistado
        atacante->tropas--;
        defensor->tropas = 1;

        printf("Uma tropa foi movida de %s para %s.\n", atacante->nome, defensor->nome);
    } else if (atacante->tropas <= 1) {
        printf("O atacante %s não tem mais tropas suficientes para atacar (resta 1).\n", atacante->nome);
    }
}

// Verifica se o jogador cumpriu a missão (passagem por referência constante).
int verificarVitoria(const Territorio* mapa, int id_missao, const char* cor_jogador) {
    int i;
    int territorios_conquistados = 0;
    int verde_destruido = 1; // Começa como 1 (sim) e verifica se há algum Verde restante

    for (i = 0; i < NUM_TERRITORIOS; i++) {
        // Conta quantos territórios o jogador domina
        if (strcmp(mapa[i].cor_exercito, cor_jogador) == 0) {
            territorios_conquistados++;
        }

        // Verifica se ainda existe algum exército VERDE no mapa
        if (strcmp(mapa[i].cor_exercito, "Verde") == 0) {
            verde_destruido = 0; // Encontrou um território verde, então a missão não está completa
        }
    }

    switch (id_missao) {
        case 1: // Missão: Destruir o exército Verde
            return verde_destruido;
        case 2: // Missão: Conquistar 3 territórios
            return territorios_conquistados >= 3;
        default:
            return 0;
    }
}