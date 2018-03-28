// Raytracer.cpp : Defines the entry point for the console application.
#define _CRT_SECURE_NO_WARNINGS // for Visual Studio 2017 (maybe 2015 as well)

#include <iostream>
#include <vector>
#include "InfoGraphCours1_Vecteurs.h"
#include <math.h>
#include <random>



void save_image(const char* filename, const unsigned char* tableau, int w, int h) { // (0,0) is top-left corner

    FILE *f;

    int filesize = 54 + 3 * w*h;

    unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
    unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
    unsigned char bmppad[3] = { 0,0,0 };

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(w);
    bmpinfoheader[5] = (unsigned char)(w >> 8);
    bmpinfoheader[6] = (unsigned char)(w >> 16);
    bmpinfoheader[7] = (unsigned char)(w >> 24);
    bmpinfoheader[8] = (unsigned char)(h);
    bmpinfoheader[9] = (unsigned char)(h >> 8);
    bmpinfoheader[10] = (unsigned char)(h >> 16);
    bmpinfoheader[11] = (unsigned char)(h >> 24);

    f = fopen(filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    unsigned char *row = new unsigned char[w * 3];
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++) {
            row[j * 3] = tableau[(w*(h - i - 1) * 3) + j * 3+2];
            row[j * 3+1] = tableau[(w*(h - i - 1) * 3) + j * 3+1];
            row[j * 3+2] = tableau[(w*(h - i - 1) * 3) + j * 3];
        }
        fwrite(row, 3, w, f);
        fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
    }
    fclose(f);
    delete[] row;
}

