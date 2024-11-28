#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <windows.h>

// Estrutura para armazenar os dados do cliente
typedef struct {
    int id;
    char nome[50];
    char cpf[15];  // Campo para o CPF (11 dígitos + pontos e traço)
    char animal[50];
    char tipoanimal[50];
    int ativo; // 1 para ativo, 0 para desativado
} Cliente;

// Declaraï¿½ï¿½o das funï¿½ï¿½es
int validarCPF(const char *cpf);
void cadastrarCliente(Cliente **clientes, int *quantidade);
void listarClientes(Cliente *clientes, int quantidade);
void consultarCliente(Cliente *clientes, int quantidade);
void desativarCliente(Cliente *clientes, int quantidade);
void excluirCliente(Cliente **clientes, int *quantidade);
void venderServico(Cliente *clientes, int quantidade);
void salvarClientesEmArquivo(Cliente *clientes, int quantidade);
void salvarServicoVendido(int idCliente, char *descricaoServico, float preco);
void linha();
void menuPrincipal();
void introducao();

void introducao() {
    const char* titulo = "PetShop MaulFloresta\n\n";
    const char* nome1 = "JoÃ£o Franco Pinheiro RGM: 38400227\n";
    const char* nome2 = "Guilherme Caleffi Pellens Pereira RGM: 38400227\n";

    // Exibir tÃ­tulo letra por letra
    for (int i = 0; i < strlen(titulo); i++) {
        printf("%c", titulo[i]);
        fflush(stdout); // ForÃ§a a saÃ­da imediatamente
        usleep(100000); // Espera de 0.1 segundo entre cada letra
    }
    
    // Exibir o nome 1 letra por letra criando um efeito de animação
    for (int i = 0; i < strlen(nome1); i++) {
        printf("%c", nome1[i]);
        fflush(stdout);
        usleep(100000); 
    }

    // Repetindo a primeira instrução só que com o segundo nome
    for (int i = 0; i < strlen(nome2); i++) {
        printf("%c", nome2[i]);
        fflush(stdout);
        usleep(100000);
    }

    sleep(2); // Espera de 2 segundos antes de seguir para o menu
    menuPrincipal(); // Chama o menu principal
}

// Funï¿½ï¿½o principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    introducao();
    return 0;
}

// Funï¿½ï¿½o para imprimir uma linha separadora
void linha() {
    printf("---------------------------------\n");
}

// Funï¿½ï¿½o para mostrar o menu principal
void menuPrincipal() {
    Cliente *clientes = NULL;
    int quantidade = 0;
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
            printf("Entrada invï¿½lida. Tente novamente.\n");
            int ch;
            fflush(stdin);
            continue;
        }

        linha();

        switch (opcao) {
            case 1: cadastrarCliente(&clientes, &quantidade); break;
            case 2: listarClientes(clientes, quantidade); break;
            case 3: consultarCliente(clientes, quantidade); break;
            case 4: desativarCliente(clientes, quantidade); break;
            case 5: excluirCliente(&clientes, &quantidade); break;
            case 6: venderServico(clientes, quantidade); break;
            case 7: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n"); break;
        }

        sleep(2);
        linha();
    } while (opcao != 7);

    free(clientes);
}

// Funï¿½ï¿½o para validar o CPF
int validarCPF(const char *cpf) {
    int i, j, soma = 0, digito1, digito2;
    char cpfNumerico[12];

    int k = 0;
    for (i = 0; cpf[i] != '\0'; i++) {
        if (isdigit(cpf[i])) {
            cpfNumerico[k++] = cpf[i];
        }
    }
    cpfNumerico[k] = '\0';

    if (strlen(cpfNumerico) != 11) {
        return 0; // CPF invï¿½lido
    }

    for (i = 1; i < 11; i++) {
        if (cpfNumerico[i] != cpfNumerico[0]) {
            break;
        }
    }
    if (i == 11) {
        return 0;
    }

    for (i = 0, j = 10; i < 9; i++, j--) {
        soma += (cpfNumerico[i] - '0') * j;
    }
    digito1 = (soma * 10) % 11;
    if (digito1 == 10) digito1 = 0;

    soma = 0;
    for (i = 0, j = 11; i < 10; i++, j--) {
        soma += (cpfNumerico[i] - '0') * j;
    }
    digito2 = (soma * 10) % 11;
    if (digito2 == 10) digito2 = 0;

    return (cpfNumerico[9] - '0' == digito1 && cpfNumerico[10] - '0' == digito2);
}

