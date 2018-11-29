#include <stdio.h>

void main( )	
{
	int	i, nb_client;

	/* Partie 1 : Définition des variables structures */
	struct	date
	{
		int	jour;
		int	mois;
		int	annee;
	};

	struct	compte
	{
		char 		nom[80];
		char		rue[80];
		char		ville[80];
		int		no_compte;
		char		etat_compte;
		float		versement;
		struct	date 	der_versement;

};

struct 	compte		client[100];

/* Partie 2 : Saisie du nombre de clients à traiter */
printf("Combien de clients souhaitez-vous traiter : ");
scanf("%d",&nb_client);

/* Saisie des informations concernant chaque client */
for( i=0; i<nb_client; i++ )
{
	printf("Traitement du client n°%d",i+1);
	printf("Entrer le nom =");
scanf("%s",client[i].nom);
	printf("Entrer la rue =");
scanf("%s",client[i].rue);
	printf("Entrer la ville=");
scanf("%s",client[i].ville);
	printf("Entrer le numéro de compte =");
scanf("%d",&client[i].no_compte);
	
printf("Entrer l’état du compte =");
scanf("%c",&client[i].etat_compte);
	printf("Entrer le montant du versement =");
scanf("%f",&client[i].versement);
	printf("Entrer la date du versement sous le format jj/m/aaaa=");
scanf("%d/%d/%d",	&client[i].der_versement.jour    , 
&client[i].der_versement.mois   ,
&client[i].der_versement.annee );
}

	/* Partie 3 :  Affichage des informations concernant chaque client */
for( i=0; i<nb_client; i++)
{
		printf("Client n°		: %d\n", i);
	printf("Nom			: %s\n", client[i].nom);
	printf("Rue			: %s\n", client[i].rue);
	printf("Ville			: %s\n", client[i].ville);
		printf("Compte n°		: %d\n", client[i].no_compte);
		printf("Etat du compte n°	: %c\n", client[i].etat_compte);
		printf("Versement		: %f\n", client[i].versement);
		printf("Date de versement	: %d/%d/%d\n", client[i].der_versement.jour, 
   client[i].der_versement.mois,	   client[i].der_versement.annee );
		getchar();
}

} /* end main */
