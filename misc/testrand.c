#include	<string.h>
#include	<stdlib.h>
#include	<signal.h>

#include	<lib/gerais.h>
#include	<li/lib/li.h>
#include	<muta/lib/muta.h>
#include	<tcase/lib/tcase.h>
#include	<pteste/pteste.h>


extern	OPERADOR_MUTACAO	g_tab_operador[];

main(argc, argv)
int	argc;
char	*argv[];
{
int i, n;
double cont = 0.0;

    init_sort2(0, atol(argv[3]));
    g_tab_operador[0].percentage = atof(argv[1]);
    n = atoi(argv[2]);
    for (i = 0; i < n; i++)
    {
        if (sorteio2(0))
            cont++;
        //op_rand_int(0, i+1);
        // printf("%d-%d  ", i+1, op_rand_int(0, i+1));
    }
    printf("\n\n%lf %lf\n" , g_tab_operador[0].percentage, cont/n);

    cont = 0;
    init_sort(0, atol(argv[3]));
    g_tab_operador[0].percentage = atof(argv[1]);
    n = atoi(argv[2]);
    for (i = 0; i < n; i++)
    {
        if (sorteio(0))
            cont++;
        //op_rand_int(0, i+1);
        // printf("%d-%d  ", i+1, op_rand_int(0, i+1));
    }
    printf("\n\n%lf %lf\n" , g_tab_operador[0].percentage, cont/n);
}
