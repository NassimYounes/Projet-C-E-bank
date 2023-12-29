#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Compte {
    int numero;
    char nom[50];
    float solde;
};

int numeroCompteGlobal = 0; // N de compte global
////////////////////////////creer nv compte en lui donnant un numéro/////////////////////
void creerCompte() {
    FILE *fichier = fopen("donnees.txt", "a");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    struct Compte nouveauCompte;

    printf("Entrez le nom du client : ");
    scanf("%s", nouveauCompte.nom);

    printf("Entrez le solde initial : ");
    scanf("%f", &nouveauCompte.solde);
    numeroCompteGlobal++;
    nouveauCompte.numero = numeroCompteGlobal;
    fprintf(fichier, "%d %s %.2f\n", nouveauCompte.numero, nouveauCompte.nom, nouveauCompte.solde);
    fclose(fichier);

    printf("Le compte a été créé avec succès. Numéro de compte : %d\n", nouveauCompte.numero);
}

///////changer le montant d'un compte à partir de numéro////////////////////
void mettreAJourCompte(int numeroCompte) {
    FILE *fichier = fopen("donnees.txt", "r+");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    struct Compte compte;
    int compteTrouve = 0;

    // Parcourir le fichier jusqu'au compte avec le numéro spécifié
    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) != EOF) {
        if (compte.numero == numeroCompte) {
            float nouveauSolde;
            printf("Entrez le nouveau solde : ");
            scanf("%f", &nouveauSolde);

            compte.solde = nouveauSolde;

            // Remettre le curseur du fichier à la position du compte
            fseek(fichier, -((long int)sizeof(struct Compte)), SEEK_CUR);

            // Écrire les nouvelles informations du compte
            fprintf(fichier, "%d %s %.2f\n", compte.numero, compte.nom, compte.solde);
            compteTrouve = 1;
            break;
        }
    }

    fclose(fichier);

    if (!compteTrouve) {
        printf("Le compte %d n'a pas été trouvé.\n", numeroCompte);
    }
}

void gererTransactions(int numeroCompte) {
    FILE *fichier = fopen("donnees.txt", "r+");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    struct Compte compte;
    int compteTrouve = 0;

    // Créer un fichier temporaire pour stocker les comptes mis à jour
    FILE *tempFichier = fopen("tempdonnees.txt", "w");
    if (tempFichier == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(fichier);
        return;
    }

    // Parcourir le fichier jusqu'au compte avec le numéro spécifié
    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) != EOF) {
        if (compte.numero == numeroCompte) {
            compteTrouve = 1; // Marquer le compte comme trouvé

            printf("Transactions du compte %d (%s) :\n", compte.numero, compte.nom);

            // Exemple : afficher des transactions (vous devrez adapter cette partie)
            int choixTransaction;
            do {
                // Menu des transactions
                printf("\nMenu Transactions:\n");
                printf("1. Ajout\n");
                printf("2. Retrait\n");
                printf("0. Quitter\n");
                printf("Choisissez une option : ");
                scanf("%d", &choixTransaction);

                switch (choixTransaction) {
                    case 1:
                        ///////////////////////////// Le cas d'un ajout d'argent////////////////////////////////////
                        {
                            float montantDepot;
                            printf("Entrez le montant ajouté : ");
                            scanf("%f", &montantDepot);

                            if (montantDepot > 0) {
                                compte.solde += montantDepot;
                                printf("Dépôt de %.2f effectué avec succès. Nouveau solde : %.2f\n", montantDepot, compte.solde);
                            } else {
                                printf("Montant invalide pour l'ajout.\n");
                            }
                        }
                        break;
                    case 2:
                        ///////////////////////////////////////// Le cas d'un retrait d'argent////////////////////////////////
                        {
                            float montantRetrait;
                            printf("Entrez le montant du retrait : ");
                            scanf("%f", &montantRetrait);

                            if (montantRetrait > 0 && montantRetrait <= compte.solde) {
                                compte.solde -= montantRetrait;
                                printf("Retrait de %.2f effectué avec succès. Nouveau solde : %.2f\n", montantRetrait, compte.solde);
                            } else {
                                printf("Montant invalide pour le retrait ou solde insuffisant.\n");
                            }
                        }
                        break;
                    case 0:
                        printf("Retour au menu principal.\n");
                        break;
                    default:
                        printf("Option invalide. Veuillez choisir à nouveau.\n");
                }

            } while (choixTransaction != 0);

            // Écrire le compte mis à jour dans le fichier temporaire
            fprintf(tempFichier, "%d %s %.2f\n", compte.numero, compte.nom, compte.solde);
        } else {
            // Écrire le compte dans le fichier temporaire sans le modifier
            fprintf(tempFichier, "%d %s %.2f\n", compte.numero, compte.nom, compte.solde);
        }
    }

    fclose(fichier);
    fclose(tempFichier);

    if (!compteTrouve) {
        printf("Le compte %d n'a pas été trouvé.\n", numeroCompte);
        remove("tempdonnees.txt"); // Supprimer le fichier temporaire si le compte n'a pas été trouvé
    } else {
        // Remplacer l'ancien fichier par le fichier temporaire
        remove("donnees.txt");
        rename("tempdonnees.txt", "donnees.txt");
    }
}

