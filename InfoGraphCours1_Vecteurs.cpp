#include "InfoGraphCours1_Vecteurs.h"
//#include "math.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <random>

std::default_random_engine engine;
std::uniform_real_distribution<double> u(0,1);
std::uniform_real_distribution<double> u2(0,1./4.);


Vecteur::Vecteur(double x, double y, double z)
{
	X=x;
	Y=y;
	Z=z;
}

void Vecteur::set(double x, double y, double z)
{
	X=x;
	Y=y;
	Z=z;
}

double Vecteur::GetX() const
{
	return X;
}

double Vecteur::GetY() const
{
	return Y;
}

double Vecteur::GetZ() const
{
	return Z;
}

void Vecteur::SetX(double x)
{
	X = x;
}

void Vecteur::SetY(double y)
{
    Y = y;
}

void Vecteur::SetZ(double z)
{
	Z = z;
}

double Vecteur::GetNorme() const
{
    double norme,quantiteIntermediaire;
    quantiteIntermediaire = pow(X,2)+pow(Y,2)+pow(Z,2);
    norme = sqrt(quantiteIntermediaire);
	return norme;
}
void Vecteur::operator+=(const Vecteur V)
{
    double addx,addy,addz;
    addx = V.GetX();
    addy = V.GetY();
    addz = V.GetZ();
	
    X = X + addx;
    Y = Y + addy;
    Z = Z + addz;
}

Vecteur Vecteur::operator*(double w){
    Vecteur newVect;
    newVect.set(X*w,Y*w,Z*w);
    return newVect;
}

void Vecteur::operator*=(double w)
{
    X = w*X;
    Y = w*Y;
    Z = w*Z;
}

Vecteur Vecteur::operator*(const Vecteur V)
{
    double Xprime,Yprime,Zprime,newX,newY,newZ;
	Xprime = V.GetX();
	Yprime = V.GetY();
	Zprime = V.GetZ();
	
	newX = Y*Zprime - Z*Yprime;
	newY = Z*Xprime - X*Zprime;
	newZ = X*Yprime - Y*Xprime;
	
    return(Vecteur(newX,newY,newZ));
}

void Vecteur::operator=(const Vecteur V)
{
	X = V.GetX();
	Y = V.GetY();
	Z = V.GetZ();
}


Sphere::Sphere(const Vecteur o, double r, double Rau_R, double Rau_G, double Rau_B, bool spec)
{
	O = o;
	R = r;
    rau_R = Rau_R;
    rau_G = Rau_G;
    rau_B = Rau_B;
    specularite = spec;
}

void Sphere::set(const Vecteur o, double r, double Rau_R, double Rau_G, double Rau_B)
{
    double newX,newY,newZ;
	R = r;
	newX = o.GetX();
	newY = o.GetY();
	newZ = o.GetZ();
	
	O.SetX(newX);
	O.SetY(newY);
	O.SetZ(newZ);

    rau_R = Rau_R;
    rau_G = Rau_G;
    rau_B = Rau_B;
}
double Sphere::GetOX() const
{
    return O.GetX();
}
double Sphere::GetOY() const
{
    return O.GetY();
}
double Sphere::GetOZ() const
{
    return O.GetZ();
}
bool Sphere::intersection(const Vecteur C, const Vecteur u)
{
    double t;
    t = racines(C,u);

    if (t>0)
	{
		return true;
	}
	else
	{
		return false;
	}		
		
}
double Sphere::racines(const Vecteur C, const Vecteur u)
{
    double Xco,Yco,Zco,a,b,c,discriminant,t,t1,t2;
    /* on resoud ax**2+bx+c */


    // norme de u au carré :
    a = (u.GetX()*u.GetX() + u.GetY()*u.GetY() + u.GetZ()*u.GetZ());


    /* calcul des coordonnes du membre de droite du produit scalaire */
    Xco = C.GetX()-O.GetX();
    Yco = C.GetY()-O.GetY();
    Zco = C.GetZ()-O.GetZ();


    b = 2*(u.GetX()*Xco + u.GetY()*Yco + u.GetZ()*Zco);
    c = (Xco*Xco + Yco*Yco + Zco*Zco) - R*R;



    discriminant = b*b-4*a*c;
    if (discriminant == 0)
    {
        t = -b/(2*a);
    }
    else if(discriminant > 0)
    {
        t1 = (-b+sqrt(discriminant))/(2*a);
        t2 = (-b-sqrt(discriminant))/(2*a);

        if (max(t1,t2)>0.)
        {
            t = min(max(0.,t1),max(0.,t2));
        }
        else{
            t=-1.;
        }
    }
    else
    {
        t=-1.;
    }
    return t;
}