int main()
{

    int W = 1024;
    int H = 1024;
    std::vector<unsigned char> img(W*H * 3, 0);

    /* Definition des donnees du probleme */
    double AngleVue = -W/(2*tan(3.14*30/180));
    Vecteur C(0,0,55);
    Vecteur O1(-11,0,-20);
    Vecteur O2(11,0,-20);
    Vecteur L(-10,-20,40);
    Vecteur U(0.0,0.0,AngleVue);

    Vecteur OrigineSol(0,1000,0);
    Vecteur OriginePlafond(0,-1000,0);
    Vecteur OrigineMurGauche(1000,0,0);
    Vecteur OrigineMurFond(0,0,-1000);
    Vecteur OrigineMurDroite(-1000,0,0);
    Vecteur OrigineMurDos(0,0,1000);
    Vecteur OrigineCadreur(0,0,65);
    double RayonSol,RayonPlafond,RayonMurGauche,RayonMurFond,RayonMurDroite,RayonMurDos;
    RayonSol = 940;
    RayonPlafond = 940;
    RayonMurGauche = 945;
    RayonMurDroite = 945;
    RayonMurFond = 940;
    RayonMurDos = 940;




    double R=10,I=500000000;
    Sphere S1(O1,R,1,1,1);
    Sphere S2(O2,R,0,1,1,true);
    Sphere Sol(OrigineSol,RayonSol,1,0,0);
    Sphere Plafond(OriginePlafond,RayonPlafond,0.5,0.25,0);
    Sphere MurGauche(OrigineMurGauche,RayonMurGauche,0.8,0,0.1);
    Sphere MurDroite(OrigineMurDroite,RayonMurDroite,0,0,1);
    Sphere MurFond(OrigineMurFond,RayonMurFond,1,1,0);
    Sphere MurDos(OrigineMurDos,RayonMurDos,0,1,1);
    Sphere Cadreur(OrigineCadreur,R,0,0,0);

    Scene MaScene;
    MaScene.AddSphere(Sol);
    MaScene.AddSphere(Plafond);
    MaScene.AddSphere(MurGauche);
    MaScene.AddSphere(MurDroite);
    MaScene.AddSphere(MurFond);
    MaScene.AddSphere(MurDos);
    MaScene.AddSphere(S1);
    MaScene.AddSphere(S2);
    //MaScene.AddSphere(Cadreur);


    // variables pour la boucle
    int indexe_a_sonder;
    double current_t;
    double newt;
    bool flag;
    bool eclaire;
    int N=50; //nb de rayons générés par pixel


    /* On parcourt tous les pixels */
    #pragma omp parallel for
    for(int h=0;h<=1023;h++)
    {
        for(int w=0;w<=1023;w++)
        {                
            /* Définition de paramètres  */
            current_t = 2000;   // On initialise t, de l'équation P = C + t*u comme si P était à l'infinie
            indexe_a_sonder = 0; // on commence par sonder la première sphère de la scène
            flag = false;   // pour l'instant on a pas détecter d'intersection
            eclaire = false;    // et on ne sait pas si la sphère est éclairée

            // U est le vecteur directeur du rayon lumineux
            U.set(h-512.0,w-512.0,AngleVue);

            // On parcours les objets de la scène
            for(int indexe=0;indexe<MaScene.NbElem();indexe++)
            {
                // On vérifie s'il y a intersection entre le rayon et la sphère ou non
                if (MaScene.GetIndex(indexe).intersection(C,U))
                {
                    // Si oui alors on va chercher le t qui permet d'atteindre le point de la sphère
                    newt = MaScene.GetIndex(indexe).racines(C,U);

                    // Si le nouveau t est supérieur à 0 (on regarde devant soi) et plus petit que l'ancien t
                    // (pour n'afficher que la sphère la plus au premier plan touchée par ce rayon)
                    if ((newt>0) && (newt<current_t))
                    {
                            // Alors on stocke le nouveau t,l'indexe de la sphère à afficher dans la liste
                            // des objets de la scène, et on met le flag sur true pour indiquer qu'une
                            // intersection a été trouvée
                            current_t = newt;
                            indexe_a_sonder = indexe;
                            flag = true;
                            // On doit regarder si le pixel est eclairé ou pas
                            eclaire = MaScene.Eclairage(C,U,L,indexe);

                    }
                }

                }
                // il faut qu'il y ait eu intersection pour ce pixel sondée
                if (flag)
                {
                    /*if (eclaire)
                    {
                        // on va chercher le coefficient à appliquer aux couleurs
                        double coeff = MaScene.GetIndex(indexe_a_sonder).ombrage(C,U,L,I);

                        if (MaScene.GetIndex(indexe_a_sonder).GetSpec())
                        {

                            Vecteur MaCouleur = MaScene.Specularite(C,U,indexe_a_sonder,0);
                            img[(w * W + h)*3] = min(pow(coeff*MaCouleur.GetX(),0.45),255.);
                            img[(w * W + h)*3+1] = min(pow(coeff*MaCouleur.GetY(),0.45),255.);
                            img[(w * W + h)*3+2] = min(pow(coeff*MaCouleur.GetZ(),0.45),255.);
                        }
                        else{

                            img[(w * W + h)*3] = min(pow(coeff*MaScene.GetIndex(indexe_a_sonder).GetRauR(),0.45),255.);
                            img[(w * W + h)*3+1] = min(pow(coeff*MaScene.GetIndex(indexe_a_sonder).GetRauG(),0.45),255.);
                            img[(w * W + h)*3+2] = min(pow(coeff*MaScene.GetIndex(indexe_a_sonder).GetRauB(),0.45),255.);
                        }

                    }*/
                    double rouge,vert,bleu;


                    for(int n=0;n<N;n++){
                       Vecteur MaCouleur=MaScene.BRDF(C,U,L,I,indexe_a_sonder,0);

                       rouge +=  (MaCouleur.GetX());
                       vert +=  (MaCouleur.GetY());
                       bleu += (MaCouleur.GetZ());

                    }


                    img[(w * W + h)*3] = (min(pow(rouge/N,0.45),255.));
                    img[(w * W + h)*3+1] = (min(pow(vert/N,0.45),255.));
                    img[(w * W + h)*3+2] = (min(pow(bleu/N,0.45),255.));
                    rouge=0,vert=0,bleu=0;


               }


        }
    }



    save_image("textures_fonct.bmp", &img[0], W, H);

    return 0;
}
