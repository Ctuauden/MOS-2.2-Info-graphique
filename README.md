# MOS-2.2-Info-graphique
# I – Organisation du programme
Ce programme comporte plusieurs classes : Vecteur, Sphere et Scene.
*	Une instance de la classe Vecteur possède trois attributs X, Y et Z. 
Cette classe est utilisée pour décrire à la fois des vecteurs de direction (comme la direction de la lumière) mais également pour repérer des coordonnées de points (exemple : le centre de chaque sphère est un vecteur O[Ox,Oy,Oz]).
Une méthode importante de cette classe est la méthode GetNorm() qui renvoie la norme euclidienne d’un Vecteur.
* Une instance de la classe Sphere possède 6 attributs : 
  *	O qui est une instance de la classe Vecteur et donne le centre de la sphère ;
  *	R qui est un réel indiquant le rayon de la sphère ;
  *	Rau_R, rau_G et rau_B qui représentent l’albédo de la sphère ;
  *	Specularite qui est un booléen renseignant sur la nature spéculaire ou non de la sphère.
Cette classe possède 3 méthodes très importantes qui sont racines, intersection et ombrage.
La méthode racines calcule le scalaire par lequel il faut multiplier le vecteur directeur normé d’un rayon pour atteindre le point d’intersection du rayon et de la sphère. Au cas où cette racine n’existe pas (le rayon ne touche pas la sphère) alors on renvoie -1.
La méthode intersection fait appel à la méthode racines et délivre un booléen qui témoigne du fait que le rayon touche ou non la sphère.
Enfin la méthode ombrage permet de déterminer si le pixel considérer est éclairé directement ou non par lumière de la scène.
* Une instance de la classe Scene possède un seul attribut ListeSphere qui contient la liste des sphères présentes dans la scène à déssiner.
On ajoute à cette liste une sphère grâce à la méthode AddSphere 
Cette classe possède une méthode Eclairage qui va renvoyer un booléen indiquant le caractère éclairé ou non du point sondé.
Elle contient également une méthode Specularite qui renvoie un vecteur dont les composantes sont l’albédo (RGB) de la sphère non-spéculaire sur laquelle le rayon a rebondi. Cette méthode permet de créer des sphères miroirs.
Enfin, la classe est dotée d’une méthode BRDF qui calcul l’équation du rendu pour un point donné. Elle incorpore également la gestion de l’anti-aliasing.
# II – Première scène simple
Voici une première image qui ne contient qu’une sphère avec un éclairage qui dépend de l’exposition et la position des points par rapport à la source de lumière.
![Scene Simple](/scene_simple.bmp)
 
 

# III – Ajout d’ombres 
On passe maintenant à une scène dans laquelle on a placé plusieurs sphères pour simuler des murs sur lesquels voir les ombres qui se projettent.
![Ombres](/ombres.bmp)
 
 

# IV – Sphère miroir
On insère maintenant une sphère spéculaire qui va réfléchir ce qui l’entoure. On a ajouté un cadreur (simulé par une sphère noire derrière la caméra) pour observer que les réflexions marchent bien dans toutes les directions.
![Miroir](/miroir.bmp)
 
 

# V – Équation du rendu 
On a codé une équation du rendu en utilisant une BRDF afin d’obtenir un rendu plus réaliste en prenant en compte l’éclairage indirect.
![Rendu](/Equation_du_rendu.bmp)
 
 

# VI – Anti-Aliasing
L’anti-aliasing va nous permettre d’effacer le crénelage du au fait que les pixels soient carrés.
![AntiAliasing](/comparaison AA.png)
 
 

# VII – Textures procédurales
Les textures procédurales consistent à utiliser une fonction pour déterminer l’albédo d’une sphère (et même d’un point) plutôt que d’utiliser des surfaces prédéfinis.
![Textures_Procedurales](/textures_procédurales.bmp)
 

