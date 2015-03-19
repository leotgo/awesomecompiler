#include "cc_misc.h"
extern int lineCounter;

int getLineNumber (void)
{
  return lineCounter;
}

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem); //altere para que apareça a linha
}

void main_init (int argc, char **argv)
{
  //implemente esta função com rotinas de inicialização, se necessário
}

void main_finalize (void)
{
  //implemente esta função com rotinas de inicialização, se necessário
}