double Sphere::ombrage(const Vecteur C, const Vecteur u, const Vecteur L, double I)
{
    Vecteur P,N,l,Ubis;
    double t = racines(C,u);
    double normeDenominateur,produitscalaire,Xl,Yl,Zl;

    // On calcul des coordonnes de P
    Ubis = u;
    Ubis*= t;

    P += Ubis;
    P += C;



    // l est le vecteur qui amene du point P a la source de lumiere L
    Xl = L.GetX()-P.GetX();
    Yl = L.GetY()-P.GetY();
    Zl = L.GetZ()-P.GetZ();
    l.set(Xl,Yl,Zl);

    normeDenominateur = l.GetNorme();


    // On calcul la normale a la surface de la sphere et on rend unitaires N et l
    N.set(P.GetX()-O.GetX(),P.GetY()-O.GetY(),P.GetZ()-O.GetZ());

    double coeff1 = 1/N.GetNorme();
    double coeff2 = 1/normeDenominateur;
    N*=coeff1;
    l*=coeff2;

    // calcul du produit scalaire entre N et l
    produitscalaire = N.GetX()*l.GetX() + N.GetY()*l.GetY() + N.GetZ()*l.GetZ();

    if (produitscalaire>0)
    {

        return (produitscalaire*I)/(3.14*normeDenominateur*normeDenominateur);
    }
    else
    {
        return 0;
    }
}

double Sphere::GetRauR()
{
    return rau_R;
}

double Sphere::GetRauG()
{
    return rau_G;
}

double Sphere::GetRauB()
{
    return rau_B;
}
bool Sphere::GetSpec() const{
    return specularite;
}
Scene::Scene()
{

}

int Scene::NbElem()
{
    return ListeSphere.size();
}

void Scene::AddSphere(const Sphere S)
{
    ListeSphere.push_back(S);
}

Sphere Scene::GetIndex(int i)
{
    return ListeSphere[i];
}

bool Scene::Eclairage(const Vecteur C, const Vecteur u, const Vecteur L,int index)
{
    /* Le but de cette fonction est d'indiquer si oui ou non le pixel, pour lequel une intersection avec un rayon
    a été trouvée, doit être éclairée car il est touchée par la lumière de la source L*/
    Vecteur P,l,Ubis,Normale;


    // On va chercher la racine permettant de trouver le point d'intersection
    double t = ListeSphere[index].racines(C,u);
    double Xl,Yl,Zl,d;

    // On calcul des coordonnes de P le point d'intersection de U et la sphere
    Ubis = u;
    Ubis*=t;
    P += Ubis;
    P += C;

    // On calcul la normale
    Normale.set(P.GetX()-ListeSphere[index].GetOX(),P.GetY()-ListeSphere[index].GetOY(),P.GetZ()-ListeSphere[index].GetOZ());
    Normale*=(1/Normale.GetNorme());

    // l est le vecteur qui amene du point P a la source de lumiere L
    Xl = L.GetX()-P.GetX();
    Yl = L.GetY()-P.GetY();
    Zl = L.GetZ()-P.GetZ();
    l.set(Xl,Yl,Zl);
    // normalisation de l
    double NormeL = l.GetNorme();
    l*=(1/NormeL);

    // Maintenant qu'on a P le point à sonder et l le vecteur normé le reliant à la source lumineuse
    // On va parcourir toutes les sphères de la scène et regarder si ce chemin (entre P et L)
    // intersecte une autre sphère de la scène ou non.
   bool flag=true;
   double epsilon = 0.01;
   // On se place à epsilon * normale plus loin que le point
   Normale *= epsilon;
   P += Normale;

    for(int i=0;i<ListeSphere.size();i++)
    {
            if (ListeSphere[i].intersection(P,l))
            {
                d = ListeSphere[i].racines(P,l)*l.GetNorme();
                if (d<NormeL)
                {
                  flag = false;
                }
            }


    }

return flag;
}

