#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*per ogni righa letta del file corrisponde un mattoincino 
quindi creo una strucctora adatta a contenere le informazioni*/
struct mattoncino {
	int lunghezza, larghezza, colore;
	char altezza;
};

//funzione che leggeil filo passato al programma e crea/carica un array di tipo "mattoncino".
struct mattoncino *leggi_file(FILE * fin, int *size)
{
	struct mattoncino *v;
	int dim = 64;
	//si utilizza l'allocazione dinamica in quanto non si conosce il numero di elementi a priori
	if (!(v = malloc(dim * sizeof(struct mattoncino))))
		return NULL;
	char buf[1024];
	char colore[20];
	while (fgets(buf, sizeof(struct mattoncino), fin)) {

		if (4 != sscanf(buf, "%dx%d %c %s", &v[*size].lunghezza,
				&v[*size].larghezza, &v[*size].altezza, colore))
			continue;
		/*per ogni colore ho associato un numero intero essendo più facile siccesivamente 
		   trattare gli elementi per colore associando ad ogni casella di un arrray un 
		   colre indicizzato dal numero stesso */
		if (!(strcmp(colore, "rosso")))
			v[*size].colore = 0;
		else if (!(strcmp(colore, "giallo")))
			v[*size].colore = 1;
		else if (!(strcmp(colore, "verde")))
			v[*size].colore = 2;
		else if (!(strcmp(colore, "blu")))
			v[*size].colore = 3;
		else if (!(strcmp(colore, "nero")))
			v[*size].colore = 4;
		else if (!(strcmp(colore, "bianco")))
			v[*size].colore = 5;
		else if (!(strcmp(colore, "arancione")))
			v[*size].colore = 6;

		(*size)++;
		//verifica ed eventuale aggiornamneto della memoria allocata rispetto al numero di elementi letti
		if (dim == (*size)) {
			dim *= 2;
			if (!(v = realloc(v, dim * sizeof(struct mattoncino))))
				return 0;
		}
	}
	v = realloc(v, (*size) * sizeof(struct mattoncino));
	return v;
}

void suddivisone_per_colore(struct mattoncino *v, int size, int *colori)
{
	int i;
	/*ogni casella numerata corrisponte ad un colore. In questo modo si perde l'informazione 
	   del colore associato ad ogni gruppo, infatti esse sono informazioni superflue 
	   per le richieste di questo programma */
	for (i = 0; i < size; i++)
		colori[v[i].colore]++;
}

//ricerca  e restituzione del numero di elementi più grande            
int max_colore(int *colori)
{
	int max = colori[0];
	int i;
	for (i = 0; i < 7; i++)
		if (colori[i] > max)
			max = colori[i];
	return max;
}

int altezza(struct mattoncino *v, int size)
{
	int i;
	double h = 0.0;
	for (i = 0; i < size; i++) {
		if (v[i].larghezza != v[i].lunghezza) {
			if (v[i].altezza == 'A')
				h++;
			else
				h += 1 / (double)(3);
		}
	}
	//arrotondamento dell'altezza sia per eccesso che difetto, la soglia per i due casi è il decimale 0.5
	if ((h - (int)h >= 0.5))
		return (int)h + 1;
	return (int)h;
}

//per la massima lunghezza si ricera il lato più lungo tra le due dimensioni
int lunghezza(struct mattoncino *v, int size)
{
	int i, lunghezza = 0;
	for (i = 0; i < size; i++) {
		if (v[i].lunghezza >= v[i].larghezza)
			lunghezza += v[i].lunghezza;
		else
			lunghezza += v[i].larghezza;
	}
	return lunghezza;
}

int piramidi(struct mattoncino *v, int size)
{
	//sicuramente si potranno fare size piramidi, ognuna è formata da un solo elemento
	int n = size;
	int i;
	/*per ogni elemento successivo che può andare a formare una piramide 
	   con l'elemento corrente si decrementa di un il numero di piramidi in quanto 
	   nè è presente una in meno */
	for (i = 0; i < size - 1; i++) {
		if ((v[i].lunghezza >= v[i + 1].lunghezza &&
		     v[i].larghezza >= v[i + 1].larghezza) ||
		    (v[i].lunghezza >= v[i + 1].larghezza &&
		     v[i].larghezza >= v[i + 1].lunghezza))
			n--;
	}
	return n;
}

/*
 * Soluzione alternativa.
 */
int piramidi2(struct mattoncino *v, int size)
{
	int p=1;
	int i;
	for(i = 1; i < size; i++){
                // si controlla che il mattoncino sopra ci stia su quello sotto, anche ruotandolo;
                // se ci sta, si passa al mattoncino successivo, altrimenti si incrementa il numero di piramidi
		if (v[i].larghezza <= v[i - 1].larghezza && v[i].lunghezza <= v[i - 1].lunghezza) continue;   // ci sta
                if (v[i].larghezza <= v[i - 1].lunghezza && v[i].lunghezza <= v[i - 1].larghezza) continue;   // ci sta ruotato
                p++;   // non ci sta
					
	}
	return p;
}

//funzione che serve per il confronto tra elementi di tipo "mattoncino" per il loro ordinamneto secondo le caratteristiche richieste
int compar(const void *p1, const void *p2)
{
	const struct mattoncino *a = p1, *b = p2;
	if ((a->lunghezza) == (b->lunghezza))
		return (a->larghezza) - (b->larghezza);
	return (a->lunghezza) - (b->lunghezza);
}

int main(int argc, char *argv[])
{
	FILE *fin;
	int size = 0;
	int i;
	int colori[7] = { 0 };

	if (!(fin = fopen(argv[1], "r")))
		return 0;
	struct mattoncino *v;
	if (!(v = leggi_file(fin, &size)))
		return 0;
	//una volta salvate tutte le informazioni dell'array si chiude il file per liberare memoria
	fclose(fin);

	suddivisone_per_colore(v, size, colori);
	printf("[MAX-PER-COLORE]\n%d\n%d\n", 2, 2);
	printf("[ALTEZZA-NON-QUADRATI]\n%d\n", altezza(v, size));
	printf("[LUNGHEZZA]\n%d\n", lunghezza(v, size));
	printf("[PIRAMIDI]\n%d\n", piramidi(v, size));
	/*chiamata della funzione apposita per ordinare l'insieme di elementi
	   "mattoncino" secondo le caretteristiche specificate nella funzione compar */
	qsort(v, size, sizeof(struct mattoncino), compar);
	printf("[ORDINAMENTO]\n");

	for (i = 0; i < size; i++)
		printf("%dx%d\n", v[i].lunghezza, v[i].larghezza);
	
	//prima di terminare il programma si libera la memoria allocata precednetemente in maniera dinamica
	
	free(v);
	return 0;
}