// Funï¿½ï¿½o para salvar os clientes em um arquivo
void salvarClientesEmArquivo(Cliente *clientes, int quantidade) {
    FILE *arquivo = fopen("clientes.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravaï¿½ï¿½o.\n");
        return;
    }

    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "ID: %d, Nome: %s, CPF: %s, Animal: %s, Tipo: %s, Status: %s\n",
                clientes[i].id,
                clientes[i].nome,
                clientes[i].cpf,
                clientes[i].animal,
                clientes[i].tipoanimal,
                clientes[i].ativo ? "Ativo" : "Desativado");
    }

    fclose(arquivo);
}

// Funï¿½ï¿½o para cadastrar um cliente
void cadastrarCliente(Cliente **clientes, int *quantidade) {
    (*quantidade)++;
    *clientes = realloc(*clientes, (*quantidade) * sizeof(Cliente));

    if (*clientes == NULL) {
        printf("Erro ao alocar memï¿½ria.\n");
        exit(1);
    }

    Cliente *novoCliente = &(*clientes)[*quantidade - 1];
    novoCliente->id = *quantidade;

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", novoCliente->nome);

    do {
        printf("Digite o CPF do cliente (formato: 000.000.000-00): ");
        scanf(" %[^\n]", novoCliente->cpf);

        if (!validarCPF(novoCliente->cpf)) {
            printf("CPF invï¿½lido. Tente novamente.\n");
        }
    } while (!validarCPF(novoCliente->cpf));

    printf("Digite o nome do animal: ");
    scanf(" %[^\n]", novoCliente->animal);

    printf("Digite o tipo de animal: ");
    scanf(" %[^\n]", novoCliente->tipoanimal);

    novoCliente->ativo = 1;

    printf("Cliente cadastrado com sucesso! ID: %d\n", novoCliente->id);

    salvarClientesEmArquivo(*clientes, *quantidade);
}

// Funï¿½ï¿½o para listar os clientes
void listarClientes(Cliente *clientes, int quantidade) {
    if (quantidade == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    int opcaoOrdenacao;
    printf("Escolha a ordem para listar os clientes:\n");
    printf("1. Ordenar por ID\n");
    printf("2. Ordenar alfabeticamente\n");
    printf("Escolha uma opcao: ");
    
    if (scanf("%d", &opcaoOrdenacao) != 1) {
        printf("Entrada invï¿½lida.\n");
        return;
    }

    if (opcaoOrdenacao == 1) {
        Cliente temp;
        for (int i = 0; i < quantidade - 1; i++) {
            for (int j = 0; j < quantidade - 1 - i; j++) {
                if (clientes[j].id > clientes[j + 1].id) {
                    temp = clientes[j];
                    clientes[j] = clientes[j + 1];
                    clientes[j + 1] = temp;
                }
            }
        }
        printf("Clientes ordenados por ID.\n");
    } else if (opcaoOrdenacao == 2) {
        Cliente temp;
        for (int i = 0; i < quantidade - 1; i++) {
            for (int j = 0; j < quantidade - 1 - i; j++) {
                if (strcmp(clientes[j].nome, clientes[j + 1].nome) > 0) {
                    temp = clientes[j];
                    clientes[j] = clientes[j + 1];
                    clientes[j + 1] = temp;
                }
            }
        }
        printf("Clientes ordenados alfabeticamente.\n");
    } else {
        printf("Opï¿½ï¿½o invï¿½lida.\n");
        return;
    }

    printf("======== LISTA DE CLIENTES ========\n");
    for (int i = 0; i < quantidade; i++) {
        printf("ID: %d, Nome: %s, CPF: %s, Animal: %s, Tipo: %s, Status: %s\n",
               clientes[i].id,
               clientes[i].nome,
               clientes[i].cpf,
               clientes[i].animal,
               clientes[i].tipoanimal,
               clientes[i].ativo ? "Ativo" : "Desativado");
    }
}

// Funï¿½ï¿½o para consultar um cliente pelo CPF
void consultarCliente(Cliente *clientes, int quantidade) {
    char cpf[15];
    printf("Digite o CPF do cliente (formato: 000.000.000-00): ");
    scanf(" %[^\n]", cpf);

    for (int i = 0; i < quantidade; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            printf("Cliente encontrado:\n");
            printf("ID: %d, Nome: %s, CPF: %s, Animal: %s, Tipo: %s, Status: %s\n",
                   clientes[i].id,
                   clientes[i].nome,
                   clientes[i].cpf,
                   clientes[i].animal,
                   clientes[i].tipoanimal,
                   clientes[i].ativo ? "Ativo" : "Desativado");
            return;
        }
    }

    printf("Cliente nï¿½o encontrado.\n");
}

