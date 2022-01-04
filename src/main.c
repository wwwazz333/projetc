#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MODE S_IRWXU | S_IROTH | S_IXOTH | S_IRGRP | S_IXGRP

void createProject(const char* name);
void createFile(const char* dossier, const char* name);

int file_exist(const char* file_path);

int main(int argc, char** argv)
{
    char dossier[255] = ".";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("-h\t--help\t:\tmontre se message d'aide\n");
            printf("-p\t--projet\t:\tcrée un projet avec le nom donné\n");
            printf("-f\t--fichier\t:\tcrée un fichier dans le dossier indiquer (projet) sinon dans le projet courrant\n");

            return EXIT_SUCCESS;
        }
    }
    if (argc % 2 == 0) {
        for (int i = 1; i < argc; i += 2) {
            if (argv[i][0] != '-') { // création Projet
                if (!file_exist(argv[i])) {
                    createProject(argv[i]);
                }
                strcpy(dossier, argv[i]);
                break;
            }
        }
    } else {
        for (int i = 1; i < argc; i += 2) {
            if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--projet") == 0) { // création Projet
                if (!file_exist(argv[i + 1])) {
                    createProject(argv[i + 1]);
                }
                strcpy(dossier, argv[i + 1]);
            }
        }
    }
    for (int i = 1; i < argc; i++) { // doit être éxécuter en après -p
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fichier") == 0) {
            createFile(dossier, argv[i + 1]);
        }
    }
    return EXIT_SUCCESS;
}

int file_exist(const char* file_path)
{
    if (file_path == NULL) {
        return -1;
    }
    if (access(file_path, F_OK) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void createProject(const char* name)
{
    char path[255];
    FILE* fp;

    printf("Création du projet : %s\n", name);
    sprintf(path, "%s", name);
    mkdir(path, MODE);

    sprintf(path, "%s/Makefile", name);
    fp = fopen(path, "w");
    if (!fp) {
        fprintf(stderr, "écriture dans %s\n", path);
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "CC = gcc\nEXEC = main\nSRCDIR = src\nSRC = $(wildcard src/*.c)\nOBJDIR = obj\nOBJ = $(SRC:$(SRCDIR)/%s=$(OBJDIR)/%s)\nFLAGS = -Iinclude -g3 -Wall -Wextra\nCFLAGS = \nWINDOWS = no\nall : $(EXEC)\n$(OBJDIR)/%s : $(SRCDIR)/%s\n	$(CC) $(FLAGS) -o $@ -c $<\n$(EXEC) : $(OBJ)\n	$(CC) $(CFLAGS) -o $@ $^ \nifeq ($(WINDOWS), yes)\nclean :\n	del .\\$(OBJDIR)\\*.o\n	del $(EXEC).exe	\nelse\nclean : \n	rm -rf $(OBJDIR)/*.o \n	rm $(EXEC)\nendif\n", "%.c", "%.o", "%.o", "%.c");
    fclose(fp);

    sprintf(path, "%s/obj", name);
    mkdir(path, MODE);

    sprintf(path, "%s/include", name);
    mkdir(path, MODE);

    sprintf(path, "%s/src", name);
    mkdir(path, MODE);

    sprintf(path, "%s/src/main.c", name);

    fp = fopen(path, "w");
    if (!fp) {
        fprintf(stderr, "écriture dans %s\n", path);
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "#include <stdio.h>\n\nint main(int argc, char** argv)\n{\n\tprintf(\"hello world!\\n\");\n\treturn 0;\n}\n");

    fclose(fp);
}

void createFile(const char* dossier, const char* name)
{
    char path[255];
    FILE* fp;

    printf("Création fichier : %s\n", name);

    sprintf(path, "%s/src/%s.c", dossier, name);
    if (file_exist(path)) {
        fprintf(stderr, "fichier %s : déjà existant\n", name);
        exit(EXIT_FAILURE);
    }
    sprintf(path, "%s/include/%s.h", dossier, name);
    if (file_exist(path)) {
        fprintf(stderr, "fichier %s : déjà existant\n", name);
        exit(EXIT_FAILURE);
    }

    fp = fopen(path, "w");
    if (!fp) {
        fprintf(stderr, "écriture dans %s\n", path);
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "#ifndef _%s_H_\n#define _%s_H_\n\n\n\n#endif\n", name, name);
    fclose(fp);

    sprintf(path, "%s/src/%s.c", dossier, name);
    fp = fopen(path, "w");
    if (!fp) {
        fprintf(stderr, "écriture dans %s\n", path);
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "#include \"%s.h\"\n\n\n", name);
    fclose(fp);
}