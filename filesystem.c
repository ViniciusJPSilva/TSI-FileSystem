
/* Implementa as funções do filesystem.

   Copyright (C) 2022 Vinicius J P Silva
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include "filesystem.h"

int main() {
   return filesystem();
}

int filesystem() {
   char *commandLine;
   Command command;
   int runStatus;

   setlocale(LC_ALL, "pt-BR");

   printf(MSG_WELLCOME);

   while(TRUE){
      printPrompt(STR_PROMPT);
      commandLine = readCommand();

      if(!commandLine) continue; // Verifica se a linha de comando está vazia, igual à NULL
      
      getCommand(commandLine, &command);
      free(commandLine);

      runStatus = runCommand(command);

      if(!runStatus) 
         printf(ERR_INVALID_COMMAND, command.command);
      else 
         if(runStatus == EXIT_FILE_SYSTEM) break;
   }

   return EXIT_SUCCESS;
} // filesystem()

DiscoVirtual* criaDisco(char unidade, unsigned int tamanho) {
   DiscoVirtual* virtualDisk;
   char validUnity = toupper(unidade);

   // Valida a unidade e o tamanho do disco.
   if(validUnity < CHARACTER_CODE_A || validUnity > CHARACTER_CODE_Z || tamanho < MIN_DISK_SIZE || tamanho > MAX_DISK_SIZE)
      return NULL;

   // Verifica se o disco já existe.
   if(getVirtualDisk(validUnity, MODE_GET_DISK))
      return NULL;

   // Cria o disco.
   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_CREATE_DISK)))
      return NULL;

   // Inicializa os atributos do disco.
   initializeVirtualDisk(virtualDisk, validUnity, tamanho);

   return virtualDisk;
} // criaDisco()


int formatar(char unidade, DiscoVirtual* disco) {
   if(disco->unidade != unidade) return FALSE;

   for(unsigned int index = 0; index < disco->numeroArquivos; index++)
      free(disco->tabelaArquivos[index].conteudo);

   initializeVirtualDisk(disco, disco->unidade, disco->disco.capacidade);
   return TRUE;
}


int criar(char unidade, const char *nomeArquivo) {
   DiscoVirtual* virtualDisk;
   Arquivo file;
   char validUnity = toupper(unidade);

   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_GET_DISK)))
      return ERRO_CRIAR_ARQUIVO;

   if(getFileByName(nomeArquivo, virtualDisk))
      return ERRO_CRIAR_ARQUIVO;

   file = newInitializedFile(nomeArquivo);
   return saveFile(file, virtualDisk);
}


int abrir(char unidade, const char* nomeArquivo) {
   DiscoVirtual* virtualDisk;
   Arquivo* file;
   char validUnity = toupper(unidade);

   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_GET_DISK)))
      return ERRO_ABRIR_ARQUIVO;

   if(!(file = getFileByName(nomeArquivo, virtualDisk)))
      return ERRO_ABRIR_ARQUIVO;

   return (file->aberto = TRUE);
}

int escrever(char unidade, unsigned short fd, const void* buffer, unsigned int tamanho) {
   const unsigned int floatSize = sizeof(float);
   DiscoVirtual* virtualDisk;
   float *auxFloat;
   char *auxChar,
      validUnity = toupper(unidade);
   unsigned int oldSize;
   
   // Obtém o disco.
   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_GET_DISK))) return ERRO_ESCREVER_ARQUIVO;

   // Verifica se o arquivo está na tabela.
   if(fd >= virtualDisk->numeroArquivos) return ERRO_ESCREVER_ARQUIVO;

   // Verifica se o arquivo está aberto.
   if(!virtualDisk->tabelaArquivos[fd].aberto) return ERRO_ESCREVER_ARQUIVO;

   oldSize = virtualDisk->tabelaArquivos[fd].tamanho; // Salvando o tamanho antigo.
   if(virtualDisk->tabelaArquivos[fd].tipo == NUMERICO) {
      // Verifica se há espaço livre.
      if(!checkDiskCapacity(virtualDisk, virtualDisk->tabelaArquivos[fd], tamanho * floatSize)) return NO_ERROR;

      // Verifica se é a primeira vez escrevendo no arquivo.
      if(!virtualDisk->tabelaArquivos[fd].conteudo)
         auxFloat = (float*) calloc(1, floatSize * tamanho);
      else {
         oldSize /= floatSize;
         auxFloat = (float*) realloc(virtualDisk->tabelaArquivos[fd].conteudo, floatSize * (tamanho) + oldSize);
      }

      // Armazena os valores.
      for(unsigned int index = oldSize; index < oldSize + tamanho; index++)
            auxFloat[index] = ((float*)buffer)[index - oldSize];

      virtualDisk->tabelaArquivos[fd].conteudo = (void*) auxFloat;
      virtualDisk->tabelaArquivos[fd].tamanho += floatSize * tamanho;
   } else {
      tamanho = strlen(((char*)buffer));
      if(tamanho <= 0) return ERRO_ESCREVER_ARQUIVO;

      // Verifica se há espaço livre.
      if(!checkDiskCapacity(virtualDisk, virtualDisk->tabelaArquivos[fd], tamanho)) return NO_ERROR;

       // Verifica se é a primeira vez escrevendo no arquivo.
      if(!virtualDisk->tabelaArquivos[fd].conteudo) 
         auxChar = (char*) calloc(1, tamanho + 5);
      else {
         auxChar = (char*) realloc((char*)virtualDisk->tabelaArquivos[fd].conteudo, ++tamanho + strlen((char*)virtualDisk->tabelaArquivos[fd].conteudo));
         strcat(auxChar, COMMAND_DELIMITER);
      }

      strcat(auxChar, buffer);
      strcat(auxChar, STR_CHAR_NULO);

      virtualDisk->tabelaArquivos[fd].conteudo = (void*) auxChar;
      virtualDisk->tabelaArquivos[fd].tamanho += tamanho;
   }

   virtualDisk->tabelaArquivos[fd].blocosLogicos = virtualDisk->tabelaArquivos[fd].tamanho / TAMANHO_BLOCO_LOGICO;
   if(virtualDisk->tabelaArquivos[fd].tamanho % TAMANHO_BLOCO_LOGICO != 0) virtualDisk->tabelaArquivos[fd].blocosLogicos++;

   updateVirtualDisk(virtualDisk);
   return NO_ERROR;
} // escrever()


int ler(char unidade, unsigned short fd, void** buffer, unsigned int tamanho) {
   const unsigned int floatSize = sizeof(float);
   DiscoVirtual* virtualDisk;
   float *auxFloat;
   char *auxChar,
      validUnity = toupper(unidade);
   unsigned int count = 0;
   
   // Obtém o disco.
   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_GET_DISK))) return ERRO_LER_ARQUIVO;

   // Verifica se o arquivo está aberto.
   if(!virtualDisk->tabelaArquivos[fd].aberto) return ERRO_LER_ARQUIVO;

   // Verifica se o arquivo está na tabela.
   if(fd >= virtualDisk->numeroArquivos) return ERRO_LER_ARQUIVO;

   // Obtendo os dados e preenchendo o buffer.
   if(virtualDisk->tabelaArquivos[fd].tipo == NUMERICO) {
      auxFloat = (float*) calloc(1, virtualDisk->tabelaArquivos[fd].tamanho);
      for(count = 0; count < virtualDisk->tabelaArquivos[fd].tamanho / floatSize && (count < tamanho || tamanho == -1); count++) 
         auxFloat[count] = ((float*)virtualDisk->tabelaArquivos[fd].conteudo)[count];
      
      *buffer = (void*)auxFloat;
      return count;
   } else {
      auxChar = (char*) calloc(1, strlen((char*)virtualDisk->tabelaArquivos[fd].conteudo) + 1);

      strcpy(auxChar, (char*)virtualDisk->tabelaArquivos[fd].conteudo);
      strcat(auxChar, STR_CHAR_NULO);

      *buffer = (void*)auxChar;
      return count;
   }

   return ERRO_LER_ARQUIVO;
} // ler()


int fechar(char unidade, unsigned short fd) {
   DiscoVirtual* virtualDisk;
   char validUnity = toupper(unidade);

   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_GET_DISK)))
      return ERRO_ABRIR_ARQUIVO;

   if(fd >= virtualDisk->numeroArquivos)
      return ERRO_ABRIR_ARQUIVO;

   return !(virtualDisk->tabelaArquivos[fd].aberto = FALSE);
}


int excluir(char unidade, const char* nomeArquivo) {
   DiscoVirtual* virtualDisk;
   Arquivo* file;
   char validUnity = toupper(unidade);

   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_GET_DISK))) return ERRO_EXCLUIR_ARQUIVO;
   if(!(file = getFileByName(nomeArquivo, getVirtualDisk(validUnity, MODE_GET_DISK)))) return ERRO_EXCLUIR_ARQUIVO;

   if(!confirmChoice()) return ERRO_EXCLUIR_ARQUIVO;

   unsigned int oldFd = file->fd;
   free(file->conteudo);

   virtualDisk->tabelaArquivos[oldFd] = virtualDisk->tabelaArquivos[--virtualDisk->numeroArquivos];
   virtualDisk->tabelaArquivos[oldFd].fd = oldFd;
   updateVirtualDisk(virtualDisk);

   return TRUE;
} // excluir()


InfoDisco* infoDisco(char unidade) {
   DiscoVirtual* virtualDisk;
   InfoDisco* diskInformation;
   char validUnity = toupper(unidade);

   if(!(virtualDisk = getVirtualDisk(validUnity, MODE_GET_DISK)))
      return NULL;

   diskInformation = (InfoDisco*) calloc(1, sizeof(InfoDisco));
   diskInformation->discoVirtual = *virtualDisk;
   
   return diskInformation;
}


/* Cria ou obtém o endereço da estrutura de armazenamento do disco identificado por drive, que representa sua letra de identificação, por exemplo C, D, E, etc.
   O parâmetro mode informa o modo de acesso ao disco virtual, os valores válidos são: MODE_CREATE_DISK e MODE_GET_DISK.

   Se mode for MODE_CREATE_DISK e a letra de identificação do drive for válida, a função cria o disco e retorna um ponteiro para sua estrutura de armazenamento.
   Se mode for MODE_GET_DISK e a letra de identificação do drive for válida, a função obtém o ponteiro para a estrutura de armazenamento do disco previamente criada.

   Se se ocorrer algum erro na criação ou no acesso ao disco, ou se a identificação do drive for inválida será retornado NULL.
*/
DiscoVirtual* getVirtualDisk(char drive, char mode) {
	static DiscoVirtual *virtualDisk[NUMERO_DE_DISCOS];
   static unsigned char currentNumberOfDisks = 0;
   
   switch (mode) {
   case MODE_CREATE_DISK: 
         if ((drive >= CHARACTER_CODE_A && drive <= CHARACTER_CODE_Z) && currentNumberOfDisks < NUMERO_DE_DISCOS) {
            virtualDisk[currentNumberOfDisks] = (DiscoVirtual *) calloc(1, sizeof(DiscoVirtual));
            return (virtualDisk[currentNumberOfDisks] != NULL) ? virtualDisk[currentNumberOfDisks++] : NULL;
         }

   case MODE_GET_DISK: 
         if (drive >= CHARACTER_CODE_A && drive <= CHARACTER_CODE_Z) {
            for (unsigned int index = 0; index < currentNumberOfDisks; index++) 
               if(virtualDisk[index]->unidade == drive) 
                  return virtualDisk[index];
         }
   }
   return NULL;
} // getVirtualDisk()