// Funï¿½ï¿½o para desativar um cliente
void desativarCliente(Cliente *clientes, int quantidade) {
    int id;
    printf("Digite o ID do cliente para desativar: ");
    if (scanf("%d", &id) != 1 || id <= 0 || id > quantidade) {
        printf("ID invï¿½lido.\n");
        return;
    }

    if (clientes[id - 1].ativo == 0) {
        printf("Cliente jï¿½ estï¿½ desativado.\n");
    } else {
        clientes[id - 1].ativo = 0;
        printf("Cliente desativado com sucesso.\n");
    }
}

// Funï¿½ï¿½o para excluir um cliente
void excluirCliente(Cliente **clientes, int *quantidade) {
    int id;
    printf("Digite o ID do cliente para excluir: ");
    if (scanf("%d", &id) != 1 || id <= 0 || id > *quantidade) {
        printf("ID invï¿½lido.\n");
        return;
    }

    for (int i = id - 1; i < *quantidade - 1; i++) {
        (*clientes)[i] = (*clientes)[i + 1];
    }

    (*quantidade)--;
    *clientes = realloc(*clientes, (*quantidade) * sizeof(Cliente));

    if (*clientes == NULL && *quantidade > 0) {
        printf("Erro ao alocar memï¿½ria.\n");
        exit(1);
    }

    printf("Cliente excluï¿½do com sucesso.\n");
}

