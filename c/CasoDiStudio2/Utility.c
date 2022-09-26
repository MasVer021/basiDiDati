#include "Utility.h"
#define NAMEFILETEMP "temp.txt"


char **search (FILE *f, char *artistNameToSearch, int *songNum)
{
  char songName[LEN];
  char artistName[LEN];
  int num = 0;
  char **songs = malloc(SN * sizeof(char *));

  if (!songs) // controllo allocazione
  {
    return NULL;
  }

  while (!feof(f) && fscanf(f, "%s %s", artistName, songName) == 2)
  {
    if (strcmp(artistName, artistNameToSearch) == 0)
    {
      songs[num] = malloc(strlen(artistName) * sizeof(char) + 1);

      if (!songs[num]) // controllo allocazione
      {
        return NULL;
      }

      strcpy(songs[num], songName);
      num++;

      if (num ==SN) // controllo se si è arrivati alla fine del vettore di canzoni
      {
        songs = realloc(songs, (num + SN) * sizeof(char *));
      }
    }
    else if (strcmp(artistName, artistNameToSearch) > 0)
    {
      break;
    }
  }

  songs = realloc(songs, num * sizeof(char *));
  *songNum = num;

  rewind(f);
  return songs;
}
int searchArtista (char * fileNameMetaDati, char *artistNameToSearch)
{

  char artistName[LEN];
  FILE *f;
  if(!(f = fopen(fileNameMetaDati,"r")))
  {
    return -1; //errore apertura file
  }

  while (!feof(f) && fscanf(f, "%s ", artistName) == 1)
  {
    if (strcmp(artistName, artistNameToSearch) == 0)
    {
      fclose(f);
      return 1;// artista trovato
    }

    else if (strcmp(artistName, artistNameToSearch) > 0)
    {
      fclose(f);
      return 0; // artista non trovato
    }
  }
  fclose(f);
  return 0; // artista non trovato
}
void ricercaArtista(char *fileName)
{
  char **songs;
  char artist[LEN];
  int numSong;
  FILE *f;

  if(!(f = fopen(fileName,"r")))
  {
    fprintf(stderr, "Errore nell'apertura del file :%s\n",fileName);
    return;
  }

  printf("Inserisci l'artista da cercare :");
  scanf("%s", artist);

  songs = search(f, artist, &numSong);

  if (numSong == 0)
  {
    printf("Artista non trovato\n");
    return;
  }

  printf("Canzoni dell'artista %s trovate:\n", artist);

  for (int i = 0; i < numSong; i++)
  {
    printf("%d# %s\n", i + 1, songs[i]);
  }

  freeDooblePointer(songs, numSong);
  fclose(f);
  return;
}
int addArtist(char *nominativo, char gruppo, int eta, char *genere,char *fileNameMetaDati)
{
  FILE *f ,*ft;
  char nominativoFile[LEN],genereFile[LEN];
  char gruppoFile;
  int etaFile,pre,id;
  int flag[4];

  if(!(f = fopen(fileNameMetaDati,"r")))
  {
    return -1; //errore apertura file
  }

  if(!(ft = fopen(NAMEFILETEMP,"w")))
  {
    fclose(f);
    return -2;//errore apertura file temporaneo
  }
  pre = 1;

  while(!feof(f) && pre && fscanf(f, "%s %c %d %s", nominativoFile,&gruppoFile,&etaFile,genereFile)==4)
  {
    pre =0;
    flag[0] = strcmp(toUpperString(nominativoFile),toUpperString(nominativo))==0;
    flag[1] = toupper(gruppoFile)==toupper(gruppo);
    flag[2] = etaFile==eta;
    flag[3] = strcmp(toUpperString(genereFile),toUpperString(genere))==0;

    if(flag[0] && flag[1] && flag[2] && flag[3] )
    {
      fclose(f);
      fclose(ft);
      remove(NAMEFILETEMP);
      return -4; // artista gia presente
    }

    if(strcmp(toUpperString(nominativoFile),toUpperString(nominativo))<0)
    {
      pre =1;
    }
    else if(flag[0] && toupper(gruppoFile)<toupper(gruppo))
    {
      pre = 1;
    }
    else if (flag[0] && flag[1] && etaFile<eta )
    {
      pre = 1;
    }
    else if(flag[0] && flag[1] && flag[2] && strcmp(toUpperString(genereFile),toUpperString(genere))<0 )
    {
      pre =1;
    }

    if(pre)
    {
      if(fprintf(ft, "%s %c %d %s\n",nominativoFile,gruppoFile,etaFile,genereFile)<4)
      {
        fclose(f);
        fclose(ft);
        remove(NAMEFILETEMP);
        return -3;//errore inserimento artista
      }
    }
  }

  if(fprintf(ft, "%s %c %d %s\n",nominativo,gruppo,eta,genere)<4)
  {
    fclose(f);
    fclose(ft);
    remove(NAMEFILETEMP);
    return -3;//errore inserimento artista
  }

  id = 4;

  while(!feof(f) && id==4)
  {
    if(fprintf(ft, "%s %c %d %s\n",nominativoFile,gruppoFile,etaFile,genereFile)<4)
    {
      fclose(f);
      fclose(ft);
      remove(NAMEFILETEMP);
      return -3;//errore inserimento artista
    }
    id = fscanf(f, "%s %c %d %s", nominativoFile,&gruppoFile,&etaFile,genereFile);
  }





  fclose(f);
  fclose(ft);
  remove(fileNameMetaDati);
  rename(NAMEFILETEMP, fileNameMetaDati);
  return 0;
}
void inserisciArtista(char *fileNameMetaDati)
{
  char nominativo[LEN],genere[LEN];
  char gruppo;
  int eta;

  printf("Inserisci il nominativo dell'artista:");
  scanf("%s",nominativo);
  getchar();

  do{
    printf("L'artista fa parte di un gruppo (S/N):");
    scanf("%c",&gruppo);
  }while(toupper(gruppo)!='S' && toupper(gruppo)!='N');

  do{
    if(toupper(gruppo)=='S')
    {
      printf("Inserire anni gruppo:");
    }
    else
    {
      printf("Inserire età artista:");
    }
    scanf("%d",&eta);
  }while(eta < 0);

  printf("Inserisci il genere principale dell'artista:");
  scanf("%s",genere);

  switch (addArtist(nominativo,gruppo,eta,genere,fileNameMetaDati))
  {
    case -1:
        fprintf(stderr, "Errore apertura file%s\n",fileNameMetaDati);
        break;
    case -2:
        fprintf(stderr, "Errore apertura file%s\n",NAMEFILETEMP);
        break;
    case -3:
        fprintf(stderr, "Errore inserimento artista\n");
        break;
    case -4:
          fprintf(stderr, "Artista già presente in elenco\n");
          break;
  }
  return;
}
int addSong(char *artistName, char *songName,char *filename)
{
    int numS,id;
    char songN[LEN];
    char ArtistN[LEN];
    char TempCatString[LEN+LEN] ;
    char tempString[LEN + LEN];
    char ** songs;
    FILE *f ,*ft;

    if(!(f = fopen(filename,"r")))
    {
      return -1; //errore apertura file
    }

    songs = search(f, artistName, &numS);
    for (int i = 0; i < numS; i++)
    {
      if (strcmp(songs[i], songName) == 0)
      {
        freeDooblePointer(songs, numS);
        return -4 ; // canzone già presente
      }
    }
    freeDooblePointer(songs, numS);



    if(!(ft = fopen(NAMEFILETEMP,"w")))
    {
      fclose(f);
      return -2;//errore apertura file temporaneo
    }

    toUpperString(strcat(strcpy(TempCatString, artistName),songName));

    while (!feof(f) && fscanf(f, "%s %s", ArtistN, songN)==2 && strcmp(TempCatString, toUpperString(strcat(strcpy(tempString, ArtistN),songN)))>0 )
    {
      if(fprintf(ft,"%s %s\n",ArtistN,songN)<0) // copio i record all'interno dell'file temporaneo
      {
        fclose(f);
        fclose(ft);
        remove(NAMEFILETEMP);
        return -3; // errore inserimento canzone
      }
    }

    if(fprintf(ft,"%s %s\n",artistName,songName)<0) //inserisco il nuovo record all'interno del file temporaneo
    {
      fclose(f);
      fclose(ft);
      remove(NAMEFILETEMP);
      return -3;// errore inserimento canzone
    }

    id = 2;
    while(!feof(f) && id == 2)
    {
      if(fprintf(ft,"%s %s\n",ArtistN,songN)<0) // copio i record all'interno dell'file temporaneo
      {
        fclose(f);
        fclose(ft);
        remove(NAMEFILETEMP);
        return -3;// errore inserimento canzone
      }
      id=fscanf(f, "%s %s", ArtistN, songN);
    }


    fclose(f);
    fclose(ft);
    remove(filename);
    rename(NAMEFILETEMP, filename);
    return 0;
}
void inserisciCanzone(char *filename,char *fileNameMetaDati)
{
  char songName[LEN];
  char artistName[LEN];
  int  numS;
  char **songs;

  printf("Inserisci l'artista da associare alla canzone:");
  scanf("%s", artistName);

  printf("Inserisci il nome della canzone da inserire:");
  scanf("%s", songName);


  if(!searchArtista(fileNameMetaDati,artistName))
  {
    printf("Artista non predente nel file dati artisti, si prega di inserire l'artista\n");
    return;
  }

  switch (addSong(artistName, songName,filename))
  {
    case -1:
        fprintf(stderr, "Errore apertura file%s\n",filename);
        break;
    case -2:
        fprintf(stderr, "Errore apertura file%s\n",NAMEFILETEMP);
        break;
    case -3:
        fprintf(stderr, "Errore inserimento canzone\n");
        break;
    case -4:
          fprintf(stderr, "Canzone già presente in elenco\n");
          break;
  }
  return;
}
int deleteArtist(char *artist,char *fileName,char *fileNameMetaDati)
{
  char songName[LEN],artistName[LEN],genereFile[LEN],gruppoFile;
  int etaFile;

  FILE *f ,*ft;
  if(!(f = fopen(fileName,"r")))
  {
    return -1; //errore apertura file
  }

  if(!(ft = fopen(NAMEFILETEMP,"w")))
  {
    fclose(f);
    return -2;//errore apertura file temporaneo
  }

  while (!feof(f) && fscanf(f, "%s %s", artistName, songName) == 2)
  {
    if (strcmp(artistName, artist) != 0)
    {
      if (fprintf(ft, "%s %s\n", artistName, songName) < 2)
      {
        fclose(f);
        fclose(ft);
        remove(NAMEFILETEMP);
        return -3; //errore eliminazione artista
      }
    }
  }

  fclose(f);
  fclose(ft);
  remove(fileName);
  rename(NAMEFILETEMP, fileName);

  if(!(f = fopen(fileNameMetaDati,"r")))
  {
    return -4; //errore apertura file fileNameMetaDati
  }

  if(!(ft = fopen(NAMEFILETEMP,"w")))
  {
    fclose(f);
    return -2;//errore apertura file temporaneo
  }

  while (!feof(f) && fscanf(f, "%s %c %d %s", artistName,&gruppoFile,&etaFile,genereFile) == 4)
  {
    if (strcmp(artistName, artist) != 0)
    {
      if (fprintf(ft, "%s %c %d %s\n",artistName,gruppoFile,etaFile,genereFile) < 4)
      {
        fclose(f);
        fclose(ft);
        remove(NAMEFILETEMP);
        return -3; //errore eliminazione artista
      }
    }
  }

  fclose(f);
  fclose(ft);
  remove(fileNameMetaDati);
  rename(NAMEFILETEMP, fileNameMetaDati);
  return 0;
}
void eliminaArtista(char *fileName,char *fileNameMetaDati)
{
  char artistName[LEN];
  int numSong;
  char **songs;
  FILE *f;

  printf("Inserisci il nome dell'artista da cancellare:");
  scanf("%s", artistName);

  if(!(f = fopen(fileName,"r")))
  {
    fprintf(stderr, "Errore apertura file%s\n",fileName);
    return ; //errore apertura file fileNameMetaDati
  }
  songs = search(f, artistName, &numSong);
  fclose(f);
  freeDooblePointer(songs, numSong);

  if (numSong == 0)
  {
    printf("Artista non trovato\n");
    return;
  }


  switch (deleteArtist(artistName ,fileName, fileNameMetaDati))
  {
    case -1:
        fprintf(stderr, "Errore apertura file%s\n",fileName);
        break;
    case -2:
        fprintf(stderr, "Errore apertura file%s\n",NAMEFILETEMP);
        break;
    case -3:
        fprintf(stderr, "Errore eliminazione artista\n");
        break;
    case -4:
          fprintf(stderr, "Errore apertura file%s\n",fileNameMetaDati);
          break;
  }
  return;

}
int editArtist(char *oldAlrtist, char *newArtist,char *fileName,char *fileNameMetaDati)
{

  char **songs;
  int numSong,result,etaFile;
  char songName[LEN],artistName[LEN],genereFile[LEN],gruppoFile;
  FILE *f;

  if(!(f = fopen(fileName,"r")))
  {
    fprintf(stderr, "Errore apertura file%s\n",fileName);
    return ; //errore apertura file fileName
  }

  songs = search(f,oldAlrtist,&numSong);
  fclose(f);

  if(!(f = fopen(fileNameMetaDati,"r")))
  {
    fprintf(stderr, "Errore apertura file%s\n",fileName);
    return ; //errore apertura file fileNameMetaDati
  }

  while(fscanf(f, "%s %c %d %s", artistName,&gruppoFile,&etaFile,genereFile) == 4)
  {
    if (strcmp(artistName, oldAlrtist) == 0)
    {
      break;
    }
  }
  fclose(f);

  deleteArtist(oldAlrtist,fileName,fileNameMetaDati);
  addArtist(newArtist,gruppoFile,etaFile,genereFile,fileNameMetaDati);

  result =0;
  for (int i=0;i<numSong;i++)
  {
    result+=addSong(newArtist, songs[i],fileName);
  }
  return result;
}
void modificaArtista(char *fileName,char *fileNameMetaDati)
{
  char oldAlrtist[LEN];
  char newArtist[LEN];
  int statusID, numSong;
  char **songs;
  FILE *f;

  printf("Inserisci il nome dell'artista da modificare:");
  scanf("%s", oldAlrtist);



  if(!(f = fopen(fileName,"r")))
  {
    fprintf(stderr, "Errore apertura file%s\n",fileName);
    return ; //errore apertura file fileNameMetaDati
  }

  songs = search(f,oldAlrtist,&numSong);
  fclose(f);

  freeDooblePointer(songs, numSong);

  if (numSong == 0)
  {
    printf("Artista non trovato,si prega di inserire artista\n");
    return;
  }

  printf("Inserisci il Nuovo nome dell'artista:");
  scanf("%s", newArtist);

  if (editArtist(oldAlrtist, newArtist,fileName,fileNameMetaDati)!=0)
  {
    printf("Errore modifica artista\n");
  }
}
void freeDooblePointer(void **p, int num)
{
  for (int i = 0; i < num; i++)
  {
    free(p[i]);
  }
  free(p);
}
char *toUpperString(char *S)
{
  int lun = (strlen(S));
  char *p = calloc((lun + 1) , sizeof(char));

  if (!p)
  {
    return NULL;
  }

  for (int i = 0; i < lun; i++)
  {
    p[i] = toupper(S[i]);
  }

  return p;
}

