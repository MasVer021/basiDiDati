#include "Utility.h"


int main(void)
{
  char fileName[LEN];
  char fileNameMetaDati[LEN];
  char artist[LEN];
  FILE *fp;
  char **songs;
  int menuID, submenuID, statusID;

  printf("Inserisci il nome del file contentente il database di canzoni:");
  scanf("%s", fileName);
  printf("File %s loaded\n", fileName);

  printf("Inserisci il nome del file contentente i dati degli artisti:");
  scanf("%s", fileNameMetaDati);
  printf("File %s loaded\n", fileNameMetaDati);

  do
  {
    printf("1-Ricerca canzoni in base all'autore\n2-Inserici una nuova "
           "canzone\n3-Modifica nome autore\n4-Elimina artista e sue "
           "canzoni\n5-Inserisci Artista\n6-Cambia file contenente il database\n7-Visualizza le canzoni degli artisti facenti parte di un gruppo da meno di 5 anni o con meno di 30 anni di eta\n0-Esci\n");
    scanf("%d", &menuID);
    switch (menuID)
    {
      case 1:
        do
        {
          ricercaArtista(fileName);
          printf("1-Ricerca altro autore\n0-Menu principale\n");
          scanf("%d", &submenuID);
        } while (submenuID != 0);
        break;

      case 2:
        do
        {
          inserisciCanzone(fileName,fileNameMetaDati);
          printf("1-Inserisci una nuova canzone\n0-Menu principale\n");
          scanf("%d", &submenuID);
        } while (submenuID != 0);
        break;

      case 3:
        do
        {
          modificaArtista(fileName,fileNameMetaDati);
          printf("1-Modifica nuovo artista\n0-Menu principale\n");
          scanf("%d", &submenuID);
        } while (submenuID != 0);
        break;

      case 4:
        do
        {
          eliminaArtista(fileName,fileNameMetaDati);
          printf("1-Elimina altro artista\n0-Menu principale\n");
          scanf("%d", &submenuID);
        } while (submenuID != 0);
        break;
      case 5:
        do
        {
          inserisciArtista(fileNameMetaDati);
          printf("1-Inserisci altro artista\n0-Menu principale\n");
          scanf("%d", &submenuID);
        } while (submenuID != 0);
        break;
      case 6:
        printf("Inserisci il nome del file contentente il database di canzoni:");
        scanf("%s", fileName);
        printf("File %s loaded\n", fileName);
        break;
      case 7:
          query(fileName,fileNameMetaDati);
          break;

    }
  } while (menuID != 0);

  return 0;
}