// Funï¿½ï¿½o para vender um serviï¿½o para um cliente
void venderServico(Cliente *clientes, int quantidade) {
   int id, opcao, opcao2;
    float preco;
    char descricaoServico[50];

    if (quantidade == 0) {
        printf("Nenhum cliente cadastrado. NÃ£o Ã© possÃ­vel realizar vendas.\n");
        return;
    }

    printf("Digite o ID do cliente para a venda de serviÃ§o: ");
    if (scanf("%d", &id) != 1 || id < 1 || id > quantidade) {
        printf("ID invÃ¡lido. OperaÃ§Ã£o cancelada.\n");
        return;
    }

    if (!clientes[id - 1].ativo) {
        printf("O cliente estÃ¡ desativado. NÃ£o Ã© possÃ­vel vender um serviÃ§o.\n");
        return;
    }

    printf("Cliente selecionado: %s (CPF: %s)\n", clientes[id - 1].nome, clientes[id - 1].cpf);

    printf("1. AcessÃ³rios\n");
    printf("2. RaÃ§Ãµes\n");
    printf("3. Misturas\n");
    printf("4. Roupas\n");
    printf("5. Banho & Tosa\n");
    printf("6. Higiene\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    linha();

    switch (opcao) {
        case 1:
            printf("1. Brinquedos - R$ 15,00\n");
            printf("2. Camas - R$ 25,00\n");
            printf("3. Mantas - R$ 20,00\n");
            printf("4. Pedras para aquÃ¡rio - R$ 5,00\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao2);
            switch (opcao2) {
                case 1: preco = 15.00; strcpy(descricaoServico, "Brinquedos"); break;
                case 2: preco = 25.00; strcpy(descricaoServico, "Camas"); break;
                case 3: preco = 20.00; strcpy(descricaoServico, "Mantas"); break;
                case 4: preco = 5.00; strcpy(descricaoServico, "Pedras para aquÃ¡rio"); break;
                default: printf("OpÃ§Ã£o invÃ¡lida.\n"); return;
            }
            break;
        case 2:
            printf("1. RaÃ§Ã£o para gatos - R$ 20,50\n");
            printf("2. RaÃ§Ã£o para cachorros - R$ 22,50\n");
            printf("3. RaÃ§Ã£o para herbÃ­voros - R$ 14,90\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao2);
            switch (opcao2) {
                case 1: preco = 20.50; strcpy(descricaoServico, "RaÃ§Ã£o para gatos"); break;
                case 2: preco = 22.50; strcpy(descricaoServico, "RaÃ§Ã£o para cachorros"); break;
                case 3: preco = 14.90; strcpy(descricaoServico, "RaÃ§Ã£o para herbÃ­voros\n"); break;
                default: printf("OpÃ§Ã£o invÃ¡lida.\n"); return;
            }
            break;
        case 3:
            printf("1. Mistura para gatos - R$ 11,90\n");
            printf("2. Mistura para cachorros - R$ 12,50\n");
            printf("3. Mistura para herbÃ­voros - R$ 10,40\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao2);
            switch (opcao2) {
                case 1: preco = 11.00; strcpy(descricaoServico, "Mistura para gatos"); break;
                case 2: preco = 12.50; strcpy(descricaoServico, "RaÃ§Ã£o para cachorros"); break;
                case 3: preco = 10.40; strcpy(descricaoServico, "RaÃ§Ã£o para pÃ¡ssaros\n"); break;
                default: printf("OpÃ§Ã£o invÃ¡lida.\n"); return;
            }
            break;
        case 4:
            printf("1. Baratas - R$ 12,50\n");
            printf("2. Alta qualidade - R$ 22,50\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao2);
            switch (opcao2) {
                case 1: preco = 12.50; strcpy(descricaoServico, "Roupas baratas"); break;
                case 2: preco = 22.50; strcpy(descricaoServico, "Roupas de alta qualidade"); break;
                default: printf("OpÃ§Ã£o invÃ¡lida.\n"); return;
            }
            break;
        case 5:
            printf("1. Banho - R$ 35,50\n");
            printf("2. Tosa - R$ 25,50\n");
            printf("3. Banho & Tosa - R$ 50,90\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao2);
            switch (opcao2) {
                case 1: preco = 35.50; strcpy(descricaoServico, "Banho"); break;
                case 2: preco = 25.50; strcpy(descricaoServico, "Tosa"); break;
                case 3: preco = 50.90; strcpy(descricaoServico, "Banho & Tosa\n"); break;
                default: printf("OpÃ§Ã£o invÃ¡lida.\n"); return;
            }
            break;
        case 6:
            printf("1. Xixi nÃ£o pode - R$ 12,40\n");
            printf("2. Kit 30 Tapetes SanitÃ¡rios - R$ 20,50\n");
            printf("3. Higienizador de orelhas - R$ 15,90\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao2);
            switch (opcao2) {
                case 1: preco = 12.40; strcpy(descricaoServico, "Xixi nÃ£o pode"); break;
                case 2: preco = 20.50; strcpy(descricaoServico, "Kit 30 Tapetes SanitÃ¡rios"); break;
                case 3: preco = 15.90; strcpy(descricaoServico, "Higienizador de orelhas\n"); break;
                default: printf("OpÃ§Ã£o invÃ¡lida.\n"); return;
            }
            break;
        case 7:
            printf("OperaÃ§Ã£o cancelada.\n");
            return;
        default:
            printf("OpÃ§Ã£o invÃ¡lida. OperaÃ§Ã£o cancelada.\n");
            return;
    }

    // Registra a venda
    printf("ServiÃ§o '%s' vendido por R$%.2f ao cliente ID %d.\n", descricaoServico, preco, id);
    salvarServicoVendido(id, descricaoServico, preco);
}

// Funï¿½ï¿½o para salvar o serviï¿½o vendido em um arquivo
void salvarServicoVendido(int idCliente, char *descricaoServico, float preco) {
    FILE *arquivo = fopen("servicos_vendidos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravaï¿½ï¿½o.\n");
        return;
    }

    fprintf(arquivo, "Cliente ID: %d, Serviï¿½o: %s, Preï¿½o: %.2f\n", idCliente, descricaoServico, preco);
    fclose(arquivo);
}
