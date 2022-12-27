/* Define as constantes, os tipos de dados e os protótipos de funções do FileSystem.

   Copyright (C) 2022 Vinicius J P Silva
*/
#pragma once 

#include <stdio.h>

#define FALSE 0
#define TRUE 1
#define ERRO_ABRIR_ARQUIVO -1
#define ERRO_CRIAR_ARQUIVO -2
#define ERRO_FECHAR_ARQUIVO -3
#define ERRO_ESCREVER_ARQUIVO -4
#define ERRO_EXCLUIR_ARQUIVO -5
#define ERRO_LER_ARQUIVO -6
#define FIM_DE_ARQUIVO -7

// Número máximo de caracteres no nome do arquivo.
#define TAMANHO_NOME_ARQUIVO 13

// Comprimento da hora de criação do arquivo no formato hh:mm:ss. 
#define TAMANHO_HORA 9

// Tamanho do bloco lógico em bytes.
#define TAMANHO_BLOCO_LOGICO 4096

// Número máximo de arquivos do disco virtual.
#define NUMERO_DE_ARQUIVOS_DO_DISCO 1024

// Número máximo de discos virtuais que podem ser criados.
#define NUMERO_DE_DISCOS 26

// Representação descritiva dos tipos de dados que um arquivo pode armazenar. 
#define TIPO_NUMERICO "Numérico"
#define TIPO_TEXTO "Texto"

// Código UTF-8 do caractere A maiúsculo.
#define CHARACTER_CODE_A 0x41

// Código UTF-8 do caractere Z maiúsculo.
#define CHARACTER_CODE_Z 0x5A

// Indica que o disco virtual deve ser criado.
#define MODE_CREATE_DISK 0 

// Indica que o disco virtual previamente criado pode ser acessado.
#define MODE_GET_DISK 1

// Representa a string do prompt à ser exibido.
#define STR_PROMPT "fs > "

// Representa os comandos do FileSystem.
#define FS_COMMAND_CD "cd"
#define FS_COMMAND_CA "ca"
#define FS_COMMAND_AA "aa"
#define FS_COMMAND_EA "ea"
#define FS_COMMAND_LA "la"
#define FS_COMMAND_FA "fa"
#define FS_COMMAND_DA "da"
#define FS_COMMAND_LD "ld"
#define FS_COMMAND_FD "fd"
#define FS_COMMAND_ET "et"
#define FS_COMMAND_EF "ef"
#define FS_COMMAND_IA "ia"
#define FS_COMMAND_HP "hp"

// Número de comandos.
#define AMOUNT_FS_COMMANDS 13

// Delimitador entre os comandos inseridos no prompt.
#define COMMAND_DELIMITER " "

// Representa o caractere de final de linha '\n'.
#define EOL '\n'

// Representa o caracter de retono ao inicio da linha '\r'.
#define CARRIAGE_RETURN '\r'

// Representa o caracter espaco em branco ' '.
#define CHAR_EMPTY ' '

// Representa o caracter que separa os diretórios em um caminho do sistema linux.
#define CHAR_PATH_SEPARATOR '/'

// Separados dos milhares.
#define CHAR_THOUSAND_SEPARATOR '.'

// Representa o caracter nulo.
#define CHAR_NULO '\0'
#define STR_CHAR_NULO "\0"

// Valor para funções de busca retornarem caso não encontrem determinado elemento.
#define NOT_FOUND -1

// Tamanho máximo de caracteres da linha de comando.
#define COMMAND_LINE_SIZE 1024

// Tamanho máximo de uma palavra.
#define STD_WORD_SIZE 50

// Número máximo de elementos que podem ser escritos por vez.
#define MAX_NUMBER_ELEMENTS 20

// Tamanho mínimo de um disco (1 MB).
#define MIN_DISK_SIZE 1048576

// Tamanho máximo de um disco (30 MB).
#define MAX_DISK_SIZE 31457280

// Tamanho padrão dos comandos do FileSystem.
#define STD_COMMAND_SIZE 10

// Valor que indica a finalização do FileSystem.
#define EXIT_FILE_SYSTEM -1

// Erros
#define NO_ERROR 100
#define ERROR_INCORRECT_SYNTAX 101
#define ERROR_CREATE_DISK 102
#define ERROR_FORMAT_DISK 103
#define ERROR_DISK_NOT_EXIST 104
#define ERROR_CREATE_FILE 105
#define ERROR_OPEN_FILE 106
#define ERROR_CLOSE_FILE 107
#define ERROR_FILE_NOT_EXIST 108
#define ERROR_WRITE_FILE 109
#define ERROR_READ_FILE 110
#define ERROR_DELETE_FILE 111
#define ERROR_NAME_SIZE 112
#define ERROR_DISK_SIZE 113
#define ERROR_CLOSED_FILE 114

