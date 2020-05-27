#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

double alphaInit = 0.9;

struct vecteur
    {  
       double *v;
       double norme;
       char *name;

    };

struct neuronne
{
	double *vN;
 	char *nameN;
};




/******************************************Read fichier dans structure**************************************************/
void read(struct vecteur *tableau)
{
    FILE * iris;
    iris=fopen("iris.data","r");
    ssize_t GetLine;
    size_t longeur=0;
    char *string;
    int a;
	int j;
   char *token;
   char *TypeIris;
   double Double;
 
   for(a=0;a<150;a++)
   {  
        GetLine = getline(&string,&longeur,iris);
        if(GetLine !=(-1))
        {
		        
			    tableau[a].v= malloc(4*sizeof(double));
			    j=0;

		        token=strtok(string,",");
		   	
		        while(token !=NULL)
				{
					Double = strtod(token,&TypeIris);
					if(Double != 0.0)
					{
						tableau[a].v[j]=Double;
						
						j++;
					}else{
							if(TypeIris != NULL )
							{
								tableau[a].name = strdup(TypeIris);
							      
							}
					     }
					token= strtok(NULL,",");
				
					
			    } 

		}
}

    fclose(iris);
}

//********************************* calcule de la norme***************************************
 void norme_vecteur(struct vecteur *tableau,int n)

{
    
    int i,j;
    double sum;
   
    for(i=0;i<n;i++)
    {
		sum=0;
	    	for(j=0;j<4;j++)
	    	{

	    		// metre au carré
	    		sum=sum + pow(tableau[i].v[j],2);
	    		

	    	}

			tableau[i].norme=sqrt(sum);

	    	 for (j=0;j<4;j++)
	    	 {
	    	 	
	    	 	tableau[i].v[j]=tableau[i].v[j]/tableau[i].norme;

	    	 }
    	 	

   	 }	
			
}
void afficherStructureNormalise(struct vecteur *tableau,int longeurTab, int nbcolone)
{ int a,j;


for(a=0;a<longeurTab;a++)
   {  
            
        for(j=0;j<nbcolone;j++)
           {
             printf("%f\t", tableau[a].v[j]);
    
            } 
                
            printf(" %s\n", tableau[a].name);


        
   }


}

//***************************************calcul de la moyenne*******************************

double* vecteur_moyen(struct vecteur *tableau,int longeurTab)
{int i,j;

	double *VectMoyen = malloc(4*sizeof(double));
	 
    for(i=0;i<4;i++)
    {
        for(j=0;j<longeurTab;j++)
           { 
             VectMoyen[i]+=tableau[j].v[i];
	   }
	   
           VectMoyen[i]/=longeurTab;
       	           
    }    
	return VectMoyen ;
}


void afficherVectMoyen(double *v,int longeurVect)
{
	int i;
	printf("\n le vecteur moyen est \n ");
	for(i=0;i<longeurVect;i++)
	{
		printf(" %f , ", v[i]);
	}
	printf("\n");
}


/*********************************création de la matrice des Neurones******************************/



struct neuronne** initialiserMatriceNeuronne(int NbLigne,int NbCol,double *Vecteur_moyen)
{
	 srand(time(NULL));
	int i,j,k;
	double Max,Min;
	struct neuronne **MatriceNeuronne = malloc(NbLigne*sizeof(struct neuronne*));
	for(i=0;i<NbLigne;i++)
	{
		MatriceNeuronne[i]=malloc(NbCol*sizeof(struct neuronne));
		for(j=0;j<NbCol;j++)
		{	
			MatriceNeuronne[i][j].vN = malloc(4*sizeof(double));
			MatriceNeuronne[i][j].nameN =NULL;
			for(k=0;k<4;k++)
			{	
				
				Max = Vecteur_moyen[k] + 0.05;
				Min = Vecteur_moyen[k] - 0.05;				
				MatriceNeuronne[i][j].vN[k] = (double)rand()/RAND_MAX*(Max-Min)+Min;
			}

		}
	
	}

	return MatriceNeuronne;

	
}






void afficherMatriceNeuronne(struct neuronne **MatriceNeuronne,int NbLigne,int NbCol)
{
	int i,j,k;
        printf("\n la matrice des neuronnes \n ");
	for(i=0;i<NbLigne;i++)
	{
		for(j=0;j<NbCol;j++)
		{
			for(k=0;k<4;k++)
			{
				printf(" %f , ",MatriceNeuronne[i][j].vN[k]);
			}
			printf(" %s \n",MatriceNeuronne[i][j].nameN ); 

		}
	
	}

	
}

/******************************************Apprentissage**************************************************/

double distanceEucledienne(double *v1,double *v2,int longeurV)
{
	int i;
	double sum =0;
	for(i=0;i<longeurV;i++)
	{
		sum =sum+pow((v1[i]-v2[i]),2);
	}
	return sqrt(sum);
}

int* BestMatchUnit(struct neuronne **N, double *v, int NbLigne, int NbCol, int longeurV)
{
	int i,j;
	double distance ;
	double DistanceMin =distanceEucledienne(v,N[0][0].vN,longeurV) ;
	int *BMU=malloc(2*sizeof(int));

	for(i=0;i<NbLigne;i++)
	{
		for(j=0;j<NbCol;j++)
		{
			distance = distanceEucledienne(v,N[i][j].vN,longeurV) ;
			if(distance < DistanceMin)
			{	
				DistanceMin = distance;
				BMU[0]= i;
				BMU[1]=j;
			}
		}
	}
	return BMU;

}

