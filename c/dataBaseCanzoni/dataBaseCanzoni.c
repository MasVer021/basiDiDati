#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 50
#define SN 10

int addSong(FILE *f,char *artistName,char *songName);
char **searchSong(FILE *f,char *artistName,int *songNum);
int editArtist(FILE *f,char *filename,char *oldAlrtist, char *newArtist);
int deleteArtist(FILE *f,char *filename,char *alrtist);
void ricerca(FILE *f);
void modifica(FILE *f,char *fileName);
void inserimento(FILE *f);
void cancellazione (FILE *f,char *fileName);
void freeDooblePointer(void **p, int num);




int main(void)
{
  char fileName[LEN];
  char artist [LEN];
  FILE *fp;
  char ** songs;
  int menuID,submenuID,statusID;

  printf("Inserisci il nome del file contentente il database di canzoni:");
  scanf("%s",fileName);
  printf("File %s loaded\n",fileName);

  do{
    printf("1-Ricerca canzoni in base all'autore\n2-Inserici una nuova canzone\n3-Modifica nome autore\n4-Elimina artista e sue canzoni\n5-Cambia file contenente il database\n0-Esci\n");
    scanf("%d",&menuID);
    switch(menuID)
    {
      case 1:
        do{
          fp=fopen(fileName,"r");
          if(!fp)
          {
            fprintf(stderr, "Errore nell'apertura del file");
          }
          else
          {
            ricerca(fp);
            fclose(fp);
          }
          printf("1-Ricerca altro autore\n0-Menu principale\n");
          scanf("%d",&submenuID);
        }while(submenuID!=0);
        break;

      case 2:
        do{
          fp=fopen(fileName,"a+");
          if(!fp)
          {
            fprintf(stderr, "Errore nell'apertura del file");
          }
          else
          {
            inserimento(fp);
            fclose(fp);
          }
          printf("1-Inserisci una nuova canzone\n0-Menu principale\n");
          scanf("%d",&submenuID);
        }while(submenuID!=0);
        break;

      case 3:
          do{
            fp=fopen(fileName,"r");
            if(!fp)
            {
              fprintf(stderr, "Errore nell'apertura del file");
            }
            else
            {
              modifica(fp,fileName);
              fclose(fp);
            }
            printf("1-Modifica nuovo artista\n0-Menu principale\n");
            scanf("%d",&submenuID);
          }while(submenuID!=0);
          break;

      case 4:
          do{
            fp=fopen(fileName,"r");
            if(!fp)
            {
              fprintf(stderr, "Errore nell'apertura del file");
            }
            else
            {
              cancellazione(fp,fileName);
              fclose(fp);
            }
            printf("1-Elimina altro artista\n0-Menu principale\n");
            scanf("%d",&submenuID);
          }while(submenuID!=0);
          break;
      case 5:
          printf("Inserisci il nome del file contentente il database di canzoni:");
          scanf("%s",fileName);
          printf("File %s loaded\n",fileName);
          break;

    }
  }while(menuID!=0);

  return 0;
}

void ricerca(FILE *f)
{
  char ** songs;
  char artist [LEN];
  int numSong;

  printf("Inserisci l'artista da cercare :");
  scanf("%s",artist );
  songs = searchSong(f,artist,&numSong);
  if(numSong==0)
  {
    printf("Artista non trovato\n");
    return;
  }
  printf("Canzoni dell'artista %s trovate\n",artist);

  for(int i=0;i<numSong;i++)
  {
    printf("%d# %s\n",i+1,songs[i]);
  }
  freeDooblePointer(songs,numSong);
}

void inserimento(FILE *f)
{

  char songName [LEN];
  char artistName[LEN];
  int statusID;

  printf("Inserisci il nome della canzone da inserire:");
  scanf("%s",songName);

  printf("Inserisci l'artista da associare alla canzone:");
  scanf("%s",artistName);

  statusID=addSong(f,artistName,songName);
  if(statusID == 1)
  {
    printf("Canzone già presente nel database\n");
  }
  else if(statusID == -1)
  {
    printf("Errore inserimento canzone\n");
  }

}