// Mensagens
#define ERR_INVALID_COMMAND "\'%s\' não é reconhecido como um comando FileSystem.\nDigite o comando \'hp\' para listar os comandos disponíveis.\n\n"
#define STR_ERR_INCORRECT_SYNTAX "A sintaxe do comando está incorreta.\n\n"
#define STR_ERR_CREATE_DISK "Ocorreu um erro ao criar o disco.\n\n"
#define STR_ERR_FORMAT_DISK "Ocorreu um erro ao formatar o disco.\n\n"
#define STR_ERR_DISK_NOT_EXIST "O disco informado não existe.\n\n"
#define STR_ERR_CREATE_FILE "Ocorreu um erro ao criar o arquivo.\n\n"
#define STR_ERR_OPEN_FILE "Ocorreu um erro ao abrir o arquivo.\n\n"
#define STR_ERR_CLOSE_FILE "Ocorreu um erro ao fechar o arquivo.\n\n"
#define STR_ERR_FILE_NOT_EXIST "O arquivo informado não existe.\n\n"
#define STR_ERR_WRITE_FILE "Ocorreu um erro ao escrever no arquivo.\n\n"
#define STR_ERR_READ_FILE "Ocorreu um erro ao ler o arquivo.\n\n"
#define STR_ERR_DELETE_FILE "Ocorreu um erro ao deletar o arquivo.\n\n"
#define STR_ERR_NAME_SIZE "Operação cancelada. O nome do arquivo é extenso demais.\n\n"
#define STR_ERR_DISK_SIZE "Capacidade inválida! Minimo: %d bytes - Máximo: %d bytes\n\n"
#define STR_ERR_CLOSED_FILE "O arquivo está fechado!\n\n"
#define STR_ERR_EMPTY_FILE "O arquivo \'%s\' está vazio!.\n\n"

#define MSG_WELLCOME "\n\tFile System [versão 1.0.1]\n\tVinicius J P Silva. Todos os direitos reservados.\n\n"
#define MSG_HELP "\n\t> Comandos FileSystem\n\n\tcd\tCria um novo disco.\n\tca\tCria um novo arquivo.\n\taa\tAbre um arquivo.\n\tea\tEscreve em um arquivo\n\tla\tLê um arquivo.\n\tfa\tFecha um arquivo.\n\tda\tDeleta um arquivo.\n\tld\tExibe informações de um disco.\n\tfd\tFormata um disco.\n\tet\tExibe informações de um arquivo.\n\tia\tImporta um arquivo do sistema de arquivos para um disco virtual.\n\tef\tEncerra o FileSystem.\n\n"
#define MSG_FILE_CONTENT "\nConteúdo do arquivo \'%s\':\n"
#define MSG_DISK_IS_FULL "O disco %c atingiu sua capacidade máxima de armazenamento de "
#define MSG_INSUFFICIENT_MEMORY "Não há espaço suficiente! (Espaço disponível "

// Representa um comando.
typedef struct {
	char command[STD_COMMAND_SIZE],
		parameters[COMMAND_LINE_SIZE - STD_COMMAND_SIZE];
} Command; 

// Representa valores Booleanos: 0 (FALSE) e 1 (TRUE).
typedef int Boolean;

// Representa o código dos comandos FileSystem.
typedef enum { CD, CA, AA, EA, LA, FA, DA, LD, FD, ET, EF, IA, HP } FileSystemCommands;

// Representa os tipos de dados que um arquivo pode armazenar.  Os tipos são numérico (int ou float) e string.
typedef enum { NUMERICO, STRING } TipoArquivo;

// Atributos do arquivo
typedef struct 
{
	// Descritor de arquivo (file descriptor - fd) usado para identificar um arquivo na tabela de alocação de arquivos do disco.
	unsigned short fd;

	// Nome e hora de criação do arquivo no formato hh:mm:ss. 
	char nome[TAMANHO_NOME_ARQUIVO], horaCriacao[TAMANHO_HORA];

	// Tamanho do arquivo em bytes.
	unsigned int tamanho; 

	// Número de blocos lógicos do arquivo.
	unsigned int blocosLogicos; 

	// Armazena o conteúdo do arquivo. O tipo de dado do conteúdo é definido pelo campo tipo.
	void* conteudo;

	// Representa o tipo de dado que o arquivo pode armazenar. 
	TipoArquivo tipo;

	// Indica se o arquivo está aberto (TRUE) ou fechado (FALSE).
	Boolean aberto;
} Arquivo;