///////////affichier le montant d'un compte à partir d'un num///////////////////////////////////////////
void afficherDetailsCompte(int numeroCompte) {
    FILE *fichier = fopen("donnees.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    struct Compte compte;
    int compteTrouve = 0;

    // Parcourir le fichier jusqu'au compte avec le numéro spécifié
    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) != EOF) {
        if (compte.numero == numeroCompte) {
            compteTrouve = 1; // Marquer le compte comme trouvé

            // Afficher les détails du compte
            printf("Détails du compte %d (%s) :\n", compte.numero, compte.nom);
            printf("Solde : %.2f\n", compte.solde);

            break;
        }
    }

    fclose(fichier);

    if (!compteTrouve) {
        printf("Le compte %d n'a pas été trouvé.\n", numeroCompte);
    }
}

void supprimerCompte(int numeroCompte) {
    FILE *fichier = fopen("donnees.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    FILE *tempFichier = fopen("tempdonnees.txt", "w");
    if (tempFichier == NULL) {
        printf("Erreur lors de la création du fichier temporaire.\n");
        fclose(fichier);
        return;
    }

    struct Compte compte;
    int compteTrouve = 0;

    // Parcourir le fichier jusqu'au compte avec le numéro 
    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) != EOF) {
        if (compte.numero == numeroCompte) {
            compteTrouve = 1; // Marquer le compte comme trouvé

            // Demander confirmation à l'utilisateur
            char confirmation;
            printf("Voulez-vous vraiment supprimer le compte %d (%s) ? (O/N): ", compte.numero, compte.nom);
            scanf(" %c", &confirmation);
            if (confirmation == 'O' || confirmation == 'o') {
                continue; // Ne pas écrire le compte dans le fichier temporaire
            }
        }

        // Écrire le compte dans le fichier temporaire
        fprintf(tempFichier, "%d %s %.2f\n", compte.numero, compte.nom, compte.solde);
    }

    fclose(fichier);
    fclose(tempFichier);

    if (!compteTrouve) {
        printf("Le compte %d n'a pas été trouvé.\n", numeroCompte);
        remove("tempdonnees.txt"); // Supprimer le fichier temporaire si le compte n'a pas été trouvé
        return;
    }

    // Remplacer l'ancien fichier par le fichier temporaire
    remove("donnees.txt");
    rename("tempdonnees.txt", "donnees.txt");

    printf("Le compte %d a été supprimé avec succès.\n", numeroCompte);
}
///////////////////////////////////////////affichier tous les comptes/////////////////////////////
void afficherListeClients() {
    FILE *fichier = fopen("donnees.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    struct Compte compte;

    printf("Liste des clients :\n");

    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) == 3) {
        // Afficher les détails du compte
        printf("Numéro de compte : %d\n", compte.numero);
        printf("Nom du client : %s\n", compte.nom);
        printf("Solde : %.2f\n", compte.solde);
        printf("\n");
    }

    fclose(fichier);
}

int main() {
    int choix;
    int numeroCompte;

    do {
        printf("\nMenu:\n");
        printf("1. Créer un compte\n");
        printf("2. Mettre à jour un compte\n");
        printf("3. Afficher les détails d'un compte\n");
        printf("4. Gérer les transactions d'un compte\n");
        printf("5. Supprimer un compte\n");
        printf("6. Afficher la liste des clients\n");
        printf("0. Quitter\n");
        printf("Choisissez une option : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                creerCompte();
                break;
            case 2:
                printf("Entrez le numéro de compte à mettre à jour : ");
                scanf("%d", &numeroCompte);
                mettreAJourCompte(numeroCompte);
                break;
            case 3:
                printf("Entrez le numéro de compte à afficher : ");
                scanf("%d", &numeroCompte);
                afficherDetailsCompte(numeroCompte);
                break;
            case 4:
                printf("Entrez le numéro de compte pour gérer les transactions : ");
                scanf("%d", &numeroCompte);
                gererTransactions(numeroCompte);
                break;
            case 5:
                printf("Entrez le numéro de compte à supprimer : ");
                scanf("%d", &numeroCompte);
                supprimerCompte(numeroCompte);
                break;
            case 6:
                afficherListeClients();
                break;
            case 0:
                printf("Programme terminé.\n");
                break;
            default:
                printf("Option invalide. Veuillez choisir à nouveau.\n");
        }
    } while (choix != 0);

    return 0;
}
