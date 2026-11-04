#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <iomanip>

using namespace std;

enum Couleur { ROUGE, NOIR };

struct Noeud {
    int valeur;
    Couleur couleur;
    Noeud* gauche;
    Noeud* droite;
    Noeud* parent;
};

Noeud* creerNoeud(int val) {
    Noeud* n = new Noeud;
    n->valeur = val;
    n->couleur = ROUGE;
    n->gauche = nullptr;
    n->droite = nullptr;
    n->parent = nullptr;
    return n;
}

void rotationGauche(Noeud*& racine, Noeud* x) {
    Noeud* y = x->droite;

    x->droite = y->gauche;

    if (y->gauche)
        y->gauche->parent = x;

    y->parent = x->parent;

    if (!x->parent)
        racine = y;
    else if (x == x->parent->gauche)
        x->parent->gauche = y;
    else
        x->parent->droite = y;

    y->gauche = x;
    x->parent = y;
}

void rotationDroite(Noeud*& racine, Noeud* x) {
    Noeud* y = x->gauche;

    x->gauche = y->droite;

    if (y->droite)
        y->droite->parent = x;

    y->parent = x->parent;

    if (!x->parent)
        racine = y;
    else if (x == x->parent->droite)
        x->parent->droite = y;
    else
        x->parent->gauche = y;

    y->droite = x;
    x->parent = y;
}

void corrigerInsertion(Noeud*& racine, Noeud* z) {
    while (z != racine && z->parent->couleur == ROUGE) {

        Noeud* parent = z->parent;
        Noeud* gp = parent->parent;

        if (parent == gp->gauche) {

            Noeud* oncle = gp->droite;

            if (oncle && oncle->couleur == ROUGE) {
                parent->couleur = NOIR;
                oncle->couleur = NOIR;
                gp->couleur = ROUGE;
                z = gp;
            }
            else {
                if (z == parent->droite) {
                    z = parent;
                    rotationGauche(racine, z);
                }

                z->parent->couleur = NOIR;
                z->parent->parent->couleur = ROUGE;

                rotationDroite(racine, z->parent->parent);
            }
        }
        else {

            Noeud* oncle = gp->gauche;

            if (oncle && oncle->couleur == ROUGE) {
                parent->couleur = NOIR;
                oncle->couleur = NOIR;
                gp->couleur = ROUGE;
                z = gp;
            }
            else {
                if (z == parent->gauche) {
                    z = parent;
                    rotationDroite(racine, z);
                }

                z->parent->couleur = NOIR;
                z->parent->parent->couleur = ROUGE;

                rotationGauche(racine, z->parent->parent);
            }
        }
    }

    racine->couleur = NOIR;
}

void inserer(Noeud*& racine, int val) {
    Noeud* z = creerNoeud(val);

    Noeud* parent = nullptr;
    Noeud* courant = racine;

    while (courant) {
        parent = courant;

        if (val < courant->valeur)
            courant = courant->gauche;
        else if (val > courant->valeur)
            courant = courant->droite;
        else {
            delete z;
            return;
        }
    }

    z->parent = parent;

    if (!parent)
        racine = z;
    else if (val < parent->valeur)
        parent->gauche = z;
    else
        parent->droite = z;

    corrigerInsertion(racine, z);
}

void liberer(Noeud* racine) {
    if (!racine)
        return;

    liberer(racine->gauche);
    liberer(racine->droite);

    delete racine;
}

void testerInsertion(int N) {

    Noeud* racine = nullptr;

    vector<int> valeurs;

    for (int i = 1; i <= N; i++)
        valeurs.push_back(i);

    mt19937 gen(42);
    shuffle(valeurs.begin(), valeurs.end(), gen);

    // Mesure de l'arbre Rouge-Noir
    auto debut = chrono::high_resolution_clock::now();

    for (int v : valeurs)
        inserer(racine, v);

    auto fin = chrono::high_resolution_clock::now();

    double tempsRB =
        chrono::duration<double, milli>(fin - debut).count();

    // Mesure de map
    map<int, int> arbreMap;

    debut = chrono::high_resolution_clock::now();

    for (int v : valeurs)
        arbreMap.insert({v, v});

    fin = chrono::high_resolution_clock::now();

    double tempsMap =
        chrono::duration<double, milli>(fin - debut).count();

    // Affichage des résultats
    cout << left << setw(15) << N
         << right << setw(20) << fixed << setprecision(4) << tempsRB
         << setw(20) << tempsMap
         << setw(20) << (tempsRB * 1000 / N)
         << setw(20) << (tempsMap * 1000 / N)
         << endl;

    liberer(racine);
}

int main() {

    cout << "\n";
    cout << "*******COMPARAISON DES INSERTIONS ********\n\n";

    cout << left << setw(15) << "Elements"
         << right << setw(20) << "Rouge-Noir (ms)"
         << setw(20) << "Map (ms)"
         << setw(20) << "RN moyen (us)"
         << setw(20) << "Map moyen (us)"
         << endl;

    cout << string(95, '-') << endl;

    testerInsertion(1000);
    testerInsertion(10000);
    testerInsertion(100000);
    testerInsertion(1000000);

    cout << string(95, '-') << endl;

    return 0;
}