// Atributos do disco
typedef struct {
	// Capacidade máxima de armazenamento em bytes do disco.
	unsigned int capacidade;

	// Espaço ocupado do disco.
	unsigned int espacoOcupado;
	
	// Espaço livre do disco.
	unsigned int espacoLivre;

	// Número total de blocos lógicos.
	unsigned int blocosLogicosTotais;

	// Número de blocos lógicos ocupados do disco.
	unsigned int blocosLogicosOcupados;

	// Número de blocos lógicos livres do disco.
	unsigned int blocosLogicosLivres;
} Disco;

// Atributos do disco virtual
typedef struct {
	// Letra de identificação do disco.
	char unidade;

	// Dados de armazenamento do disco.
	Disco disco;

	// Número de arquivos do disco.
	unsigned int numeroArquivos;

	// A tabela de alocação de arquivos é usada para armazenar todos os arquivos do disco. 	
	Arquivo tabelaArquivos[NUMERO_DE_ARQUIVOS_DO_DISCO];
} DiscoVirtual;

// Informações sobre o disco virtual
typedef struct {
	DiscoVirtual discoVirtual;
} InfoDisco;

/*
   Cria um disco virtual.
   O tamanho especifica a capacidade máxima de armazenamento em bytes do disco, ou seja, o seu tamanho "físico".
   O parâmetro unidade especifica a letra de identificação do disco, por exemplo C, D, E, etc.
   Retorna um ponteiro para a estrutura de armazenamento criada para o disco, se ocorrer erro retorna NULL.
*/
DiscoVirtual* criaDisco(char unidade, unsigned int tamanho);

/*
   Formata logicamente o disco virtual apagando todo o conteúdo de sua tabela de alocação de arquivos, consequentemente
   apagando todo os arquivos do disco.
   O parâmetro unidade especifica um caractere usado para identificação do disco, por exemplo C, D, E, etc.
   Reinicializa os dados da estrutura do disco em caso de sucesso e retorna TRUE, caso contrário, FALSE.
   Toda a memória alocada dinamicamente para armazenar a estrutura do disco e de seus arquivos devem ser liberadas, ou
   seja, devolvidas ao sistema operacional.
*/
int formatar(char unidade, DiscoVirtual* disco);

/*
   Cria um arquivo para leitura e escrita no disco identificado pela unidade.
   Retorna um inteiro que representa o descritor de arquivo ou ERRO_CRIAR_ARQUIVO se o arquivo não pode ser criado.
 */
int criar(char unidade, const char *nomeArquivo);

/*
   Abre um arquivo para leitura e escrita localizado no disco identificado pela unidade.
   Retorna TRUE se o arquivo foi aberto e ERRO_ABRIR_ARQUIVO se ocorrer algum erro, por exemplo, o arquivo não existe.
*/
int abrir(char unidade, const char* nomeArquivo);

/*
	Escreve no arquivo n elementos obtidos do buffer. O valor de n é definido por tamanho. 	
	O arquivo é identificado pelo descritor de arquivo fd e está localizado no disco identificado pela unidade.
	Retorna o número de bytes escritos após uma escrita bem sucedida. Se ocorrer algum erro na escrita retorna ERRO_ESCREVER_ARQUIVO.
	
    NOTA: O bloco lógico é a unidade básica de alocação de espaço (memória) no disco, portanto sempre que uma escrita é feita no 
	      disco deve-se escrever no mínimo um bloco lógico de tamanho definido em TAMANHO_BLOCO_LOGICO. Sendo assim, quando
	      o usuário solicita que seja escrito um número inteiro de 4 bytes (valor de TAMANHO_BLOCO_LOGICO), usa-se um bloco lógico para 
	      armazená-lo no arquivo, mas quando é escrito a string "Férias!", usa-se 2 blocos lógicos. Como essa string possui 7 caracteres e cada 
	      caractere (char) ocupa 1 byte de memória, ela precisa de 7 bytes de memória. No entanto, como cada bloco lógico possui 4 bytes, 
	      a string exige 2 blocos lógicos (8 bytes) para ser armazenada no disco, logo deve-se escrever dois blocos.
*/
int escrever(char unidade, unsigned short fd, const void* buffer, unsigned int tamanho);