void shuffle(int tab[],int longeurTab)
{
	int temporaire;
	int i ;
	int Rand;
	srand(time(NULL));
	for(i=0;i<longeurTab;i++)
	{	Rand = rand()%longeurTab;
		temporaire=tab[Rand];
		tab[Rand]=tab[i];
		tab[i]=temporaire;
	}
}

void RegleApprentissage(double *vN, double *vD,int longeurV,double alpha)
{
	int i;
	for(i=0;i<longeurV;i++)
	{
		vN[i] = vN[i]+alpha*(vD[i]-vN[i]);
	}
}


void Apprentissage_phase1(double alphaInit, struct neuronne **N, struct vecteur *tableau,int NbLigne,int NbCol,int longeurTab,int nb_iteration,int rayon)
{
	int tab[longeurTab];
	for(int i=0;i<longeurTab;i++)
	{ tab[i]=i;}
	
	int *BMU=malloc(2*sizeof(int));
       
	int l,i,c,j;
        
	double alpha;
	
        int t;
     
       t=nb_iteration/rayon;
        
	for(i=0;i<nb_iteration;i++)
	{
		alpha=alphaInit*(1-(i/nb_iteration));
		shuffle(tab,longeurTab);
        
                 if((i && rayon >1) && ((i%t)==0))
                  {rayon--;}
                  
		for(j=0;j<longeurTab;j++)
		{
			BMU=BestMatchUnit(N,tableau[tab[j]].v,NbLigne,NbCol,4);
			
		        for(l=BMU[0]-rayon;l<=BMU[0]+rayon;l++)
			{
				for(c = BMU[1]-rayon;c<=BMU[1]+rayon;c++)
				{
					if( (l>=0)&&(l<NbLigne) && (c>=0)&&(c<NbCol) )
					
					{
                                                
						RegleApprentissage(N[l][c].vN, tableau[tab[j]].v,4,alpha);
					}
				}
			}
                         
		}


	} 

	
      
  
        
}

 void  Apprentissage_phase2(double alphaInit, struct neuronne **N, struct vecteur *tableau,int NbLigne,int NbCol,int longeurTab)
{
       
       alphaInit = alphaInit/10;
       Apprentissage_phase1(alphaInit,N,tableau,10,6,longeurTab,1500,1);
}   
/******************************************labeling**************************************************/
void labeling(struct vecteur *tableau,struct neuronne **N,int NbLigne, int NbCol,int longeurTab)
{
	double distEuc;
	int l,c;
	int i,posBMU;
	double distMin;

	for(l=0;l<NbLigne;l++)
	{
		for(c=0;c<NbCol;c++)
		{
			distMin= distanceEucledienne(N[l][c].vN,tableau[0].v,4);
        	       
   			
			for(i=0;i<longeurTab;i++)
			{

				distEuc = distanceEucledienne(N[l][c].vN,tableau[i].v,4);
				
				if(distEuc < distMin)
				{
					distMin= distEuc;
					posBMU = i;
					


				}

				
				
			}
        	
      			N[l][c].nameN = strdup(tableau[posBMU].name);	
      			


		}
	}
}



void matrice(struct neuronne **N,int NbLigne,int NbCol)
{
    int l;
    int c;
    int result;					

    for(l=0;l<NbLigne;l++)
    {      	

        for(c=0;c<NbCol;c++)
        {  
			

            result = strncmp(N[l][c].nameN,"Iris-virginica",14);
            if( result == 0 )
            {	
            	
            	printf("\033[35m A\033[00m");
               

            }
            result = strncmp(N[l][c].nameN,"Iris-versicolor",15);
            if( result == 0 )
            {	
            	
            	printf("\033[36m B\033[00m");
               

            }
            result = strncmp(N[l][c].nameN,"Iris-setosa",11);
            if( result == 0 )
            {	
            	
            	printf("\033[32m C\033[00m");
               

            }

            
        }
        printf("\n");    

    }

		
}

/**********Main************/


int main()
{	
     srand(time(NULL));
     struct vecteur *tableau=malloc(150*sizeof(struct vecteur));
     read(tableau);
     norme_vecteur(tableau,150); 
   //afficherStructureNormalise(tableau,150,4);
     double * Vecteur_moyen = vecteur_moyen(tableau,150);
   //afficherVectMoyen(Vecteur_moyen,4);

      struct neuronne **MatriceNeuronne = initialiserMatriceNeuronne(10,6,Vecteur_moyen);
   //afficherMatriceNeuronne(MatriceNeuronne,10,6);

      Apprentissage_phase1(alphaInit,MatriceNeuronne,tableau,10,6,149,500,3);
  
      Apprentissage_phase2(alphaInit,MatriceNeuronne,tableau,10,6,149);
      labeling(tableau,MatriceNeuronne,10,6,150);

    //afficherMatriceNeuronne(MatriceNeuronne,10,6);

     matrice(MatriceNeuronne,10,6);


  

    return 0;
}
