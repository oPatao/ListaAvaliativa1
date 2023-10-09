#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_GENRE_LENGTH 50
#define MAX_LOCATION_LENGTH 100
#define MAX_ALBUMS 25

typedef struct Artist {
    char name[MAX_NAME_LENGTH];
    char genre[MAX_GENRE_LENGTH];
    char location[MAX_LOCATION_LENGTH];
    char albums[MAX_ALBUMS][MAX_NAME_LENGTH];
    int albumCount;
} Artist;

void printArtist(Artist* artist) {
    printf("Nome: %s\n", artist->name);
    printf("Gênero: %s\n", artist->genre);
    printf("Local: %s\n", artist->location);
    printf("Álbuns:\n");
    for (int i = 0; i < artist->albumCount; i++) {
        printf("- %s\n", artist->albums[i]);
    }
    printf("===========\n");
}

void insertArtist(Artist** artistList, int* artistCount, Artist newArtist) {

    Artist* newList = (Artist*)malloc((*artistCount + 1) * sizeof(Artist));

    int i = 0;
    while (i < *artistCount && strcmp(newArtist.name, (*artistList)[i].name) > 0) {
        newList[i] = (*artistList)[i];
        i++;
    }

    newList[i] = newArtist;

    for (; i < *artistCount; i++) {
        newList[i + 1] = (*artistList)[i];
    }

    free(*artistList);
    *artistList = newList;
    (*artistCount)++;
}

void loadArtistsFromFile(const char* filename, Artist** artistList, int* artistCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    Artist artist;
    *artistCount = 0;
    *artistList = NULL;

    while (1) {
        fgets(artist.name, MAX_NAME_LENGTH, file);
        fgets(artist.genre, MAX_GENRE_LENGTH, file);
        fgets(artist.location, MAX_LOCATION_LENGTH, file);

        if (strcmp(artist.name, "===========\n") == 0) {
            break;
        }

        artist.name[strlen(artist.name) - 1] = '\0';
        artist.genre[strlen(artist.genre) - 1] = '\0';
        artist.location[strlen(artist.location) - 1] = '\0';

        artist.albumCount = 0;
        while (1) {
            fgets(artist.albums[artist.albumCount], MAX_NAME_LENGTH, file);

            if (strcmp(artist.albums[artist.albumCount], "===========\n") == 0) {
                break;
            }

            artist.albums[artist.albumCount][strlen(artist.albums[artist.albumCount]) - 1] = '\0';
            artist.albumCount++;
        }


        insertArtist(artistList, artistCount, artist);
    }

    fclose(file);
}

void saveArtistsToFile(const char* filename, Artist* artistList, int artistCount) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    for (int i = 0; i < artistCount; i++) {
        fprintf(file, "%s\n", artistList[i].name);
        fprintf(file, "%s\n", artistList[i].genre);
        fprintf(file, "%s\n", artistList[i].location);
        for (int j = 0; j < artistList[i].albumCount; j++) {
            fprintf(file, "%s\n", artistList[i].albums[j]);
        }
        fprintf(file, "===========\n");
    }

    fclose(file);
}

int findArtistByName(Artist* artistList, int artistCount, const char* name) {
    for (int i = 0; i < artistCount; i++) {
        if (strcmp(artistList[i].name, name) == 0) {
            return i; 
        }
    }
    return -1; 
}

int main() {
    Artist* artistList = NULL;
    int artistCount = 0;

    loadArtistsFromFile("artistas.txt", &artistList, &artistCount);

    int choice;
    char searchName[MAX_NAME_LENGTH];

    while (1) {
        printf("\nMenu:\n");
        printf("1. Inserção ordenada de um novo artista\n");
        printf("2. Remoção de um artista\n");
        printf("3. Edição de um artista\n");
        printf("4. Busca binária por um artista\n");
        printf("5. Busca sequencial por um álbum\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                {
                    Artist newArtist;
                    printf("\nDigite o nome do novo artista: ");
                    scanf("%s", newArtist.name);
                    printf("Digite o gênero musical: ");
                    scanf("%s", newArtist.genre);
                    printf("Digite o local de criação/nascimento: ");
                    scanf("%s", newArtist.location);
                    printf("Digite os álbuns (Digite 'Fim' para encerrar):\n");
                    newArtist.albumCount = 0;
                    while (1) {
                        scanf("%s", newArtist.albums[newArtist.albumCount]);
                        if (strcmp(newArtist.albums[newArtist.albumCount], "Fim") == 0) {
                            break;
                        }
                        newArtist.albumCount++;
                    }

                    insertArtist(&artistList, &artistCount, newArtist);
                    saveArtistsToFile("artistas.txt", artistList, artistCount);
                }
                break;

            case 2: 
                {
                    printf("\nDigite o nome do artista a ser removido: ");
                    scanf("%s", searchName);
                    int index = findArtistByName(artistList, artistCount, searchName);
                    if (index != -1) {

                        for (int i = index; i < artistCount - 1; i++) {
                            artistList[i] = artistList[i + 1];
                        }
                        artistCount--;

                        saveArtistsToFile("artistas.txt", artistList, artistCount);

                        printf("Artista removido com sucesso.\n");
                    } else {
                        printf("Artista não encontrado.\n");
                    }
                }
                break;

            case 3: 
                {
                    printf("\nDigite o nome do artista a ser editado: ");
                    scanf("%s", searchName);
                    int index = findArtistByName(artistList, artistCount, searchName);
                    if (index != -1) {
                        printf("Digite o novo nome do artista: ");
                        scanf("%s", artistList[index].name);
                        printf("Digite o novo gênero musical: ");
                        scanf("%s", artistList[index].genre);
                        printf("Digite o novo local de criação/nascimento: ");
                        scanf("%s", artistList[index].location);
                        printf("Digite os novos álbuns (Digite 'Fim' para encerrar):\n");
                        artistList[index].albumCount = 0;
                        while (1) {
                            scanf("%s", artistList[index].albums[artistList[index].albumCount]);
                            if (strcmp(artistList[index].albums[artistList[index].albumCount], "Fim") == 0) {
                                break;
                            }
                            artistList[index].albumCount++;
                        }

                        saveArtistsToFile("artistas.txt", artistList, artistCount);

                        printf("Artista editado com sucesso.\n");
                    } else {
                        printf("Artista não encontrado.\n");
                    }
                }
                break;

            case 4: 
                {
                    printf("\nDigite o nome do artista a ser buscado: ");
                    scanf("%s", searchName);

                    int left = 0, right = artistCount - 1;
                    while (left <= right) {
                        int mid = left + (right - left) / 2;
                        int cmp = strcmp(artistList[mid].name, searchName);
                        if (cmp == 0) {

                            printArtist(&artistList[mid]);
                            break;
                        } else if (cmp < 0) {
                            left = mid + 1;
                        } else {
                            right = mid - 1;
                        }
                    }
                    if (left > right) {
                        printf("Artista não encontrado.\n");
                    }
                }
                break;

            case 5: 
                {
                    printf("\nDigite o nome do álbum a ser buscado: ");
                    scanf("%s", searchName);

                    int found = 0;
                    for (int i = 0; i < artistCount; i++) {
                        for (int j = 0; j < artistList[i].albumCount; j++) {
                            if (strcmp(artistList[i].albums[j], searchName) == 0) {

                                printf("Álbum encontrado no artista:\n");
                                printArtist(&artistList[i]);
                                found = 1;
                                break; 
                            }
                        }
                    }

                    if (!found) {
                        printf("Álbum não encontrado.\n");
                    }
                }
                break;

            case 6: 

                free(artistList);
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