Vecteur Scene::Specularite(Vecteur AncienP, Vecteur direction, int index, int rebonds)
{
    Vecteur Normale,directionbis;
    double t;
    double current_t,newt;
    int prochain_index;

    // on initialise le current_t, il aura une utilité plus tard
    current_t = 10000;
    // on initialise prochain_index, il aura une utilié plus tard
    prochain_index = ListeSphere.size()+1;

    // on calcul le point de la sphère spéculaire d'où il faut partir
    // et la direction dans laquelle le rayon est réfléchit
    if(ListeSphere[index].intersection(AncienP,direction)){
    t = ListeSphere[index].racines(AncienP,direction);
    directionbis = direction;
    directionbis*=t;
    AncienP += directionbis;
    Normale.set(AncienP.GetX()-ListeSphere[index].GetOX(),AncienP.GetY()-ListeSphere[index].GetOY(),AncienP.GetZ()-ListeSphere[index].GetOZ());
    Normale*=(direction.GetX()*Normale.GetX()+direction.GetY()*Normale.GetY()+direction.GetZ()*Normale.GetZ());
    Normale*=(-2);
    direction += Normale;
    }

    // on parcourt toutes les sphères
    for(int i=0;i<ListeSphere.size();i++){

        // on ne fait rien si la sphère parcourue est celle d'où provient le rayon
        if (i==index){
            // rien
        }
        // sinon
        else{

            // on vérifie que le rayon réfléchit intersecte une autre sphère
            if((ListeSphere[i].intersection(AncienP,direction)) & (rebonds<5))
            {
                // dans ce cas il faut savoir si la sphère parcourue est la plus proche
                // de la sphère source, ou si une autre est devant
                newt = ListeSphere[i].racines(AncienP,direction);

                if (newt<current_t){
                        // si on a trouvé un chemin plus court que le précédent
                        // alors on le garde en mémoire
                        current_t = newt;

                        // et on note l'index de la prochaine sphère où se réfléchira le rayon
                        prochain_index = i;
                }
            }

        }
    }

    // après avoir parcouru toutes les sphères
    // si on a trouvé une sphère sur le chemin de notre rayon
    if (prochain_index<ListeSphere.size()){

        // si la sphère touchée est spéculaire
        if(ListeSphere[prochain_index].GetSpec() && (rebonds<5)){
            // Alors on va appeler la fonction Specularite sur cette sphère
            cout<<"j'ai rebondi \t";
            Specularite(AncienP,direction,prochain_index,rebonds++);

        }
        // si la sphère n'est pas spéculaire
        if(ListeSphere[prochain_index].GetSpec()==false){

            // on renvoie la couleur de la sphère sur laquelle le rayon va se réfléchir
            Vecteur couleur(ListeSphere[prochain_index].GetRauR(),ListeSphere[prochain_index].GetRauG(),ListeSphere[prochain_index].GetRauB());
            return couleur;
        }
    }

    // si on arrive au maximum de rebonds
    else if(rebonds>=5){
        // on renvoie la couleur de la sphère actuelle
        Vecteur couleur(ListeSphere[index].GetRauR(),ListeSphere[index].GetRauG(),ListeSphere[index].GetRauB());
        return couleur;
    }
    // si on a pas trouvé de sphère sur laquelle le rayon se réfléchit
    else if(prochain_index>=ListeSphere.size()){
        // on renvoie un pixek noir
        Vecteur couleur(0,0,0);
        return couleur;
    }
}