void printPrompt(const char *prompt) {
   printf("%s", prompt);
}

char * readCommand() {
    char *command = (char *) calloc(1, COMMAND_LINE_SIZE);
    
    if(command){ // Verifica se foi possível alocar memória.
        fgets(command, COMMAND_LINE_SIZE, stdin);
        removeEnterChar(command);
        if(isEmptyString(command)) command = deallocateString(command);// Libera a memória alocada, caso a string lida for vazia.
    }

    return command;
} // readCommand()

int getCommand(const char* commandLine, Command* command) {
   char commandLineCopy[COMMAND_LINE_SIZE], *token;

   // Obtendo o comando
   strcpy(commandLineCopy, commandLine);
   strncpy(command->command, strtok(commandLineCopy, COMMAND_DELIMITER), STD_COMMAND_SIZE);

   // Obtendo os parametros
   if((token = strtok(NULL, STR_CHAR_NULO))) {
      strcpy(command->parameters, token);
      return TRUE;
   }

   strcpy(command->parameters, STR_CHAR_NULO);
   return FALSE;
} // getCommand()

int runCommand(Command command) {
   unsigned int errorCode = NO_ERROR;

   switch (isValidCommand(command.command)){
      case CD:
         errorCode = fileSystemCreateDisk(command.parameters);
         break;
      case CA:
         errorCode = fileSystemCreateFile(command.parameters);
         break;
      case AA:
         errorCode = fileSystemOpenFile(command.parameters);
         break;
      case EA:
         errorCode = fileSystemWriteFile(command.parameters);
         break;
      case LA:
         errorCode = fileSystemReadFile(command.parameters);
         break;
      case FA:
         errorCode = fileSystemCloseFile(command.parameters);
         break;
      case DA:
         errorCode = fileSystemDeleteFile(command.parameters);
         break;
      case LD:
         errorCode = fileSystemShowDiskInformation(command.parameters);
         break;
      case FD:
         errorCode = fileSystemFormatDisk(command.parameters);
         break;
      case ET:
         errorCode = fileSystemDisplayFileSize(command.parameters);
         break;
      case EF:
         return finalizeFileSystem();
      case IA:
         errorCode = fileSystemImportFile(command.parameters);
         break;
      case HP:
         printf(MSG_HELP);
         break;
      default:
         return FALSE;
   }

   if(errorCode != NO_ERROR)
      printError(errorCode);

   return TRUE;
} // runCommand()