/*
	Lê n elementos do arquivo e armazena no buffer. O valor de n é definido por tamanho. 
	O arquivo é identificado pelo descritor de arquivo fd e está localizado no disco identificado pela unidade.
	Retorna o número de elementos lidos do arquivo em uma leitura bem sucedida.
	Retorna FIM_DE_ARQUIVO se o fim do arquivo for atingido após a leitura e ERRO_LER_ARQUIVO se ocorrer algum erro na leitura.

	NOTA: O bloco lógico é a unidade básica de alocação de espaço (memória) no disco, portanto sempre que uma leitura é feita no 
	      disco deve-se ler no mínimo um bloco lógico de tamanho definido em TAMANHO_BLOCO_LOGICO. Sendo assim, quando
	      o usuário solicita que seja lido um número inteiro de 4 bytes (valor de TAMANHO_BLOCO_LOGICO), lê-se um bloco lógico para 
	      recuperá-lo do arquivo, mas quando é lido a string "Férias!", lê-se 2 blocos lógicos. Como essa string possui 7 caracteres e cada 
	      caractere (char) ocupa 1 byte de memória, ela precisa de 7 bytes de memória. No entanto, como cada bloco lógico possui 4 bytes, 
	      a string exige 2 blocos lógicos (8 bytes) para ser armazenada no disco, logo deve-se ler os dois blocos.
*/
int ler(char unidade, unsigned short fd, void** buffer, unsigned int tamanho);

/*
   Fecha um arquivo identificado pelo seu descritor (fd) e que está localizado no disco representado por unidade.
   Retorna TRUE se a operação for bem sucedida e ERRO_FECHAR_ARQUIVO se não conseguir fechar o arquivo.
*/
int fechar(char unidade, unsigned short fd);

/*
   Apaga um arquivo que está localizado no disco identificado por unidade.
   Retorna TRUE se a operação for bem sucedida e ERRO_EXCLUIR_ARQUIVO se não conseguir excluir o arquivo do disco virtual.
*/
int excluir(char unidade, const char* nomeArquivo);

/*
   Obtém um relatório com as seguintes informações:

   - Dados do Arquivo
   --------------------
   1. nome;
   2. tipo;
   3. tamanho em bytes;
   4. número de blocos lógicos;
   5. hora de criação.

   - Dados do Disco
   ------------------
   1. número de arquivos;
   2. a capacidade de armazenamento;
   3. o espaço ocupado do disco;
   4. o espaço livre do disco;
   5. o número total de blocos lógicos;
   6. o número de blocos lógicos ocupados;
   7. o número de blocos lógicos livres.

   Os dados do arquivo devem ser obtidos para cada arquivo do disco identificado por unidade. 
   Retorna o conteúdo do relatório. Se ocorrer um erro ao obter esses dados retorna NULL.
*/
InfoDisco* infoDisco(char unidade);

/* Cria ou obtém o endereço da estrutura de armazenamento do disco identificado por drive, que representa sua letra de identificação, por exemplo C, D, E, etc.
   O parâmetro mode informa o modo de acesso ao disco virtual, os valores válidos são: MODE_CREATE_DISK e MODE_GET_DISK 1.

   Se mode for MODE_CREATE_DISK e a letra de identificação do drive for válida, a função cria o disco e retorna um ponteiro para sua estrutura de armazenamento.
   Se mode for MODE_GET_DISK e a letra de identificação do drive for válida, a função obtém o ponteiro para a estrutura de armazenamento do disco previamente criada.

   Se se ocorrer algum erro na criação ou no acesso ao disco, ou se a identificação do drive for inválida será retornado NULL.
*/
DiscoVirtual* getVirtualDisk(char drive, char mode);

/*

*/
int filesystem();

/*
	Exibe o prompt na saída padrão (stdout).
*/ 
void printPrompt(const char *prompt);

/*
	Lê um comando (string) do dispositivo de entrada padrão (stdin).
	> A função utiliza alocação dinâmica para retornar o endereço da string que contém o comando, é necessário liberar após utilizá-la (free).
	Retorna: 
		Ponteiro (char *) para a string que contém o comando lido.
*/
char * readCommand();

/*
	Separa o comando de seus parâmetros obtidos da linha de comando (commandLine) e os armazena na struct command.
	Retorna:
		TRUE caso consiga obter pelo menos 1 parâmetro.
		FALSE caso não obtenha nenhum parâmetro.
*/
int getCommand(const char* commandLine, Command* command);

