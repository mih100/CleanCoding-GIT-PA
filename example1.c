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

typedef struct Stiva {
    int varf;
    int capacitate;
    int *elemente;
} Stiva;

// Creează un nod nou
Nod* creeaza_nod(int valoare) {
    Nod *nod_nou = (Nod *)malloc(sizeof(Nod));
    nod_nou->valoare = valoare;
    nod_nou->urmator = NULL;
    return nod_nou;
}

// Creează un graf cu n noduri
Graf* creeaza_graf(int nr_noduri) {
    Graf *graf = (Graf *)malloc(sizeof(Graf));
    graf->nr_noduri = nr_noduri;
    graf->vizitat = (int *)calloc(nr_noduri, sizeof(int));
    graf->liste_adiacenta = (Nod **)malloc(nr_noduri * sizeof(Nod *));

    for (int i = 0; i < nr_noduri; i++) {
        graf->liste_adiacenta[i] = NULL;
    }

    return graf;
}

// Adaugă muchie între două noduri (neorientat)
void adauga_muchie(Graf *graf, int sursa, int destinatie) {
    Nod *nod_nou = creeaza_nod(destinatie);
    nod_nou->urmator = graf->liste_adiacenta[sursa];
    graf->liste_adiacenta[sursa] = nod_nou;

    nod_nou = creeaza_nod(sursa);
    nod_nou->urmator = graf->liste_adiacenta[destinatie];
    graf->liste_adiacenta[destinatie] = nod_nou;
}

// Creează o stivă
Stiva* creeaza_stiva(int capacitate) {
    Stiva *s = (Stiva *)malloc(sizeof(Stiva));
    s->capacitate = capacitate;
    s->varf = -1;
    s->elemente = (int *)malloc(capacitate * sizeof(int));
    return s;
}

// Adaugă element în stivă
void push(Stiva *s, int valoare) {
    if (s->varf < s->capacitate - 1) {
        s->elemente[++(s->varf)] = valoare;
    }
}

// DFS pentru a marca nodurile accesibile
void dfs(Graf *graf, int nod, Stiva *s) {
    graf->vizitat[nod] = 1;
    push(s, nod);

    Nod *vecin = graf->liste_adiacenta[nod];
    while (vecin != NULL) {
        if (!graf->vizitat[vecin->valoare]) {
            dfs(graf, vecin->valoare, s);
        }
        vecin = vecin->urmator;
    }
}

// Resetează vectorul de vizitat
void reset_vizitat(Graf *graf) {
    for (int i = 0; i < graf->nr_noduri; i++) {
        graf->vizitat[i] = 0;
    }
}

// Verifică dacă există drum între două noduri
int exista_drum(Graf *graf, int sursa, int destinatie) {
    Stiva *s = creeaza_stiva(graf->nr_noduri);
    reset_vizitat(graf);
    dfs(graf, sursa, s);

    for (int i = 0; i <= s->varf; i++) {
        if (s->elemente[i] == destinatie) {
            free(s->elemente);
            free(s);
            return 1;  // Drum găsit
        }
    }

    free(s->elemente);
    free(s);
    return 0;  // Drum inexistent
}

// Citire muchii de la utilizator
void citeste_muchii(Graf *graf, int nr_muchii) {
    int sursa, destinatie;
    printf("Introdu %d muchii (format: nod1 nod2):\n", nr_muchii);
    for (int i = 0; i < nr_muchii; i++) {
        scanf("%d %d", &sursa, &destinatie);
        adauga_muchie(graf, sursa, destinatie);
    }
}

int main() {
    int nr_noduri, nr_muchii;
    int restaurant1, restaurant2;

    printf("Cate restaurante (noduri) sunt in retea? ");
    scanf("%d", &nr_noduri);

    printf("Cate conexiuni (muchii) sunt intre restaurante? ");
    scanf("%d", &nr_muchii);

    Graf *graf = creeaza_graf(nr_noduri);
    citeste_muchii(graf, nr_muchii);

    printf("Verificam daca exista drum intre doua restaurante.\n");
    printf("Introdu indexul primului restaurant: ");
    scanf("%d", &restaurant1);
    printf("Introdu indexul celui de-al doilea restaurant: ");
    scanf("%d", &restaurant2);

    if (exista_drum(graf, restaurant1, restaurant2)) {
        printf("Exista drum direct sau indirect intre restaurantul %d si %d.\n", restaurant1, restaurant2);
    } else {
        printf("NU exista drum intre restaurantul %d si %d.\n", restaurant1, restaurant2);
    }

    return 0;
}