int fileSystemCreateDisk(const char* parameters) {
   char diskUnity, *buffer;
   int diskCapacity, error;

   if((error = validateDiskParameters(parameters, &diskUnity)) != NO_ERROR)
      return error;

   // Lendo a capacidade.
   if(!(buffer = strtok(NULL, COMMAND_DELIMITER)))
      return ERROR_INCORRECT_SYNTAX;

   if(!stringToInteger(buffer, &diskCapacity))
      return ERROR_INCORRECT_SYNTAX;

   if(diskCapacity < MIN_DISK_SIZE || diskCapacity > MAX_DISK_SIZE) return ERROR_DISK_SIZE;

   if(!criaDisco(diskUnity, diskCapacity))
      return ERROR_CREATE_DISK;

   return NO_ERROR;
} // fileSystemCreateDisk()

int fileSystemFormatDisk(const char* parameters) {
   char diskUnity;
   DiscoVirtual* virtualDisk;
   int error;

   if((error = validateDiskParameters(parameters, &diskUnity)) != NO_ERROR)
      return error;
   
   // Checando se há mais parâmetros.
   if(strtok(NULL, COMMAND_DELIMITER))
      return ERROR_INCORRECT_SYNTAX;

   if(!(virtualDisk = getVirtualDisk(diskUnity, MODE_GET_DISK)))
      return ERROR_DISK_NOT_EXIST;

   if(!confirmChoice()) return NO_ERROR;

   return (formatar(virtualDisk->unidade, virtualDisk)) ? NO_ERROR : ERROR_FORMAT_DISK;
} // fileSystemFormatDisk()

