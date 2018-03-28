#define InfoGraphCours1_Vecteurs

using namespace std;
#include <vector>
#include<math.h>

class Vecteur
{
    public :
        Vecteur(double x=0, double y=0, double z=0);
        void set(double x, double y, double z);
        void operator+=(const Vecteur V);
        void operator*=(double w);
        Vecteur operator*(double w);
        Vecteur operator*(const Vecteur V);
        void operator=(const Vecteur V);
        double GetX() const;
        double GetY() const;
        double GetZ() const;
        void SetX(double x);
        void SetY(double y);
        void SetZ(double z);
        double GetNorme() const;

    private :
        double X;
        double Y;
        double Z;
};

/*class Object
{
        public:
    Object();
    virtual bool intersection(const Vecteur C, const Vecteur u) const;

};*/



class Sphere //: public Object
{
    public :

        Sphere(const Vecteur o, double r=0, double Rau_R=0.5, double Rau_G=0.5, double Rau_B=0.5,bool specularite=false);
        void set(const Vecteur o, double r, double Rau_R=0.5, double Rau_G=0.5, double Rau_B=0.5);
        double racines(const Vecteur C, const Vecteur u);
        bool intersection(const Vecteur C, const Vecteur u);
        double ombrage(const Vecteur C, const Vecteur u, const Vecteur L, double I);
        double GetRauR() ;
        double GetRauG() ;
        double GetRauB() ;
        double GetOX() const;
        double GetOY() const;
        double GetOZ() const;
        bool GetSpec() const;

	private :

        Vecteur O;
		double R;
        double rau_R;
        double rau_G;
        double rau_B;
        bool specularite;
};

class Scene
{
    public :
        Scene();
        void AddSphere(const Sphere S);
        Sphere GetIndex(int i);
        bool Eclairage(const Vecteur C, const Vecteur u, const Vecteur L,int index);
        Vecteur Specularite(Vecteur AncienP,Vecteur direction,int index,int rebonds);
        Vecteur BRDF(Vecteur AncienP, Vecteur direction, Vecteur L, int I, int index, int rebonds);
        int NbElem();
    private:
        std::vector<Sphere> ListeSphere;
};



/* class Triangle
{
public: Object
    Triangle(const Vecteur& A,const Vecteur& B,const Vecteur& C);
    virtual bool intersection(const Vecteur C, const Vecteur u){

        // intersection du rayon avec le plan
        double t = dot(A - C,N) / dot(u,N);
        if(t<0){return false;}; // si t<0 on regarde dans la direction opposée au plan
        Vecteur P;
        P = C + t*u;

        // definition des coefficients des matrices
        double APAB = dot(P-A,B-A); // prod. scalaire <AP,AB>
        double APAC = dot(P-A,C-A); // prod. scalaire <AP,AC>
        double ACAB = dot(C-A,B-A); // prod. scalaire <AC,AB>
        double ABAC = dot(B-A,C-A); // prod. scalaire <AB,AC>
        double ACAC = dot(C-A,C-A); // norme au carree AC
        double ABAB = dot(B-A,B-A); // norme au carree AB

    }

private :
    // coordonnees des sommets
    Vecteur A,B,C;
    Vecteur N;
};*/



// il faut modifier la liste de Scene en des pointeurs vers des Objects
// on remplace 'Sphere' par  'const Object*' (avec le * !!!)
// On peut changer le nom de la liste également
// partout où on a 'NomDeLaListe.' il faut changer en 'NomDeLaListe->' car ce sont des pointeurs maintenant

// Utiliser dynamic_cast<const Sphere> pour les objects dans la liste des objets de la scene dont on sait
// qu'ils sont des spheres. On peut faire pareil pour les triangles.

// classes pour lire des maillages triangulaires
// il faut également coder la routine d'intersection pour la classe geometry
// elle doit parcourir tous les triangles du maillages pour savoir s'il y a intersection

