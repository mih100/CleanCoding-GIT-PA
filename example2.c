#include <stdio.h>
#include <stdlib.h>

typedef struct Nod {
    int valoare;
    struct Nod *urmator;
} Nod;

typedef struct Graf {
    int nr_noduri;
    int *vizitat;
    Nod **liste_adiacenta;
} Graf;

// Creează un nod nou
Nod *creeaza_nod(int valoare) {
    Nod *nod_nou = malloc(sizeof(Nod));
    nod_nou->valoare = valoare;
    nod_nou->urmator = NULL;
    return nod_nou;
}

// Creează graful
Graf *creeaza_graf(int nr_noduri) {
    Graf *graf = malloc(sizeof(Graf));
    graf->nr_noduri = nr_noduri;
    graf->liste_adiacenta = malloc(nr_noduri * sizeof(Nod *));
    graf->vizitat = malloc(nr_noduri * sizeof(int));

    for (int i = 0; i < nr_noduri; i++) {
        graf->liste_adiacenta[i] = NULL;
        graf->vizitat[i] = 0;
    }
    return graf;
}

// Adaugă o muchie între două noduri
void adauga_muchie(Graf *graf, int sursa, int destinatie) {
    Nod *nod_nou = creeaza_nod(destinatie);
    nod_nou->urmator = graf->liste_adiacenta[sursa];
    graf->liste_adiacenta[sursa] = nod_nou;

    nod_nou = creeaza_nod(sursa);
    nod_nou->urmator = graf->liste_adiacenta[destinatie];
    graf->liste_adiacenta[destinatie] = nod_nou;
}

// Introduce muchii în graf
void introdu_muchii(Graf *graf, int nr_muchii) {
    int sursa, destinatie;
    printf("Introdu %d muchii (noduri numerotate de la 0 la %d):\n", nr_muchii, graf->nr_noduri - 1);
    for (int i = 0; i < nr_muchii; i++) {
        scanf("%d %d", &sursa, &destinatie);
        adauga_muchie(graf, sursa, destinatie);
    }
}

// Coada pentru BFS
int este_goala(Nod *coada) {
    return coada == NULL;
}

void adauga_in_coada(Nod **coada, int valoare) {
    Nod *nod_nou = creeaza_nod(valoare);
    if (*coada == NULL) {
        *coada = nod_nou;
    } else {
        Nod *temp = *coada;
        while (temp->urmator)
            temp = temp->urmator;
        temp->urmator = nod_nou;
    }
}

int scoate_din_coada(Nod **coada) {
    int valoare = (*coada)->valoare;
    Nod *temp = *coada;
    *coada = (*coada)->urmator;
    free(temp);
    return valoare;
}

// Parcurgere DFS
void DFS(Graf *graf, int nod_start) {
    Nod *lista_vecini = graf->liste_adiacenta[nod_start];
    Nod *temp = lista_vecini;

    graf->vizitat[nod_start] = 1;
    printf("%d ", nod_start);

    while (temp != NULL) {
        int nod_vecin = temp->valoare;

        if (graf->vizitat[nod_vecin] == 0) {
            DFS(graf, nod_vecin);
        }
        temp = temp->urmator;
    }
}

// Parcurgere BFS
void BFS(Graf *graf, int nod_start) {
    Nod *coada = NULL;
    graf->vizitat[nod_start] = 1;
    adauga_in_coada(&coada, nod_start);

    while (!este_goala(coada)) {
        int nod_curent = scoate_din_coada(&coada);
        printf("%d ", nod_curent);

        Nod *temp = graf->liste_adiacenta[nod_curent];
        while (temp) {
            int nod_vecin = temp->valoare;
            if (graf->vizitat[nod_vecin] == 0) {
                graf->vizitat[nod_vecin] = 1;
                adauga_in_coada(&coada, nod_vecin);
            }
            temp = temp->urmator;
        }
    }
}

// Resetare listă de vizitare
void reseteaza_vizitat(Graf *graf) {
    for (int i = 0; i < graf->nr_noduri; i++) {
        graf->vizitat[i] = 0;
    }
}

int main() {
    int nr_noduri, nr_muchii, nod_start;

    printf("Câte noduri are graful? ");
    scanf("%d", &nr_noduri);

    printf("Câte muchii are graful? ");
    scanf("%d", &nr_muchii);

    Graf *graf = creeaza_graf(nr_noduri);
    introdu_muchii(graf, nr_muchii);

    printf("\nDe unde începem DFS? ");
    scanf("%d", &nod_start);
    printf("Parcurgere DFS: ");
    DFS(graf, nod_start);

    reseteaza_vizitat(graf);

    printf("\n\nDe unde începem BFS? ");
    scanf("%d", &nod_start);
    printf("Parcurgere BFS: ");
    BFS(graf, nod_start);

    printf("\n");
    return 0;
}