int fileSystemCreateFile(const char* parameters) {
   char diskUnity, fileName[TAMANHO_NOME_ARQUIVO];
   int error;

   if((error = validateFileParameters(parameters, &diskUnity, fileName)) != NO_ERROR)
      return error;
   
   // Checando se há mais parâmetros.
   if(strtok(NULL, COMMAND_DELIMITER))
      return ERROR_INCORRECT_SYNTAX;

   if(criar(diskUnity, fileName) == ERRO_CRIAR_ARQUIVO)
      return ERROR_CREATE_FILE;

   return NO_ERROR;
} // fileSystemCreateFile()

int fileSystemOpenFile(const char* parameters) {
   char diskUnity, fileName[TAMANHO_NOME_ARQUIVO];
   int error;

   if((error = validateFileParameters(parameters, &diskUnity, fileName)) != NO_ERROR)
      return error;

   // Checando se há mais parâmetros.
   if(strtok(NULL, COMMAND_DELIMITER))
      return ERROR_INCORRECT_SYNTAX;

   if(abrir(diskUnity, fileName) == ERRO_ABRIR_ARQUIVO)
      return ERROR_OPEN_FILE;

   return NO_ERROR;
} // fileSystemOpenFile()

int fileSystemWriteFile(const char* parameters) {
   char diskUnity, fileName[TAMANHO_NOME_ARQUIVO], *buffer, *argsCopy;
   int error;
   float element;
   Boolean isValid;

   if((error = validateFileParameters(parameters, &diskUnity, fileName)) != NO_ERROR)
      return error;

   if(!(buffer = strtok(NULL, STR_CHAR_NULO)))
      return ERROR_INCORRECT_SYNTAX;

   // Copiando os argumentos, para checar o primeiro elemento.
   argsCopy = (char*) calloc(1, strlen(buffer));
   strcpy(argsCopy, buffer);

   isValid = stringToFloat(argsCopy, &element);
   free(argsCopy);

   return (isValid) ? writeNumberFile(fileName, diskUnity, buffer) : writeStringFile(fileName, diskUnity, buffer);
} // fileSystemWriteFile()

int writeNumberFile(const char* fileName, const char diskUnity, char* parameters) {
   char* buffer;
   float elements[MAX_NUMBER_ELEMENTS];
   unsigned int count = 0;
   Arquivo* file;

   if(!(file = getFileByName(fileName, getVirtualDisk(diskUnity, MODE_GET_DISK))))
      return ERROR_FILE_NOT_EXIST;

   if(file->tipo != NUMERICO && file->tamanho > 0) return ERROR_WRITE_FILE;

   if(!file->aberto) return ERROR_CLOSED_FILE;

   if(!(buffer = strtok(parameters, COMMAND_DELIMITER))) return ERROR_INCORRECT_SYNTAX;
   if(!stringToFloat(buffer, &elements[count++]))
         return ERROR_INCORRECT_SYNTAX;

   while((buffer = strtok(NULL, COMMAND_DELIMITER)) && count < MAX_NUMBER_ELEMENTS)
      if(!stringToFloat(buffer, &elements[count++]))
         return ERROR_INCORRECT_SYNTAX;

   file->tipo = NUMERICO;
   if(escrever(diskUnity, file->fd, (void*)elements, count) == ERRO_ESCREVER_ARQUIVO)
      return ERROR_WRITE_FILE;

   return NO_ERROR;
} // writeNumberFile()

int writeStringFile(const char* fileName, const char diskUnity, const char* parameters) {
   char *elements;
   unsigned int count = 0;
   int error;
   Arquivo* file;

   if(!(file = getFileByName(fileName, getVirtualDisk(diskUnity, MODE_GET_DISK))))
      return ERROR_FILE_NOT_EXIST;

   if(file->tipo != STRING && file->tamanho > 0) return ERROR_WRITE_FILE;

   if(!file->aberto) return ERROR_CLOSED_FILE;

   elements = (char*) calloc(1, strlen(parameters) + 1);
   strcpy(elements, parameters);
   strcat(elements, STR_CHAR_NULO);

   file->tipo = STRING;
   error = escrever(diskUnity, file->fd, (void*)elements, count);
   free(elements);
   
   if(error == ERRO_ESCREVER_ARQUIVO) return ERROR_WRITE_FILE;

   return NO_ERROR;
} // writeStringFile()

int fileSystemReadFile(const char* parameters) {
   const int decimalPlaces = 2;
   char diskUnity, fileName[TAMANHO_NOME_ARQUIVO];
   void **buffer = NULL;
   Arquivo *file;
   int error, count;
   unsigned int index;

   if((error = validateFileParameters(parameters, &diskUnity, fileName)) != NO_ERROR)
      return error;

   if(!(file = getFileByName(fileName, getVirtualDisk(diskUnity, MODE_GET_DISK))))
      return ERROR_FILE_NOT_EXIST;

   // Checando se há mais parâmetros.
   if(strtok(NULL, COMMAND_DELIMITER))
      return ERROR_INCORRECT_SYNTAX;

   if(file->tamanho == 0) {
      printf(STR_ERR_EMPTY_FILE, fileName);
      return NO_ERROR;
   }

   if(!file->aberto) return ERROR_CLOSED_FILE;

   if((count = ler(diskUnity, file->fd, (void*)&buffer, -1)) == ERRO_LER_ARQUIVO) return ERROR_READ_FILE;
   if(!buffer) return ERROR_READ_FILE;

   printf(MSG_FILE_CONTENT, fileName);

   if(file->tipo == NUMERICO){
      for(index = 0; index < count - 1; index++) {
         printFloat(((float*)buffer)[index], decimalPlaces);
         printf(", ");
      }

      printFloat(((float*)buffer)[index], decimalPlaces);
      printf("\n\n");
   } else {
      printf("\"%s\"\n\n", ((char*)buffer));
   }

   free(buffer); 
   return NO_ERROR;
} // fileSystemReadFile()

