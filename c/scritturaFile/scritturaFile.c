#include <stdio.h>


int main(void)
{
  char matricola [30];
  char nome [50];
  double media_voti;

  FILE *f;

  f=fopen("studenti.txt","w");
  if(!f)
  {
    fprintf(stderr,"Errore nell'apertura del file");
  }
  else
  {
    printf("Inserisci la matricola, il nome e la media dei voti di uno studente\n");
    printf("Inserisci EOF per concludere l'inserimento dei dati\n");
    printf("?");
    scanf("%s%s%lf",matricola,nome,&media_voti);

    while(!feof(stdin))
    {
      fprintf(f, "%s %s %lf \n",matricola, nome, media_voti);
      printf("?");
      scanf("%s%s%lf",matricola,nome,&media_voti);
    }
    fclose(f);
  }
  return 0;
}