/*
	Executa o comando informado.
	Retorna:
		TRUE caso seja um comando tipo FileSystem válido.
		FALSE caso não seja um comando tipo FileSystem válido.
		END_FILE_SYSTEM caso o comando executado seja FS_COMMAND_EF.
*/
int runCommand(Command command);

/*
	Função responsável por tentar criar um novo disco para o FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemCreateDisk(const char* parameters);

/*
	Função responsável por formatar um disco já existente do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemFormatDisk(const char* parameters);

/*
	Função responsável por tentar criar um novo arquivo em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros e cria um arquivo de tamanho inicial igual à 0.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemCreateFile(const char* parameters);

/*
	Função responsável por tentar abrir um arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemOpenFile(const char* parameters);

/*
	Função responsável por tentar escrever os dados fornecidos na lina de comando em um arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemWriteFile(const char* parameters);

/*
	Escreve os dados do tipo número inteiro (int) ou real (float) no arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int writeNumberFile(const char* fileName, const char diskUnity, char* parameters);

/*
	Escreve os dados do tipo texto (char[]) no arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int writeStringFile(const char* fileName, const char diskUnity, const char* parameters);

/*
	Função responsável por exibir no dispositivo de saída padrão (stdout) o conteúdo de um arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemReadFile(const char* parameters);

/*
	Função responsável por fechar um arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemCloseFile(const char* parameters);

/*
	Função responsável por deletar um arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemDeleteFile(const char* parameters);

/*
	Função responsável por exibir o tamanho e o número de blocos de um arquivo armazenado em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemDisplayFileSize(const char* parameters);

/*
	Exibe na saída padrão (stdout) as informações de determinado disco.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemShowDiskInformation(const char* parameters);

/*
	Função responsável por finalizar corretamente todo o sistema FileSystem.
	Libera toda a memória alocada para os discos e seus respectivos arquivos, ou seja, todos os dados serão apagados.
	Retorna sempre EXIT_FILE_SYSTEM
*/
int finalizeFileSystem();

/*
	Importa um arquivo do sistema operacional Linux e tenta armazená-lo em determinado disco do FileSystem.
	Efetua todas as validações dos parâmetros.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int fileSystemImportFile(const char* parameters);

/*
	Valida se o primeiro parâmetro equivale à uma unidade válida de disco no padrão <unidade> <outros parâmetros>:
	> Verifica se a unidade possui apenas uma letra.
	> Obtém o caracter da unidade.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int validateDiskParameters(const char* parameters, char* diskUnity);

/*
	Valida se os parâmetros equivalem à um comando manipulador de arquivo no padrão <unidade> <nome arquivo> :
	> Verifica se a unidade possui apenas uma letra e se o seu respectivo disco existe.
	> Obtém o caracter da unidade e o nome do arquivo.
	Retorna:
		NO_ERROR caso execute sem erros.
		Código do erro.
*/
int validateFileParameters(const char* parameters, char* diskUnity, char* fileName);

/*
	Inicializa um disco virtual com os valores padrão definidos nas constantes deste arquivo.
*/
void initializeVirtualDisk(DiscoVirtual* virtualDisk, const char validUnity, unsigned int tamanho);

/*
	Atualiza as informações do disco, baseado no tamanho e na quantidade de blocos utilizados por seus respectivos arquivos.
*/
void updateVirtualDisk(DiscoVirtual* virtualDisk);

/*
	Cria, inicializa e retorna um arquivo recém criado com os valores padrão.
	Retorna o arquivo criado.
*/
Arquivo newInitializedFile(const char* fileName);

/*
	Gera um novo descritor para o arquivo e o armazena na tabela de arquivos.
	Retorna:
		Inteiro que representa o descritor de arquivo (index na tabela) 
		ERRO_CRIAR_ARQUIVO se o arquivo não pode ser criado.
*/
int saveFile(const Arquivo file, DiscoVirtual* virtualDisk);

/*
	Busca um arquivo especificado pelo nome (nomeArquivo) dentre os arquivos armazenados no disco virtual informado.
	Retorna:
		Um ponteiro para sua estrutura do arquivo, caso encontre.
		NULL caso não encontre.
*/
Arquivo* getFileByName(const char* fileName, DiscoVirtual* virtualDisk);