int fileSystemCloseFile(const char* parameters) {
   char diskUnity, fileName[TAMANHO_NOME_ARQUIVO];
   DiscoVirtual* virtualDisk;
   Arquivo* file;
   int error;

   if((error = validateFileParameters(parameters, &diskUnity, fileName)) != NO_ERROR)
      return error;

   // Checando se há mais parâmetros.
   if(strtok(NULL, COMMAND_DELIMITER))
      return ERROR_INCORRECT_SYNTAX;
   
   if(!(virtualDisk = getVirtualDisk(diskUnity, MODE_GET_DISK)))
      return ERROR_DISK_NOT_EXIST;
   
   if(!(file = getFileByName(fileName, virtualDisk)))
      return ERROR_FILE_NOT_EXIST;

   if(fechar(diskUnity, file->fd) == ERRO_FECHAR_ARQUIVO)
      return ERROR_CLOSE_FILE;

   return NO_ERROR;
} // fileSystemCloseFile()

int fileSystemDeleteFile(const char* parameters) {
   char diskUnity, fileName[TAMANHO_NOME_ARQUIVO];
   DiscoVirtual* virtualDisk;
   int error;

   if((error = validateFileParameters(parameters, &diskUnity, fileName)) != NO_ERROR)
      return error;
   
   // Checando se há mais parâmetros.
   if(strtok(NULL, COMMAND_DELIMITER))
      return ERROR_INCORRECT_SYNTAX;

   if(!(virtualDisk = getVirtualDisk(diskUnity, MODE_GET_DISK)))
      return ERROR_DISK_NOT_EXIST;

   if(excluir(diskUnity, fileName) == ERRO_EXCLUIR_ARQUIVO)
      return ERROR_DELETE_FILE;
   
   return NO_ERROR;
} // fileSystemDeleteFile()

int fileSystemDisplayFileSize(const char* parameters) {
   char diskUnity, fileName[TAMANHO_NOME_ARQUIVO];
   DiscoVirtual* virtualDisk;
   Arquivo* file;
   int error;

   if((error = validateFileParameters(parameters, &diskUnity, fileName)) != NO_ERROR)
      return error;

   // Checando se há mais parâmetros.
   if(strtok(NULL, COMMAND_DELIMITER))
      return ERROR_INCORRECT_SYNTAX;
   
   if(!(virtualDisk = getVirtualDisk(diskUnity, MODE_GET_DISK)))
      return ERROR_DISK_NOT_EXIST;
   
   if(!(file = getFileByName(fileName, virtualDisk)))
      return ERROR_FILE_NOT_EXIST;

   printf("\n\tTamanho = ");
   printInt(file->tamanho, CHAR_THOUSAND_SEPARATOR);
   printf(" bytes\n\tBlocos = ");
   printInt(file->blocosLogicos, CHAR_THOUSAND_SEPARATOR);
   printf("\n\n");

   return NO_ERROR;
} // fileSystemDisplayFileSize()

int fileSystemShowDiskInformation(const char* parameters) {
   char diskUnity, *buffer;
   DiscoVirtual* virtualDisk;
   InfoDisco* diskInformation;
   int error;

   if((error = validateDiskParameters(parameters, &diskUnity)) != NO_ERROR)
      return error;

   if(!(virtualDisk = getVirtualDisk(diskUnity, MODE_GET_DISK)))
      return ERROR_DISK_NOT_EXIST;

   // Checando se há mais parâmetros.
   if((buffer = strtok(NULL, COMMAND_DELIMITER)))
      return ERROR_INCORRECT_SYNTAX;

   if(!(diskInformation = infoDisco(diskUnity)))
      return ERROR_DISK_NOT_EXIST;

   printDiskInformation(*diskInformation);

   free(diskInformation);
   return NO_ERROR;
} // fileSystemShowDiskInformation()

int finalizeFileSystem() {
   DiscoVirtual* virtualDisk;

   for(char drive = CHARACTER_CODE_A; drive <= CHARACTER_CODE_Z; drive++)
      if((virtualDisk = getVirtualDisk(drive, MODE_GET_DISK))) {
         formatar(drive, virtualDisk);
         free(virtualDisk);
      }

   return EXIT_FILE_SYSTEM;
}

