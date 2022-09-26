#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 50
#define SN 10

char **search (FILE *f, char *artistNameToSearch, int *songNum);
void ricercaArtista(char *fileName);
int addArtist(char *nominativo, char gruppo, int eta, char *genere,char *fileNameMetaDati);
void inserisciArtista(char *fileNameMetaDati);
int addSong(char *artistName, char *songName,char *filename);
void inserisciCanzone(char *filename,char *fileNameMetaDati);
int editArtist(char *oldAlrtist, char *newArtist,char *fileName,char *fileNameMetaDati);
void modificaArtista(char *fileName,char *fileNameMetaDati);
int deleteArtist(char *alrtist,char *fileName,char *fileNameMetaDati);
void eliminaArtista(char *fileName,char *fileNameMetaDati);
void freeDooblePointer(void **p, int num);
char **query(void);
char *toUpperString(char *S);
