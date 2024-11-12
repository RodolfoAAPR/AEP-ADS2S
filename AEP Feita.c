#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>

#define FILE_NAME "dados.txt"

struct dados_usuario {
    char user[40];
    char senha[40];
    char senhaCripto[40];
    char senhaDescripto[40];
    int data;
    char dataNascimento[11];
};
struct dados_usuario d_u;

void menu();
void CadastrarUsuario();
void ExcluirCadastro();
void AlterarCadastro();
void CripCadastro(char senha[40], int soma_data);
void DescripCadastro();

void menu() {
    system("color 02");
    int acoes;
    printf("Seja bem-vindo ao MENU!");
    printf("\n1- Adicionar novo cadastro.");
    printf("\n2- Alterar cadastro já realizado.");
    printf("\n3- Excluir cadastro já realizado.");
    printf("\n4- Exibir todos os cadastros criptografados.");
    printf("\n5- Exibir todos os cadastros descriptografados.");
    printf("\n6- Sair.");
    printf("\nOpção escolhida: ");
    scanf("%i", &acoes);

    switch (acoes) {
        case 1:
            CadastrarUsuario();
            break;
        case 2:
            AlterarCadastro();
            break;
        case 3:
            ExcluirCadastro();
            break;
        case 4:
            system("cls");
			printf("Login: %s\n", d_u.user);
			printf("Senha: %s\n\n", d_u.senhaCripto);
            sleep(2);
            menu();
            break;
        case 5:
            DescripCadastro();
            system("cls");
            printf("Login: %s\n", d_u.user);
			printf("Senha descriptografada: %s\n", d_u.senhaDescripto);
            sleep(2);
			menu();  
            break;
        case 6:
            exit(0);
            break;
        default:
            system("cls");
            printf("Opção inválida!\nAguarde...");
            sleep(2);  
            menu();
            break;
    }
}

void CadastrarUsuario() {
	
	FILE *file = fopen(FILE_NAME, "a");
	
	if (file == NULL){
		printf("ERRO AO ABRIR O ARQUIVO!\n");
		return;
	}
	
    system("cls");
    printf("----SEJA BEM VINDO AO CADASTRO----\n");
    printf("Insira o login: ");
    scanf("%s", d_u.user);
    printf("Insira a senha: ");
    scanf("%s", d_u.senha);
    printf("Insira a data de nascimento (dd/mm/aaaa): ");
    scanf("%s", d_u.dataNascimento);
    
    fprintf(file, "%s %s %s\n", d_u.user, d_u.senha, d_u.dataNascimento);
    fclose(file);

    // Calcular a soma dos dígitos da data de nascimento
    int soma_data = 0, i;
    for (i = 0; i < strlen(d_u.dataNascimento); i++) {
        if (d_u.dataNascimento[i] >= '0' && d_u.dataNascimento[i] <= '9') {
            soma_data += d_u.dataNascimento[i] - '0';
        }
	}
    d_u.data = soma_data;

    CripCadastro(d_u.senha, d_u.data);

    system("cls");
    printf("Cadastro realizado com sucesso!\n");
    printf("Aguarde...");
    sleep(2);  
    system("cls");
    menu();
}

void AlterarCadastro() {
    system("cls");
    char senha[40];
    
    FILE *file = fopen(FILE_NAME, "r");
    FILE *tempfile = fopen("temp.txt", "w");
    
    printf("Insira a senha para alterar o cadastro: ");
    scanf("%s", senha);

    if (strcmp(senha, d_u.senha) == 0) {
        printf("Cadastro encontrado. O que deseja alterar?\n");
        printf("1- Alterar login\n");
        printf("2- Alterar senha\n");
        printf("3- Alterar login e senha\n");
		printf("Escolha a opção: ");
        int escolha;
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Insira o novo login: ");
                scanf("%s", d_u.user);
                printf("Login alterado com sucesso!\n");
                menu();
                break;
            case 2:
                printf("Insira a nova senha: ");
                scanf("%s", d_u.senha);
                printf("Senha alterada com sucesso!\n");
                CripCadastro(d_u.senha, d_u.data);
                menu();
                break;
            case 3:
            	printf("Insira o novo login: ");
            	scanf("%s", d_u.user);
            	printf("Insira a nova senha: ");
            	scanf("%s", d_u.senha);
            	CripCadastro(d_u.senha, d_u.data);
            	menu();
            	break;
			default:
                printf("Opção inválida.\n");
                menu();
                break;
        }
        sleep(2);
    } else {
        printf("Senha incorreta. Não foi possível alterar o cadastro.\n");
        sleep(2);
        menu();
    }

    system("cls");
    menu();
}



void ExcluirCadastro() {
    system("cls");
    char senha[40];
    printf("Insira a senha para excluir o cadastro: ");
    scanf("%s", senha);

    if (strcmp(senha, d_u.senha) == 0) {
        strcpy(d_u.user, "");
        strcpy(d_u.senha, "");
        strcpy(d_u.senhaCripto, "");
        strcpy(d_u.dataNascimento, "");
        d_u.data = 0;

        system("cls");
        printf("Cadastro excluído com sucesso!\n");
        sleep(2000);  
    } else {
        system("cls");
        printf("Senha incorreta. Não foi possível excluir o cadastro.\n");
        sleep(2000); 
    }
    system("cls");
    menu();
}

void CripCadastro(char senha[40], int soma_data) {
    strcpy(d_u.senhaCripto, senha);
    
    //tive que procurar como normalizar o deslocamento para garantir que fique no intervalo de 0-25 (alfabeto)
    int deslocamento = soma_data % 26; // 26 pra evitar deslocamento grande

    int i;
    for (i = 0; i < strlen(d_u.senhaCripto); i++) {
        if (d_u.senhaCripto[i] >= 'A' && d_u.senhaCripto[i] <= 'Z') {
            d_u.senhaCripto[i] = ((d_u.senhaCripto[i] - 'A' + deslocamento) % 26) + 'A';
        } else if (d_u.senhaCripto[i] >= 'a' && d_u.senhaCripto[i] <= 'z') {
            d_u.senhaCripto[i] = ((d_u.senhaCripto[i] - 'a' + deslocamento) % 26) + 'a';
        }
    }
}


void DescripCadastro() {
    strcpy(d_u.senhaDescripto, d_u.senhaCripto);
    
    //tive que procurar como normalizar o deslocamento de novo para garnatir uqe fique no intervalo 0-25 (alfabeto)
    int deslocamento = d_u.data % 26; // usar valor da data como deslocamento

    int i;
    for (i = 0; i < strlen(d_u.senhaDescripto); i++) {
        if (d_u.senhaDescripto[i] >= 'A' && d_u.senhaDescripto[i] <= 'Z') {
            // Desfazer o deslocamento
            d_u.senhaDescripto[i] = ((d_u.senhaDescripto[i] - 'A' - deslocamento + 26) % 26) + 'A';
        } else if (d_u.senhaDescripto[i] >= 'a' && d_u.senhaDescripto[i] <= 'z') {
            // Desfazer o deslocamento
            d_u.senhaDescripto[i] = ((d_u.senhaDescripto[i] - 'a' - deslocamento + 26) % 26) + 'a';
        }
    }
    d_u.senhaDescripto[strlen(d_u.senhaCripto)] = '\0'; // Garantir que a string seja terminada corretamente
    printf("Senha descriptografada: %s\n", d_u.senhaDescripto); // Exibe a senha descriptografada
}


int main() {
    setlocale(LC_ALL, "Portuguese");
    menu();
    return 0;
}