/*
	Converte uma string para um valor inteiro.
	Retorna:
		TRUE caso consiga converter.
		FALSE caso não consiga converter.
*/
Boolean stringToInteger(const char* str, int* number);

/*
	Exibe no dispositivo de saída padrão (stdout) uma mensagem correspondente ao erro informado.
	Caso o codigo do erro não exista a função não exibe nada.
*/
void printError(const unsigned int errorCode);

/*
	Exibe no dispositivo de saída padrão (stdout) um relatório com as informações sobre o disco e cada arquivo armazenado nele.
	Não efetua nenhum tipo de validação.
*/
void printDiskInformation(InfoDisco diskInformation);

/*
	Exibe no dispositivo de saída padrão (stdout) uma mensagem pedindo confirmação para efetivar determinada ação.
	Returna:
		TRUE caso escolham sim (S).
		FALSE caso escolham não (N).
*/
Boolean confirmChoice();

/*
	Verifica se há espaço suficiente para armazenar 'size' bytes. Checa tanto a memória reservada pelos blocos do arquivo quanto a memória disponível no disco.
	Exibe no dispositivo de saída padrão (stdout) informando o status da capacidade de armazenamento do disco informado, comparado com o tamanho especificado.
	Retorna:
		TRUE caso haja espaço para armazenar 'size' bytes.
		FALSE caso não haja espaço.
*/
Boolean checkDiskCapacity(DiscoVirtual* virtualDisk, const Arquivo file, const unsigned int size);

/*
	Tenta converter uma string para float e armazena em 'number'.
	Pode ser utilizada para apenas checar se uma string é um valor float, basta passar NULL para o segundo parâmetro formal.
		Ex.: 
			char str[] = {"123"};
			stringToFloat(str, NULL);
	Retorna:
		TRUE caso a conversão ocorra.
		FALSE caso não ocorra.
*/
Boolean stringToFloat(const char* buffer, float* number);

/*
	Verifica se a string informada equivale à um dos comandos FileSystem.
	Retorna:
		FileSystemCommand equivalente ao comando informado, caso seja válido.
		NOT_FOUND caso seja inválido.
*/
int isValidCommand(const char* command);

/*
	Obtém a hora do sistema operacional e a copia para o vetor 'buffer'.
	Formato da hora: HH:MM:SS.
*/
void getCurrentTime(char* buffer);

/*
    Remove o caracter de nova linha ('\n') de uma string, caso exista.
    Retorna: TRUE caso consiga remover ou FALSE caso não remova.
*/
Boolean removeEnterChar(char *str);

/*
    Verifica se a string (vetor de caracteres) está vazia, ou seja, seu primeiro caracter é um '\n' ou '\0'.
    Retorna TRUE caso esteja vazia ou FALSE caso não.
*/
Boolean isEmptyString(const char *str);

/*
	Verifica se o comando informado é EF (Encerra FileSystem).
	Retorna:
		TRUE caso seja.
		FALSE caso não seja.
*/
Boolean isExitFileSystem(const char *str);

/*
	Libera a memória alocada cujo um ponteiro tipo char referencia.
	Retorna: NULL
*/
void * deallocateString(char *str);

/*
    Função responsável por formatar e exibir na saida padrão um valor natural, utilizando o modelo de separação de milhar.
    Exemplo:
        Caso o usuário forneça o número 1000 e o caracter separador '.', a função exibe na saída padrão o texto "1.000"
*/
void printInt(unsigned long long numero, const char separador);

/*
    Função responsável por formatar e exibir na saida padrão um valor decimal com até X casas decimais, definido pelo parâmetro 'decimalPlaces'.
    Exemplo:
        Caso o usuário forneça o número decimal 1.00, é exibido na tela o texto "1".
		Caso o usuário forneça o número decimal 2.50 e o valor 3 para casas decimais, é exibido na tela o texto "2.500".
*/
void printFloat(const float number, const int decimalPlaces);

/*
	Conta e retorna a quantidade de dígitos que um número inteiro possui.
	Exemplo:
		número 3 retorna 1
		número 1345 retorna 4
*/
int countDigits(const int number);

/*
	Abre, lê, armazena e retorna o conteúdo de determinado arquivo do sistema operacional.
	> A função utiliza alocação dinâmica para retornar o endereço da string que contém o conteúdo, é necessário liberar após utilizá-la (free).
	Retorna:
		char*: ponteiro para a string contendo o conteúdo do arquivo.
		NULL caso ocorra algum erro.
*/
char* readFile(const char* filePath);