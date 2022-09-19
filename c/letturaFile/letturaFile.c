#include <stdio.h>


int main(void)
{
  char matricola [30];
  char nome [50];
  double media_voti;

  FILE *f;

  f=fopen("studenti.txt","r");
  if(!f)
  {
    fprintf(stderr,"Errore nell'apertura del file");
  }
  else
  {
    printf("%s\t %s\t %s\n","Matricola","Nome","Media_Voti");
    fscanf(f,"%s%s%lf",matricola,nome,&media_voti);
    while(!feof(f))
    {
      printf("%s\t %s\t %lf\n",matricola,nome,media_voti);
      fscanf(f,"%s%s%lf",matricola,nome,&media_voti);
    }
    fclose(f);
  }
  return 0;
}