int fileSystemImportFile(const char* parameters) {
   char diskUnity, *buffer, *path, *aux, fileName[TAMANHO_NOME_ARQUIVO];
   DiscoVirtual* virtualDisk;
   int error, fd;

   if((error = validateDiskParameters(parameters, &diskUnity)) != NO_ERROR)
      return error;

   // Verifica se o disco existe.
   if(!(virtualDisk = getVirtualDisk(diskUnity, MODE_GET_DISK)))
      return ERROR_DISK_NOT_EXIST;

   // Obtendo o caminho do arquivo.
   if(!(path = strtok(NULL, COMMAND_DELIMITER))) return ERROR_INCORRECT_SYNTAX;

   // Verifica se há mais argumentos.
   if((buffer = strtok(NULL, COMMAND_DELIMITER))) return ERROR_INCORRECT_SYNTAX;

   buffer = readFile(path);
   if(!buffer) return ERROR_FILE_NOT_EXIST;

   // Obtendo o nome do arquivo.
   aux = strrchr(path, CHAR_PATH_SEPARATOR);
   if (aux) strcpy(fileName, aux + 1);

   // Verifica se o nome do arquivo possui mais que TAMANHO_NOME_ARQUIVO caracteres.
   if(strlen(fileName) >= TAMANHO_NOME_ARQUIVO - 1)
      return ERROR_NAME_SIZE;

   if((fd = criar(diskUnity, fileName)) != ERRO_CRIAR_ARQUIVO) {
      abrir(diskUnity, fileName);
      error = (escrever(diskUnity, fd, (void*)buffer, 0) == ERRO_ESCREVER_ARQUIVO) ? ERROR_WRITE_FILE : NO_ERROR;
   } else
      error = ERROR_WRITE_FILE;

   fechar(diskUnity, fd); // Fechando o arquivo.

   deallocateString(buffer);
   return error;
}

int validateDiskParameters(const char* parameters, char* diskUnity) {
	char copyParameters[COMMAND_LINE_SIZE], *buffer;

   strcpy(copyParameters, parameters);

   // Lendo o identificador.
   if(!(buffer = strtok(copyParameters, COMMAND_DELIMITER)))
      return ERROR_INCORRECT_SYNTAX;

   if(strlen(buffer) > 1)
      return ERROR_INCORRECT_SYNTAX;

   *diskUnity = toupper(buffer[0]);

   return NO_ERROR;
}

int validateFileParameters(const char* parameters, char* diskUnity, char* fileName) {
   char copyParameters[COMMAND_LINE_SIZE], *buffer;
   DiscoVirtual* virtualDisk;

   strcpy(copyParameters, parameters);

   // Lendo o identificador.
   if(!(buffer = strtok(copyParameters, COMMAND_DELIMITER)))
      return ERROR_INCORRECT_SYNTAX;

   // Verifica se o identificador possui um unico caractere.
   if(strlen(buffer) > 1)
      return ERROR_INCORRECT_SYNTAX;

   *diskUnity = toupper(buffer[0]);

   // Verifica se o disco existe.
   if(!(virtualDisk = getVirtualDisk(*diskUnity, MODE_GET_DISK)))
      return ERROR_DISK_NOT_EXIST;

   // Lendo o nome do arquivo.
   if(!(buffer = strtok(NULL, COMMAND_DELIMITER)))
      return ERROR_INCORRECT_SYNTAX;

   // Verifica se o nome do arquivo possui mais que TAMANHO_NOME_ARQUIVO caracteres.
   if(strlen(buffer) >= TAMANHO_NOME_ARQUIVO - 1)
      return ERROR_NAME_SIZE;

   strcpy(fileName, buffer);
   return NO_ERROR;
} // validateFileParameters()

void initializeVirtualDisk(DiscoVirtual* virtualDisk, const char validUnity, unsigned int tamanho) {
   virtualDisk->numeroArquivos = 0;
   virtualDisk->unidade = validUnity;
   virtualDisk->disco.espacoOcupado = 0;
   virtualDisk->disco.blocosLogicosOcupados = 0;

   if(tamanho % TAMANHO_BLOCO_LOGICO)
      tamanho += TAMANHO_BLOCO_LOGICO - (tamanho % TAMANHO_BLOCO_LOGICO);

   virtualDisk->disco.blocosLogicosTotais = tamanho / TAMANHO_BLOCO_LOGICO;
   virtualDisk->disco.blocosLogicosLivres = tamanho / TAMANHO_BLOCO_LOGICO;
   virtualDisk->disco.capacidade = tamanho - (tamanho % TAMANHO_BLOCO_LOGICO);
   virtualDisk->disco.espacoLivre = virtualDisk->disco.capacidade;
}

void updateVirtualDisk(DiscoVirtual* virtualDisk) {
   unsigned int totalSize = 0, totalBlocks = 0;

   for(unsigned int index = 0; index < virtualDisk->numeroArquivos; index++) {
      totalSize += virtualDisk->tabelaArquivos[index].blocosLogicos * TAMANHO_BLOCO_LOGICO;
      totalBlocks += virtualDisk->tabelaArquivos[index].blocosLogicos;
   }

   virtualDisk->disco.blocosLogicosOcupados = totalBlocks;
   virtualDisk->disco.blocosLogicosLivres = virtualDisk->disco.blocosLogicosTotais - totalBlocks;
   virtualDisk->disco.espacoOcupado = totalSize;
   virtualDisk->disco.espacoLivre = virtualDisk->disco.capacidade - totalSize;
}

Arquivo newInitializedFile(const char* fileName) {
   Arquivo file;

   getCurrentTime(file.horaCriacao);
   file.blocosLogicos = 0;
   file.tamanho = 0;
   file.conteudo = NULL;
   file.aberto = FALSE;
   strcpy(file.nome, fileName);

   return file;
}

