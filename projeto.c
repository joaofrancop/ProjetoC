#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

// Estrutura para armazenar os dados do cliente
typedef struct {
    int id;
    char nome[50];
    char cpf[15];  // Campo para o CPF (11 dígitos + pontos e traço)
    char animal[50];
    int ativo; // 1 para ativo, 0 para desativado
} Cliente;

// Funções
void cadastrarCliente(Cliente **clientes, int *quantidade);
void listarClientes(Cliente *clientes, int quantidade);
void consultarCliente(Cliente *clientes, int quantidade);
void desativarCliente(Cliente *clientes, int quantidade);
void excluirCliente(Cliente **clientes, int *quantidade);
void venderServico();
void linha();

// Função principal do menu
void menuPrincipal() {
    Cliente *clientes = NULL;
    int quantidade = 0; // Número de clientes cadastrados
    int opcao;

    do {
        printf("======== MENU PRINCIPAL ========\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Listar Clientes\n");
        printf("3. Consultar Cliente\n");
        printf("4. Desativar Cliente\n");
        printf("5. Excluir Cliente\n");
        printf("6. Vender Servico\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF); // Limpar buffer
            continue;
        }

        linha();

        switch (opcao) {
            case 1: cadastrarCliente(&clientes, &quantidade); break;
            case 2: listarClientes(clientes, quantidade); break;
            case 3: consultarCliente(clientes, quantidade); break;
            case 4: desativarCliente(clientes, quantidade); break;
            case 5: excluirCliente(&clientes, &quantidade); break;
            case 6: venderServico(); break;
            case 7: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n"); break;
        }

        sleep(2);
        linha();
    } while (opcao != 7);

    // Libera a memória alocada dinamicamente
    free(clientes);
}

// Função para cadastrar um cliente
void cadastrarCliente(Cliente **clientes, int *quantidade) {
    (*quantidade)++;
    *clientes = realloc(*clientes, (*quantidade) * sizeof(Cliente)); // Redimensiona o array de clientes

    if (*clientes == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    Cliente *novoCliente = &(*clientes)[*quantidade - 1];
    novoCliente->id = *quantidade;
    
    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", novoCliente->nome); // Lê o nome com espaços

    printf("Digite o CPF do cliente (formato: 000.000.000-00): ");
    scanf(" %[^\n]", novoCliente->cpf);  // Lê o CPF

    printf("Digite o nome do animal: ");
    scanf(" %[^\n]", novoCliente->animal); // Lê o nome do animal com espaços
    
    novoCliente->ativo = 1;

    printf("Cliente cadastrado com sucesso! ID: %d\n", novoCliente->id);
}

// Função para listar todos os clientes
void listarClientes(Cliente *clientes, int quantidade) {
    if (quantidade == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    for (int i = 0; i < quantidade; i++) {
        printf("ID: %d, Nome: %s, CPF: %s, Animal: %s, Status: %s\n",
               clientes[i].id,
               clientes[i].nome,
               clientes[i].cpf,
               clientes[i].animal,
               clientes[i].ativo ? "Ativo" : "Desativado");
    }
}

// Função para consultar um cliente por ID
void consultarCliente(Cliente *clientes, int quantidade) {
    int id;
    printf("Digite o ID do cliente: ");
    scanf("%d", &id);

    for (int i = 0; i < quantidade; i++) {
        if (clientes[i].id == id) {
            printf("ID: %d, Nome: %s, CPF: %s, Animal: %s, Status: %s\n",
                   clientes[i].id,
                   clientes[i].nome,
                   clientes[i].cpf,
                   clientes[i].animal,
                   clientes[i].ativo ? "Ativo" : "Desativado");
            return;
        }
    }
    printf("Cliente com ID %d não encontrado.\n", id);
}

// Função para desativar um cliente
void desativarCliente(Cliente *clientes, int quantidade) {
    int id;
    printf("Digite o ID do cliente que deseja desativar: ");
    scanf("%d", &id);

    for (int i = 0; i < quantidade; i++) {
        if (clientes[i].id == id) {
            clientes[i].ativo = 0;
            printf("Cliente %s desativado com sucesso.\n", clientes[i].nome);
            return;
        }
    }
    printf("Cliente com ID %d não encontrado.\n", id);
}

// Função para excluir um cliente
void excluirCliente(Cliente **clientes, int *quantidade) {
    int id;
    printf("Digite o ID do cliente que deseja excluir: ");
    scanf("%d", &id);

    for (int i = 0; i < *quantidade; i++) {
        if ((*clientes)[i].id == id) {
            // Desloca os clientes após o cliente a ser excluído
            for (int j = i; j < (*quantidade) - 1; j++) {
                (*clientes)[j] = (*clientes)[j + 1];
            }
            (*quantidade)--;
            *clientes = realloc(*clientes, (*quantidade) * sizeof(Cliente)); // Redimensiona o array

            if (*clientes == NULL && *quantidade > 0) {
                printf("Erro ao alocar memória.\n");
                exit(1);
            }

            printf("Cliente excluído com sucesso.\n");
            return;
        }
    }
    printf("Cliente com ID %d não encontrado.\n", id);
}

// Função para simular a venda de um serviço
void venderServico() {
    printf("Serviço vendido com sucesso!\n");
}

void linha() {
    printf("---------------------------------\n");
}

// Função principal
int main() {
    menuPrincipal();
    return 0;
}