void query(char *fileName,char *fileNameMetaDati)
{
  char songName[LEN],artistName[LEN],genereFile[LEN],gruppoFile;
  int etaFile,nums;
  char ** song;

  FILE *f ,*fmd;
  if(!(f = fopen(fileName,"r")))
  {
    return ; //errore apertura file
  }

  if(!(fmd = fopen(fileNameMetaDati,"r")))
  {
    fclose(fmd);
    return ;//errore apertura file metadati
  }

  while (!feof(fmd) && fscanf(fmd, "%s %c %d %s", artistName,&gruppoFile,&etaFile,genereFile) == 4)
  {
    if (toupper(gruppoFile)=='S' && etaFile<5)
    {
      song = search(f,artistName,&nums);
      if(nums>0)
      {
        printf("%s appartenente ad un gruppo da meno di 5 anni ed ha composto le seguenti canzoni:\n",artistName );
        for(int i=0;i<nums;i++)
        {
          printf("#%d %s\n",i+1,song[i]);
        }
      }
      freeDooblePointer(song, nums);
    }
    else if (toupper(gruppoFile)=='N' && etaFile<30)
    {
      song = search(f,artistName,&nums);

      if(nums>0)
      {
        printf("%s Ha un età anagrafica minore di 30 anni ed ha composto le seguenti canzoni:\n",artistName);
        for(int i=0;i<nums;i++)
        {
          printf("#%d %s\n",i+1,song[i]);
        }
      }
      freeDooblePointer(song,nums);
    }
  }
  return ;
}