Vecteur Scene::BRDF(Vecteur AncienP, Vecteur direction, Vecteur L, int I,int index, int rebonds){

    double add_x,add_y,r3,r4;
    Vecteur tempDir;
    tempDir = direction;
    r3 = u2(engine);
    r4 = u2(engine);
    add_x = cos(2*3.14*r3)*sqrt(1-r4);
    add_y = sin(2*3.14*r3)*sqrt(1-r4);
    tempDir.SetX(direction.GetX()+add_x);
    tempDir.SetY(direction.GetY()+add_y);


    // définition des variables
    Vecteur directionbis,Normale,l;
    double current_t,newt,r1,r2,x,y,z;

    // on initialise prochain_index à 30 pour détecter s'il n'y a pas eu d'intersection
    int prochain_index=30;

    // on initialise le current_t, il aura une utilité plus tard
    current_t = 10000;

    // on parcourt toutes les sphères
    for(int i=0;i<ListeSphere.size();i++){   
            // on vérifie que le rayon réfléchit intersecte une autre sphère
            if(ListeSphere[i].intersection(AncienP,tempDir))
            {
                // dans ce cas il faut savoir si la sphère parcourue est la plus proche
                // de la sphère source, ou si une autre est devant
                newt = ListeSphere[i].racines(AncienP,tempDir);

                if (newt<current_t){
                        // si on a trouvé un chemin plus court que le précédent
                        // alors on le garde en mémoire
                        current_t = newt;

                        // et on note l'index de la prochaine sphère où se réfléchira le rayon
                        prochain_index = i;
                }
            }

        }

    if ((prochain_index<30) && (rebonds<5)) // si on a trouvé une intersection
    {
        // Cette partie fournie des textures définies
        /*Vecteur couleur;
        if (ListeSphere[index].GetSpec()){couleur=Specularite(AncienP,tempDir,index,0);}
        //else{couleur.set(ListeSphere[index].GetRauR(),ListeSphere[index].GetRauG(),ListeSphere[index].GetRauB());}
        */

    // on calcul le point sur lequel on arrive avec la formule P = C +U*t
    directionbis = tempDir;
    directionbis*=current_t; // U*t
    AncienP += directionbis; // C + U*t

    // Cette ligne fournie des textures fonctionnelles
    Vecteur couleur(abs(sin(AncienP.GetX()/5)),abs(AncienP.GetZ()/60),abs(sin(AncienP.GetY()/5)));

    // on calcul la normale à la sphère au nouveau point
    Normale.set(AncienP.GetX()-ListeSphere[prochain_index].GetOX(),AncienP.GetY()-ListeSphere[prochain_index].GetOY(),AncienP.GetZ()-ListeSphere[prochain_index].GetOZ());
    // on la normalise
    Normale*=1/(Normale.GetNorme());

    // on va décaler le prochain point de départ de epsilon*normale pour ne pas avoir de bruit



    // on détermine l'éclairage direct

    double Xl,Yl,Zl,normeDenominateur,produitscalaire,coeff;

    // l est le vecteur qui amene du point P a la source de lumiere L
    Xl = L.GetX()-AncienP.GetX();
    Yl = L.GetY()-AncienP.GetY();
    Zl = L.GetZ()-AncienP.GetZ();
    l.set(Xl,Yl,Zl);
    normeDenominateur = l.GetNorme();
    l*=(1/l.GetNorme());

    // on utilise cette variable 'eclaire' pour savoir si le point est eclaire ou s'il y a un obstacle
    // entre lui et la lumiere
    bool eclaire = true;
    double d;
    for(int i=0;i<ListeSphere.size();i++)
    {
            if (ListeSphere[i].intersection(AncienP,l))
            {
                d = ListeSphere[i].racines(AncienP,l);
                if (d<normeDenominateur)
                {
                  eclaire = false;
                }
            }


    }
    // Eclairage direct uniquement si le point est eclaire
    if(eclaire){
    // calcul du produit scalaire entre N et l
    produitscalaire = Normale.GetX()*l.GetX() + Normale.GetY()*l.GetY() + Normale.GetZ()*l.GetZ();
    if(produitscalaire>0){coeff=(produitscalaire*I)/(3.14*normeDenominateur*normeDenominateur);}
    else if (produitscalaire<=0){coeff=0;}


    couleur *= coeff;
    }


   // Construction de la nouvelle direction
    r1 = u(engine);
    r2 = u(engine);

    x = cos(2*3.14*r1)*sqrt(1-r2);
    y = sin(2*3.14*r1)*sqrt(1-r2);
    z = sqrt(r2);
    Vecteur dirAlea(x,y,z);


    Vecteur tangente1=Normale*dirAlea;
    tangente1 *= (1/tangente1.GetNorme());
    Vecteur tangente2=tangente1*Normale;

    // on écrase l'ancienne direction pour y mettre la nouvelle direction aléatoire
    Normale*=z;
    tangente1 *=x;
    tangente2 *=y;
    tempDir = Normale;
    tempDir += tangente1;
    tempDir += tangente2;

    Vecteur prochaine_couleur = BRDF(AncienP,tempDir,L,I,prochain_index,rebonds+1);

    prochaine_couleur.SetX(prochaine_couleur.GetX()*ListeSphere[index].GetRauR());
    prochaine_couleur.SetY(prochaine_couleur.GetY()*ListeSphere[index].GetRauG());
    prochaine_couleur.SetZ(prochaine_couleur.GetZ()*ListeSphere[index].GetRauB());
    couleur+=prochaine_couleur;
    return couleur;
    }
    else{
            Vecteur couleur(ListeSphere[index].GetRauR(),ListeSphere[index].GetRauG(),ListeSphere[index].GetRauB());
            return couleur;
    }


}

/*Object::Object(){

}*/
