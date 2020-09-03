#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

/* Función para devolver un error en caso de que ocurra */
void error(const char *s);
/*Funcion para imprimir asteriscos*/
void imprimir(int numTotalArchivos, int numArchivos);
/* Calculamos el tamaño del archivo */
long fileSize(char *fname);

/* Sacamos el tipo de archivo haciendo un stat(), es como el stat de la línea de comandos */
unsigned char statFileType(char *fname);

/* Intenta sacar el tipo de archivo del ent */
unsigned char getFileType(char *ruta, struct dirent *ent);

/* Obtiene el nombre del fichero con la ruta completa */
char *getFullName(char *ruta, struct dirent *ent);

/* Genera una cadena de espacios, para dibujar el árbol */
char *generaPosStr(int niv);

/* Función principal, que cuenta archivos */
unsigned cuentaArchivos(char *ruta, int niv);

int sizes2 [5];
int numTotal;
int main(int argc, char *argv[])
{

  unsigned num;
  char * urnas[5] = {"0-1023   ", "1024-2047", "2048-3071", "3072-4095", "<4095    "};
  if (argc != 2)
    {
      error("Uso: ./directorio_2 <ruta>\n");
    }

  printf("Entrando en: %s\n", argv[1]);
  num=cuentaArchivos(argv[1], 1);
  printf("%s . Total: %u archivos\n", argv[1], num);

  //Imprimir valores
  size_t n = sizeof(sizes2)/sizeof(sizes2[0]);
  for (int i=0; i<n; i++){
    numTotal+=sizes2[i];
    //printf("\n Total de archivos en %d es %d", i, sizes2[i]);
  }
  printf("URNA\tNum. archivos \t Histograma");
  double asteriscos;
  for (int i=0; i<n; i++){
    double num = sizes2[i];
    double denom = numTotal;
    asteriscos=((num/denom)*10);
    if(asteriscos<1){
      if(asteriscos==0){
        asteriscos = 0;
      }
      else{
        asteriscos=1;
      }

    }
    printf("\n%s \t%d\t", urnas[i], sizes2[i]);
   for(int j=0; j<(int)asteriscos;j++){
     printf("*");
   }
  }
  printf("\nTotal de archivos en ese path: %d", numTotal);
  /* Empezaremos a leer en el directorio actual */

  return EXIT_SUCCESS;
}

void error(const char *s)
{
  /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
  perror (s);
  exit(EXIT_FAILURE);
}

char *getFullName(char *ruta, struct dirent *ent)
{
  char *nombrecompleto;
  int tmp;

  tmp=strlen(ruta);
  nombrecompleto=malloc(tmp+strlen(ent->d_name)+2); /* Sumamos 2, por el \0 y la barra de directorios (/) no sabemos si falta */
  if (ruta[tmp-1]=='/')
    sprintf(nombrecompleto,"%s%s", ruta, ent->d_name);
  else
    sprintf(nombrecompleto,"%s/%s", ruta, ent->d_name);
 
  return nombrecompleto;
}

char *generaPosStr(int niv)
{
  int i;
  char *tmp=malloc(niv*2+1);    /* Dos espacios por nivel más terminador */
  for (i=0; i<niv*2; ++i)
    tmp[i]=' ';
  tmp[niv*2]='\0';
  return tmp;
}

unsigned cuentaArchivos(char *ruta, int niv)
{
  /* Con un puntero a DIR abriremos el directorio */
  DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;
  unsigned numfiles=0;          /* Ficheros en el directorio actual */
  unsigned char tipo;       /* Tipo: fichero /directorio/enlace/etc */
  char *nombrecompleto;     /* Nombre completo del fichero */
  char *posstr;         /* Cadena usada para posicionarnos horizontalmente */
  dir = opendir (ruta);

  /* Miramos que no haya error */
  if (dir == NULL)
    error("No puedo abrir el directorio");
 
  while ((ent = readdir (dir)) != NULL)
    {
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
    {
      nombrecompleto=getFullName(ruta, ent);
      tipo=getFileType(nombrecompleto, ent);
      if (tipo==DT_REG)
        {
          ++numfiles;
        }
      else if (tipo==DT_DIR)
        {
          posstr=generaPosStr(niv);         
          printf("%s%s . Total: %u archivos ", posstr, nombrecompleto, cuentaArchivos(nombrecompleto, niv+1));
          /* Podemos poner las líneas que queramos */
          printf("\n");
          free(posstr);
        }
      free(nombrecompleto);
    }
    }
  closedir (dir);
  return numfiles;
}

unsigned char getFileType(char *nombre, struct dirent *ent)
{
  unsigned char tipo;
  struct stat sdata;
  long int size;
  tipo=ent->d_type;
  if (tipo==DT_UNKNOWN)
    {
      tipo=statFileType(nombre);
    }
stat(nombre, &sdata);
size = sdata.st_size;

if(size>0 && size<1024){
  sizes2[0]=sizes2[0]+1;
}
if(size>1024 && size<2048){
  sizes2[1]=sizes2[1]+1;
}
if(size>2048 && size<3072){
  sizes2[2]=sizes2[2]+1;
}
if(size>3072 && size<4095){
  sizes2[3]=sizes2[3]+1;
}
else{
  sizes2[4]=sizes2[4]+1;
}
  return tipo;
}

/* stat() vale para mucho más, pero sólo queremos el tipo ahora */
unsigned char statFileType(char *fname)
{
  struct stat sdata;
  /* Intentamos el stat() si no funciona, devolvemos tipo desconocido */
  if (stat(fname, &sdata)==-1)
    {
      return DT_UNKNOWN;
    }

  switch (sdata.st_mode & S_IFMT)
    {
    case S_IFBLK:  return DT_BLK;
    case S_IFCHR:  return DT_CHR;
    case S_IFDIR:  return DT_DIR;
    case S_IFIFO:  return DT_FIFO;
    case S_IFLNK:  return DT_LNK;
    case S_IFREG:  return DT_REG;
    case S_IFSOCK: return DT_SOCK;
    default:       return DT_UNKNOWN;
    }
}