int saveFile(const Arquivo file, DiscoVirtual* virtualDisk) {
   unsigned int index;
   
   for(index = 0; index < virtualDisk->numeroArquivos; index++)
      if(!strcmp(virtualDisk->tabelaArquivos[index].nome, file.nome))
         return ERRO_CRIAR_ARQUIVO;

   if(index >= NUMERO_DE_ARQUIVOS_DO_DISCO)
      return ERRO_CRIAR_ARQUIVO;

   virtualDisk->tabelaArquivos[index] = file;
   virtualDisk->tabelaArquivos[index].fd = index;
   virtualDisk->numeroArquivos++;
   
   return index;
}

Arquivo* getFileByName(const char* fileName, DiscoVirtual* virtualDisk) {
   for(unsigned int index = 0; index < virtualDisk->numeroArquivos; index++)
      if(!strcmp(virtualDisk->tabelaArquivos[index].nome, fileName))
         return &virtualDisk->tabelaArquivos[index];
   return NULL;
}

int stringToInteger(const char* str, int* number) {
   char *endptr;
   *number = (int) strtol(str, &endptr, 10);
   return (*endptr == CHAR_NULO) ? TRUE : FALSE;    
}

void printError(const unsigned int errorCode) {
   switch (errorCode) {
   case ERROR_INCORRECT_SYNTAX:  printf(STR_ERR_INCORRECT_SYNTAX);   break;
   case ERROR_CREATE_DISK:       printf(STR_ERR_CREATE_DISK);        break;
   case ERROR_FORMAT_DISK:       printf(STR_ERR_FORMAT_DISK);        break;
   case ERROR_DISK_NOT_EXIST:    printf(STR_ERR_DISK_NOT_EXIST);     break;
   case ERROR_CREATE_FILE:       printf(STR_ERR_CREATE_FILE);        break;
   case ERROR_OPEN_FILE:         printf(STR_ERR_OPEN_FILE);          break;
   case ERROR_CLOSE_FILE:        printf(STR_ERR_CLOSE_FILE);         break;
   case ERROR_FILE_NOT_EXIST:    printf(STR_ERR_FILE_NOT_EXIST);     break;
   case ERROR_WRITE_FILE:        printf(STR_ERR_WRITE_FILE);         break;
   case ERROR_READ_FILE:         printf(STR_ERR_READ_FILE);          break;
   case ERROR_DELETE_FILE:       printf(STR_ERR_DELETE_FILE);        break;
   case ERROR_NAME_SIZE:         printf(STR_ERR_NAME_SIZE);          break;
   case ERROR_DISK_SIZE:         printf(STR_ERR_DISK_SIZE, MIN_DISK_SIZE, MAX_DISK_SIZE);          break;
   case ERROR_CLOSED_FILE:       printf(STR_ERR_CLOSED_FILE);        break;
   default:
      break;
   }
}

void printDiskInformation(InfoDisco diskInformation) {
   DiscoVirtual disk = diskInformation.discoVirtual;
   printf("\n\tDisco %c", disk.unidade);

   // Arquivos
   if(disk.numeroArquivos > 0) {
      printf("\n\n\t\t%-15s %-12s %-13s %-15s %-10s", "Nome", "Tipo", "Hora", "Tamanho", "Blocos");
      for(unsigned int index = 0; index < disk.numeroArquivos; index++) {
         printf("\n\t\t%-15s %-*s %-13s ", disk.tabelaArquivos[index].nome, ((disk.tabelaArquivos[index].tipo == NUMERICO) ? 13 : 12), ((disk.tabelaArquivos[index].tipo == NUMERICO) ? TIPO_NUMERICO : TIPO_TEXTO), disk.tabelaArquivos[index].horaCriacao);
         printInt(disk.tabelaArquivos[index].blocosLogicos * TAMANHO_BLOCO_LOGICO, CHAR_THOUSAND_SEPARATOR);
         printf(" %-*s", 15 - countDigits(disk.tabelaArquivos[index].blocosLogicos * TAMANHO_BLOCO_LOGICO), "bytes");
         printInt(disk.tabelaArquivos[index].blocosLogicos, CHAR_THOUSAND_SEPARATOR);
      }
   }
   
   printf("\n\n\tNúmero de arquivos: ");
   printInt(disk.numeroArquivos, CHAR_THOUSAND_SEPARATOR);

   printf("\n\n\tCapacidade do disco: ");
   printInt(disk.disco.capacidade, CHAR_THOUSAND_SEPARATOR);

   printf(" bytes\n\tEspaço ocupado: ");
   printInt(disk.disco.espacoOcupado, CHAR_THOUSAND_SEPARATOR);

   printf(" bytes\n\tEspaço livre: ");
   printInt(disk.disco.espacoLivre, CHAR_THOUSAND_SEPARATOR);

   printf(" bytes\n\n\tTotal de blocos lógicos: ");
   printInt(disk.disco.blocosLogicosTotais, CHAR_THOUSAND_SEPARATOR);

   printf("\n\tBlocos lógicos ocupados: ");
   printInt(disk.disco.blocosLogicosOcupados, CHAR_THOUSAND_SEPARATOR);

   printf("\n\tBlocos lógicos livres: ");
   printInt(disk.disco.blocosLogicosLivres, CHAR_THOUSAND_SEPARATOR);
   printf("\n\n");
} // printDiskInformation()

