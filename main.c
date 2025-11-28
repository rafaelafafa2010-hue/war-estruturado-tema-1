#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char corDono[20];
    int tropas;
} Territorio;

typedef struct {
    Territorio *lista;
    int qtd;
} Mapa;

Mapa* criarMapa(int qtd) {
    Mapa m = (Mapa) malloc(sizeof(Mapa));
    if (!m) return NULL;
    m->lista = (Territorio*) malloc(sizeof(Territorio) * qtd);
    if (!m->lista) {
        free(m);
        return NULL;
    }
    m->qtd = qtd;
    return m;
}

void destruirMapa(Mapa *m) {
    if (m) {
        free(m->lista);
        free(m);
    }
}

void cadastrarTerritorios(Mapa *m) {
    printf("\n=== Cadastro de Territorios ===\n");
    for (int i = 0; i < m->qtd; i++) {
        printf("Territorio %d:\n", i);
        printf("Nome: ");
        fgets(m->lista[i].nome, 30, stdin);
        m->lista[i].nome[strcspn(m->lista[i].nome, "\n")] = 0;

        printf("Cor do dono: ");
        fgets(m->lista[i].corDono, 20, stdin);
        m->lista[i].corDono[strcspn(m->lista[i].corDono, "\n")] = 0;

        printf("Quantidade inicial de tropas: ");
        scanf("%d", &m->lista[i].tropas);
        getchar();
    }
}

void listarTerritorios(Mapa *m) {
    printf("\n=== Mapa de Territorios ===\n");
    for (int i = 0; i < m->qtd; i++) {
        printf("[%d] %s - Dono: %s - Tropas: %d\n", i,
               m->lista[i].nome,
               m->lista[i].corDono,
               m->lista[i].tropas);
    }
}

void atacar(Mapa *m) {
    int origem, destino, tropasAtacantes;
    listarTerritorios(m);
    printf("\nIndice do territorio de ORIGEM: ");
    scanf("%d", &origem);
    printf("Indice do territorio de DESTINO: ");
    scanf("%d", &destino);
    getchar();

    if (origem < 0 || origem >= m->qtd || destino < 0 || destino >= m->qtd) {
        printf("Territorio invalido.\n");
        return;
    }
    if (origem == destino) {
        printf("Origem e destino nao podem ser iguais.\n");
        return;
    }
    if (m->lista[origem].tropas <= 1) {
        printf("Nao ha tropas suficientes para atacar.\n");
        return;
    }

    printf("Quantas tropas deseja usar no ataque (1 a %d)? ",
           m->lista[origem].tropas - 1);
    scanf("%d", &tropasAtacantes);
    getchar();
    if (tropasAtacantes < 1 || tropasAtacantes >= m->lista[origem].tropas) {
        printf("Quantidade invalida.\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Ataque bem-sucedido! Tropas do defensor reduzidas.\n");
        m->lista[destino].tropas -= tropasAtacantes;
        if (m->lista[destino].tropas <= 0) {
            printf("Territorio conquistado!\n");
            m->lista[destino].tropas = tropasAtacantes;
            strcpy(m->lista[destino].corDono, m->lista[origem].corDono);
            m->lista[origem].tropas -= tropasAtacantes;
        }
    } else {
        printf("Defesa bem-sucedida! Tropas do atacante reduzidas.\n");
        m->lista[origem].tropas -= tropasAtacantes;
        if (m->lista[origem].tropas < 1) m->lista[origem].tropas = 1;
    }
}

int verificarMissao(Mapa *m) {
    // Missao simples: todos os territorios com a mesma cor
    for (int i = 1; i < m->qtd; i++) {
        if (strcmp(m->lista[i].corDono, m->lista[0].corDono) != 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    srand((unsigned int) time(NULL));

    int qtdTerritorios;
    printf("Quantidade de territorios do mapa: ");
    scanf("%d", &qtdTerritorios);
    getchar();

    Mapa *mapa = criarMapa(qtdTerritorios);
    if (!mapa) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    cadastrarTerritorios(mapa);

    int opc;
    do {
        printf("\n===== WAR ESTRUTURADO =====\n");
        printf("1 - Listar territorios\n");
        printf("2 - Atacar territorio\n");
        printf("3 - Verificar missao (todos territorios de uma cor)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1:
                listarTerritorios(mapa);
                break;
            case 2:
                atacar(mapa);
                break;
            case 3:
                if (verificarMissao(mapa)) {
                    printf("Missao concluida! Todos os territorios tem o mesmo dono.\n");
                } else {
                    printf("Missao ainda nao concluida.\n");
                }
                break;
            case 0:
                printf("Encerrando o jogo.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opc != 0);

    destruirMapa(mapa);
    return 0;
}
