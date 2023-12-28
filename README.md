
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Compte {
    int numero;
    char nom[50];
    float solde;
};
///////////creer un nv compte en lui donnant un numéro////////////////////
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

    FILE *nouveauFichier = fopen("newdonnees.txt", "w");
    if (nouveauFichier == NULL) {
        printf("Erreur lors de la création du nouveau fichier.\n");
        fclose(fichier);
        return;
    }

    // Lire les comptes du fichier existant et les copier dans le nouveau fichier
    struct Compte compte;
    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) != EOF) {
        fprintf(nouveauFichier, "%d %s %.2f\n", compte.numero, compte.nom, compte.solde);
    }

    // Ajouter le nouveau compte dans le fichier
    nouveauCompte.numero = compte.numero + 1;
    fprintf(nouveauFichier, "%d %s %.2f\n", nouveauCompte.numero, nouveauCompte.nom, nouveauCompte.solde);

    fclose(fichier);
    fclose(nouveauFichier);

    // Supprimer l'ancien fichier
    remove("donnees.txt");
    // Renommer le nouveau fichier
    rename("newdonnees.txt", "donnees.txt");

    printf("Le compte a été créé avec succès. Numéro de compte : %d\n", nouveauCompte.numero);
}
///////////////////////mettre à jour un compte en lui donnant le numéro d'un compte à modifier et le nouveau montant/////////////////////////////
void mettreAJourCompte(int numeroCompte) {
    FILE *fichier = fopen("donnees.txt", "r+");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    struct Compte compte;
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
            fprintf(fichier, "%d %s %.2f", compte.numero, compte.nom, compte.solde);
            break;
        }
    }

    fclose(fichier);
}
///////////////////////changer les param de compte : un retrait ou un ajout d,argent//////////////////////////////////////////
void gererTransactions(int numeroCompte) {
    FILE *fichier = fopen("donnees.txt", "r+");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    struct Compte compte;
    int compteTrouve = 0;

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
                        /////////////////////////le cas d'un ajout d'arg ++++++///////////////////////////////////////
                        {
                            float montantDepot;
                            printf("Entrez le montant du dépôt : ");
                            scanf("%f", &montantDepot);

                            if (montantDepot > 0) {
                                compte.solde += montantDepot;
                                printf("Dépôt de %.2f effectué avec succès. Nouveau solde : %.2f\n", montantDepot, compte.solde);
                            } else {
                                printf("Montant invalide pour le dépôt.\n");
                            }
                        }
                        break;
                    case 2:
                        ////////////////////////////le cas d'un retrait d'arg---------------//////////////////////////////////////////
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

            // Remettre à jour le solde dans le fichier
            fseek(fichier, -((long int)sizeof(struct Compte)), SEEK_CUR);

            fprintf(fichier, "%d %s %.2f", compte.numero, compte.nom, compte.solde);
            break;
        }
    }

    fclose(fichier);

    if (!compteTrouve) {
        printf("Le compte %d n'a pas été trouvé.\n", numeroCompte);
    }
}

void supprimerCompte(int numeroCompte) {
    FILE *fichier = fopen("donnees.txt", "r+");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    FILE *nouveauFichier = fopen("newdonnees.txt", "w");
    if (nouveauFichier == NULL) {
        printf("Erreur lors de la création du nouveau fichier.\n");
        fclose(fichier);
        return;
    }

    struct Compte compte;
    int compteTrouve = 0;

    // Lire les comptes 
    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) != EOF) {
        if (compte.numero == numeroCompte) {
            // Demander confirmation à l'utilisateur
            char confirmation;
            printf("Voulez-vous vraiment supprimer le compte %d (%s) ? (O/N): ", compte.numero, compte.nom);
            scanf(" %c", &confirmation);
            if (confirmation == 'O' || confirmation == 'o') {
                compteTrouve = 1; 
                continue; 
            }
        }
        // Écrire le compte dans le nouveau fichier
        fprintf(nouveauFichier, "%d %s %.2f\n", compte.numero, compte.nom, compte.solde);
    }

    fclose(fichier);
    fclose(nouveauFichier);

    if (!compteTrouve) {
        printf("Le compte %d n'a pas été trouvé.\n", numeroCompte);
        remove("newdonnees.txt"); // Supprimer le nouveau fichier si je nensuis pas besoin de l'utiliserrr
        return;
    }

    // Supprimer l'ancien fichier
    remove("donnees.txt");
    // Renommer le nouveau fichier
    rename("newdonnees.txt", "donnees.txt");

    printf("Le compte %d a été supprimé avec succès.\n", numeroCompte);
}

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

            // Vous pouvez ajouter ici d'autres informations sur le compte

            break;
        }
    }

    fclose(fichier);

    if (!compteTrouve) {
        printf("Le compte %d n'a pas été trouvé.\n", numeroCompte);
    }
}
///////////////////////////////////////////////afficher tous les comptes ///////////////////////////////////
void afficherListeClients() {
    FILE *fichier = fopen("donnees.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    struct Compte compte;

    printf("Liste des clients :\n");

    // parcourir tous les comptes du fichier 
    while (fscanf(fichier, "%d %s %f", &compte.numero, compte.nom, &compte.solde) != EOF) {
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