Boolean confirmChoice() {
   char option[2];

   do {
      printf("\nEssa operação não pode ser desfeita! Continuar (S/N)? ");
      fgets(option, 2, stdin);
      setbuf(stdin, NULL);
   } while(option[0] != 's' && option[0] != 'S' && option[0] != 'n' && option[0] != 'N' );

   if(option[0] == 's' || option[0] == 'S') return TRUE;
   return FALSE;
}

Boolean checkDiskCapacity(DiscoVirtual* virtualDisk, const Arquivo file, const unsigned int size) {
   if(!virtualDisk) return FALSE;

   int fileFreeMemory = file.blocosLogicos * TAMANHO_BLOCO_LOGICO - file.tamanho;

   if(virtualDisk->disco.espacoLivre <= 0 && fileFreeMemory <= 0) {
      printf(MSG_DISK_IS_FULL, virtualDisk->unidade);
      printInt(virtualDisk->disco.capacidade, CHAR_THOUSAND_SEPARATOR);
      printf(" bytes.\n\n");
      return FALSE;
   }

   if(size > (fileFreeMemory + virtualDisk->disco.espacoLivre)) {
      printf(MSG_INSUFFICIENT_MEMORY);
      printInt(fileFreeMemory + virtualDisk->disco.espacoLivre, CHAR_THOUSAND_SEPARATOR);
      printf(" bytes)\n\n");
      return FALSE; 
   }

   return TRUE;
}

Boolean stringToFloat(const char* buffer, float* number) {
   char *endptr;
   float f;

   if(!buffer) return FALSE; // Verifica se há valores no buffer.
   f = (float) strtod(buffer, &endptr); // Tenta converter.
      
   if (endptr == buffer) return FALSE; // Verifica se a conversão ocorreu.
   if(number) *number = f; // Verifica se number é nulo e, caso não seja, armazena o valor nele.

   return TRUE;
} // stringToFloat()

int isValidCommand(const char* command) {
   const char* validCommands[] = {FS_COMMAND_CD, FS_COMMAND_CA, FS_COMMAND_AA, FS_COMMAND_EA, FS_COMMAND_LA, FS_COMMAND_FA, 
      FS_COMMAND_DA, FS_COMMAND_LD, FS_COMMAND_FD, FS_COMMAND_ET, FS_COMMAND_EF, FS_COMMAND_IA, FS_COMMAND_HP};
   
   for(int index = 0; index < AMOUNT_FS_COMMANDS; index++) 
      if(!strcmp(command, validCommands[index])) return index;

   return NOT_FOUND;
}

void getCurrentTime(char* buffer) {
   time_t currentTime = time(NULL);
   strftime(buffer, TAMANHO_HORA, "%H:%M:%S", localtime(&currentTime));
}

int removeEnterChar(char *str) {
   int size = strlen(str);

   if(str[size - 1] == '\n'){
      str[size - 1] = '\0';
      return 1;
   }

   return 0;
}

int isEmptyString(const char *str) {
   return (str[0] == '\n' || str[0] == '\0') ? TRUE : FALSE;
}

int isExitFileSystem(const char* command) {
   return (!strncmp(command, FS_COMMAND_EF, STD_COMMAND_SIZE)) ? TRUE : FALSE;
}

void * deallocateString(char *str) {
   free(str);
   return NULL;
}

/*
    Utilizada pela função printInt(), pois a mesma utiliza recursão para formatar os valores.
    Função interna, deve ser utilizada com cautela, caso precise formatar um valor inteiro utilize a função printInt().
*/
void printIntSeparador(unsigned long long numero, const char separador) {
    if (numero < 1000) {
        printf ("%llu", numero);
        return;
    }
    printIntSeparador (numero/1000, separador);
    printf ("%c%03llu", separador, numero%1000);
}

void printInt(unsigned long long numero, const char separador) {
    if (numero < 0) {
        printf ("-");
        numero = -numero;
    }
    printIntSeparador(numero, separador);
}

void printFloat(float number, const int decimalPlaces) {
   if(number - (int)number > 0)
      printf("%.*f", decimalPlaces, number);
   else if(decimalPlaces >= 0)
      printf("%.0f", number);
}

int countDigits(const int number) {
   int count = 0, control = 0, value = number;
   if(value == 0) return 1;

   do { 
      count++;
      control++;
      if(control % 3 == 0) count++;
   } while((value/=10) != 0);

   return count;
}

char* readFile(const char* filePath) {
   // Abre o arquivo
   FILE* file = fopen(filePath, "r");
   if (file == NULL) return NULL;

   // Determina o tamanho do arquivo.
   fseek(file, 0, SEEK_END);
   unsigned long fileSize = ftell(file);
   rewind(file);

   // Aloca memória pro conteúdo.
   char* buffer = (char*) calloc(1, fileSize + 1);
   if (buffer == NULL) {
      fclose(file);
      return NULL;
   }

   // Lê o arquivo.
   unsigned long bytes_read = fread(buffer, sizeof(char), fileSize, file);
   if (bytes_read != fileSize) {
      free(buffer);
      fclose(file);
      return NULL;
   }

   buffer[fileSize] = '\0';

   // Fecha o arquivo.
   fclose(file);
   return buffer;
}