/*
#include <vector>
#include <string>

class TriangleIndices
{
public:
    TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1) : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi), uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk) {
    };
    int vtxi, vtxj, vtxk;
    int uvi, uvj, uvk;
    int ni, nj, nk;
};

class Geometry : public Object
{
public:
    Geometry() {};
    Geometry(const char* obj, double scaling, const Vector& offset) {
        readOBJ(obj);
        for (int i = 0; i < vertices.size(); i++) {
            vertices[i] = vertices[i] * scaling + offset;
        }
    }

    void readOBJ(const char* obj) {

        char matfile[255];
        char grp[255];

        FILE* f;
        f = fopen(obj, "r");

        while (!feof(f)) {
            char line[255];
            if (!fgets(line, 255, f)) break;

            std::string linetrim(line);
            linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
            strcpy(line, linetrim.c_str());

            if (line[0] == 'u' && line[1] == 's') {
                sscanf(line, "usemtl %[^\n]\n", grp);
            }
            if (line[0] == 'm' && line[1] == 't' && line[2] == 'l') {
                sscanf(line, "mtllib %[^\n]\n", matfile);
            }
            if (line[0] == 'v' && line[1] == ' ') {
                Vector vec;
                Vector col;
                if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[2], &vec[1], &col[0], &col[1], &col[2]) == 6) {
                    vertices.push_back(vec);
                    vertexcolors.push_back(col);
                }
                else {
                    sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[2], &vec[1]);  // helmet
                                                                                 //vec[2] = -vec[2]; //car2
                    vertices.push_back(vec);
                }
            }
            if (line[0] == 'v' && line[1] == 'n') {
                Vector vec;
                sscanf_s(line, "vn %lf %lf %lf\n", &vec[0], &vec[2], &vec[1]); //girl
                normals.push_back(vec);
            }
            if (line[0] == 'v' && line[1] == 't') {
                Vector vec;
                sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
                uvs.push_back(vec);
            }
            if (line[0] == 'f') {
                TriangleIndices t;
                int i0, i1, i2, i3;
                int j0, j1, j2, j3;
                int k0, k1, k2, k3;
                int nn;

                char* consumedline = line + 1;
                int offset;

                nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
                if (nn == 9) {
                    if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                    if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                    if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                    if (j0 < 0) t.uvi = uvs.size() + j0; else	t.uvi = j0 - 1;
                    if (j1 < 0) t.uvj = uvs.size() + j1; else	t.uvj = j1 - 1;
                    if (j2 < 0) t.uvk = uvs.size() + j2; else	t.uvk = j2 - 1;
                    if (k0 < 0) t.ni = normals.size() + k0; else	t.ni = k0 - 1;
                    if (k1 < 0) t.nj = normals.size() + k1; else	t.nj = k1 - 1;
                    if (k2 < 0) t.nk = normals.size() + k2; else	t.nk = k2 - 1;

                    indices.push_back(t);
                }
                else {
                    nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
                    if (nn == 6) {
                        if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                        if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                        if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                        if (j0 < 0) t.uvi = uvs.size() + j0; else	t.uvi = j0 - 1;
                        if (j1 < 0) t.uvj = uvs.size() + j1; else	t.uvj = j1 - 1;
                        if (j2 < 0) t.uvk = uvs.size() + j2; else	t.uvk = j2 - 1;
                        indices.push_back(t);
                    }
                    else {
                        nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
                        if (nn == 3) {
                            if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                            if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                            if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                        }
                        else {
                            nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
                            if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                            if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                            if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                            if (k0 < 0) t.ni = normals.size() + k0; else	t.ni = k0 - 1;
                            if (k1 < 0) t.nj = normals.size() + k1; else	t.nj = k1 - 1;
                            if (k2 < 0) t.nk = normals.size() + k2; else	t.nk = k2 - 1;
                        }
                    }
                }


                consumedline = consumedline + offset;

                while (true) {
                    if (consumedline[0] == '\n') break;
                    if (consumedline[0] == '\0') break;
                    nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
                    TriangleIndices t2;
                    if (nn == 3) {
                        if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                        if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                        if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                        if (j0 < 0) t2.uvi = uvs.size() + j0; else	t2.uvi = j0 - 1;
                        if (j2 < 0) t2.uvj = uvs.size() + j2; else	t2.uvj = j2 - 1;
                        if (j3 < 0) t2.uvk = uvs.size() + j3; else	t2.uvk = j3 - 1;
                        if (k0 < 0) t2.ni = normals.size() + k0; else	t2.ni = k0 - 1;
                        if (k2 < 0) t2.nj = normals.size() + k2; else	t2.nj = k2 - 1;
                        if (k3 < 0) t2.nk = normals.size() + k3; else	t2.nk = k3 - 1;
                        indices.push_back(t2);
                        consumedline = consumedline + offset;
                        i2 = i3;
                        j2 = j3;
                        k2 = k3;
                    }
                    else {
                        nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
                        if (nn == 2) {
                            if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                            if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                            if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                            if (j0 < 0) t2.uvi = uvs.size() + j0; else	t2.uvi = j0 - 1;
                            if (j2 < 0) t2.uvj = uvs.size() + j2; else	t2.uvj = j2 - 1;
                            if (j3 < 0) t2.uvk = uvs.size() + j3; else	t2.uvk = j3 - 1;
                            consumedline = consumedline + offset;
                            i2 = i3;
                            j2 = j3;
                            indices.push_back(t2);
                        }
                        else {
                            nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
                            if (nn == 2) {
                                if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                                if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                                if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                                if (k0 < 0) t2.ni = normals.size() + k0; else	t2.ni = k0 - 1;
                                if (k2 < 0) t2.nj = normals.size() + k2; else	t2.nj = k2 - 1;
                                if (k3 < 0) t2.nk = normals.size() + k3; else	t2.nk = k3 - 1;
                                consumedline = consumedline + offset;
                                i2 = i3;
                                k2 = k3;
                                indices.push_back(t2);
                            }
                            else {
                                nn = sscanf(consumedline, "%u%n", &i3, &offset);
                                if (nn == 1) {
                                    if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                                    if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                                    if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                                    consumedline = consumedline + offset;
                                    i2 = i3;
                                    indices.push_back(t2);
                                }
                                else {
                                    consumedline = consumedline + 1;
                                }
                            }
                        }
                    }
                }

            }


        }
        fclose(f);
    }
    std::vector<TriangleIndices> indices;
    std::vector<Vector> vertices;
    std::vector<Vector> normals;
    std::vector<Vector> uvs; // Vector en 3D mais on n'utilise que 2 composantes
    std::vector<Vector> vertexcolors;
};*/