void modifica(FILE *f,char *fileName)
{
  char oldAlrtist[LEN];
  char newArtist[LEN];
  int statusID,numSong;
  char ** songs;

  printf("Inserisci il nome dell'artista da modificare:");
  scanf("%s",oldAlrtist);
  songs =searchSong(f,oldAlrtist,&numSong);
  freeDooblePointer(songs,numSong);
  if(numSong==0)
  {
    printf("Artista non trovato\n");
    return;
  }
  rewind(f);
  printf("Inserisci il NUovo node dell'artista:");
  scanf("%s",newArtist);
  statusID=editArtist(f,fileName,oldAlrtist,newArtist);

  if(statusID == -1)
  {
    printf("Errore modifica artista\n");
  }
}

void cancellazione (FILE *f,char *fileName)
{
  char artistName[LEN];
  int statusID,numSong;
  char ** songs;

  printf("Inserisci il nome dell'artista da cancellare:");
  scanf("%s",artistName);
  songs=searchSong(f,artistName,&numSong);
  freeDooblePointer(songs,numSong);
  if(numSong==0)
  {
    printf("Artista non trovato\n");
    return;
  }
  rewind(f);
  statusID=deleteArtist(f,fileName,artistName);

  if(statusID == -1)
  {
    printf("Errore cancellazione artista\n");
  }
}

char **searchSong(FILE *f,char *artistNameToSearch,int* songNum)
{

  char songName [LEN];
  char artistName[LEN];
  int num =0;

  char ** songs = malloc(SN*sizeof(char *));

  if(!songs) // controllo allocazione
  {
    return NULL;
  }

  while(!feof(f) && fscanf(f,"%s %s",songName,artistName)==2)
  {
    if(strcmp(artistName,artistNameToSearch)==0)
    {
      songs[num] = malloc(strlen(artistName)*sizeof(char)+1);

      if(!songs[num]) //controllo allocazione
      {
        return NULL;
      }

      strcpy(songs[num],songName);
      num++;
      if(num== SN) // controllo se si è arrivati alla fine del vettore di canzoni
      {
        songs = realloc(songs,(num+SN)*sizeof(char *));
      }
    }
  }

  songs = realloc(songs,num*sizeof(char *));
  *songNum=num;
  return songs;
}


int addSong(FILE *f,char *artistName,char *songName)
{
  char ** songs;
  int  numS,i;

  songs = searchSong(f,artistName,&numS);
  for(i=0;i<numS;i++)
  {
    if(strcmp(songs[i],songName)==0)
    {
      freeDooblePointer(songs,numS);
      return 1;
    }
  }
  freeDooblePointer(songs,numS);
  if(fprintf(f,"%s %s\n",songName,artistName)>0)
  {
    return 0;
  }
  return -1;
}


int editArtist(FILE *f,char * filename,char * oldAlrtist, char * newArtist)
{
  char songName [LEN];
  char artistName[LEN];

  FILE *ft ;
  ft = fopen("temp.txt","w");
  if(!ft)
  {
    return -1;
  }

  while(!feof(f) && fscanf(f,"%s %s",songName,artistName)==2)
  {
    if(strcmp(artistName,oldAlrtist)==0)
    {
      strcpy(artistName,newArtist);
    }
    if(fprintf(ft,"%s %s\n",songName,artistName)<0)
    {
      fclose(ft);
      remove("temp.txt");
      return -1;
    }
  }
  fclose(ft);
  remove(filename);
  rename("temp.txt",filename);
  return 0;
}

int deleteArtist(FILE *f,char *filename,char *alrtist)
{
  char songName [LEN];
  char artistName[LEN];

  FILE *ft ;
  ft = fopen("temp.txt","w");
  if(!ft)
  {
    return -1;
  }
  while(!feof(f) && fscanf(f,"%s %s",songName,artistName)==2)
  {
    if(strcmp(artistName,alrtist)!=0)
    {
      if(fprintf(ft,"%s %s\n",songName,artistName)<0)
      {
        fclose(ft);
        remove("temp.txt");
        return -1;
      }
    }
  }
  fclose(ft);
  remove(filename);
  rename("temp.txt",filename);
  return 0;
}

void freeDooblePointer(void **p, int num)
{
  for(int i=0;i<num;i++)
  {
    free(p[i]);
  }
  free(p);
}
