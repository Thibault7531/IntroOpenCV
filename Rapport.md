# **Rapport \- Analyse d’image**

## 1. **Présentation théorique des détecteurs utilisés**

Trois détecteurs ont été utilisés pour obtenir les contours dans une image.

### a. **Sobel**

Le premier détecteur se base sur l’algorithme de Sobel. Le principe est de calculer une dérivée de l’image et de détecter des pics sur cette dérivée. Cet algorithme nécessite que l’image soit en niveaux de gris. La dérivée représente les fluctuations de l’intensité des pixels. Une forte fluctuation de l’intensité montre en général un contour dans l’image.

<img src="Docs/Screenshot From 2026-10-23 15-28-14.png" width="256">
<br>
Opérateur Sobel avec un noyau de taille 5

Pour obtenir la dérivée d’une image, on applique ce qu’on appelle un noyau. Un noyau est une opération prenant en compte les pixels voisins pour déterminer la valeur d’un pixel. La taille de ce noyau peut être configurée. Un noyau plus petit prend en compte des petits détails dans l’image mais peut être plus vulnérable au bruit. Un noyau plus grand permet de réduire l’impact du bruit sur la détection des contours mais réduit la capacité à détecter des petits détails.  
De plus, on peut aussi configurer quel niveau de fluctuation peut être détecté comme un contour. Plus le niveau est bas, plus on aura de détail dans la détection des contours mais augmenter le bruit et augmenter la taille des contours. Plus le niveau est haut, plus les contours peuvent être fragmentés.

<img src="Docs/Screenshot From 2026-10-23 15-38-48.png" width="256">
<br>
Détection des contours Sobel avec un noyau de taille 5 et un threshold de 160

### b. **Laplace**

Le deuxième détécteur se base sur l’algortithme de Laplace. Cet algorithme consiste à calculer la dérivée seconde de l’image. Cet algorithme nécessite que l’image soit en niveaux de gris. Plutôt que de détecter lorsqu’il y a un grand changement d’intensité, on regarde lorsque le taux de changement d’intensité change rapidement et passe par zéro. Cette opération s’applique aussi grâce à un noyau dont la taille est configurable. Pour obtenir les contours de l’image, on doit récupérer la valeur absolue de l’opération de Laplace et appliquer un threshold. Cet algorithme étant très sensible au bruit, il est préférable d’appliquer un flou gaussien sur l’image d’entrée. De plus, les contours étant souvent fragmentés, une étape de dilution est appliquée pour grossir et connecter les contours.  

<img src="Docs/Screenshot From 2026-10-23 16-00-20.png" width="256">
<br>
Détection des contours Laplace avec un noyau de taille 5 et un threshold de 70

### c. **Canny**

Le troisième détecteur utilisé se base sur l’algorithme de Canny. Cet algorithme applique un flou sur l’image de départ pour réduire l’impact du bruit, puis calcule la dérivée comme pour l’algorithme de Sobel. Cependant, après avoir calculé cette dérivée, l’algorithme réduit la taille des contours à 1 pixel et utilise 2 thresholds pour classer les pixels dans 3 catégories. Il y a les pixels qui ne sont pas des contours, les pixels qui sont des contours et les pixels où on ne sait pas tout de suite s'ils font partie des contours. Cette dernière catégorie de pixel est utilisée dans une dernière étape pour connecter les contours entre eux.  

<img src="Docs/Screenshot From 2026-10-23 16-09-53.png" width="256">
<br>
Détection des contours Canny avec un premier threshold à 330 et un deuxième threshold à 440

## **2\. Fonctions OpenCV utilisées**

### a. **Sobel**  
   

cv::Sobel() : Cette fonction calcule la dérivée de l’image. Elle a pour paramètre, une image d’entrée et de sortie, le format de l’image de sortie, le sens de la dérivée et la taille du noyau appliqué. Elle doit être appliquée 2 fois à l’image d'entrée selon l’axe x et y.

cv::minMaxLoc() : Permet d’obtenir la valeur maximale et minimale d’une image.

image.convertTo() : Permet de convertir une image vers un nouveau format en appliquant une fonction: alpha \* pixel \+ beta.

cv::threshold() : Permet d’obtenir une image binaire en fonction d’un seuil défini.

cv::cvtColor() : Permet de convertir une image vers un nouvel espace de couleur. Peut être utilisé pour convertir une image en nuances de gris.

### b. **Laplace**

cv::GaussianBlur() : Permet d’appliquer un flou gaussien à une image.

cv::subtract() : Permet d’obtenir la différence entre deux images.

cv::Laplacian() : Permet de calculer la dérivée seconde d’une image avec comme paramètre la taille du noyau utilisé.

cv::convertScaleAbs(): Permet d’obtenir une image avec les valeurs absolue d’une image d’entrée.

cv::threshold() : Permet d’obtenir une image binaire en fonction d’un seuil défini.

image.convertTo() : Permet de convertir une image vers un nouveau format en appliquant une fonction: alpha \* pixel \+ beta.

cv::dilate(): Applique un noyau sur une image qui permet d’étendre les régions blanches d’une image.

### c. **Canny**

cv::Canny() : Permet d’appliquer l’algorithme de canny en entier sur une image. Cette fonction prend en paramètres une image d’entrée et de sortie et les deux seuils à utiliser.

### d. **Autres**

cv::imread() : Permet d’importer une image avec son chemin en spécifiant l’espace de couleur à utiliser.

cv::namedWindow() : Permet de créer une fenêtre pour y afficher une image.

cv::imshow() : Permet d’afficher une image dans une fenêtre.

cv::createTrackbar() : Permet d’ajouter une barre permettant de choisir une valeur dans une fenêtre avec des bornes spécifiées.

cv::getTrackbarPos() : Permet d’obtenir la valeur d’une trackbar.

cv::waitKey() : Permet d’attendre la saisie d’une touche du clavier pour continuer le programme.

## **3\. Résultats**

Pour tester chaque détecteur, on va prendre un lot de 3 images et maximiser manuellement la performance du détecteur sur cette image en ajustant les paramètres. On va ensuite prendre la moyenne de ces paramètres et tester le détecteur sur l’entièreté des images de test. On prendra les résultats sur l'entièreté des images.

1ère image:  
hyena.pgm  
Cette image possède un contraste élevé entre le premier-plan et l'arrière-plan.

2ème image:  
basket.pgm  
Cette image possède un faible contraste et possède de l’herbe qui peut rendre la détection plus complexe.

3ème image:  
rino.pgm  
Cette image possède un faible contraste mais peu de textures complexes.

### a. **Sobel**

Paramètres maximisants:

hyena.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-13-02.png" width="256">
<br>
Taille de noyau: 1  
Seuil: 175

basket.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-15-44.png" width="256">
<br>
Taille de noyau: 5  
Seuil: 157

rino.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-18-31.png" width="256">
<br>
Taille de noyau: 7  
Seuil: 26

Paramètres optimaux:  
Taille de noyau \= (3 \+ 5 \+ 7\) / 3 \= 5  
Seuil \= (175 \+ 157 \+ 26\) / 3 \= 119

Résultats:  
Performance moyenne: 0.166733  
Taux de faux positifs moyen: 0.076676  
Taux de faux négatifs moyen: 0.830091  
Performance médiane: 0.264139  
Taux de faux positif médian: 0.128140  
Taux de faux négatif médian: 0.719694

### b. **Laplace**

Paramètres maximisants:

hyena.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-28-48.png" width="256">
<br>
Taille du noyau: 5  
Seuil: 98

basket.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-33-24.png" width="256">
<br>
Taille du noyau: 5  
Seuil: 80

rino.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-35-35.png" width="256">
<br>
Taille du noyau: 5  
Seuil: 60

Paramètres optimaux:  
Taille de noyau \= (5 \+ 5 \+ 5\) / 3 \= 5  
Seuil \= (98 \+ 80 \+ 60\) / 3 \= 79

Résultats:  
Performance moyenne: 0.167314  
Taux de faux positifs moyen: 0.504273  
Taux de faux négatifs moyen: 0.402662  
Performance médiane: 0.155442  
Taux de faux positif médian: 0.543375  
Taux de faux négatif médian: 0.372355

### c. **Canny**

Paramètres maximisants:

hyena.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-42-29.png" width="256">
<br>
Seuil 1: 252  
Seuil 2: 494

basket.pgm  
<img src="Docs/Screenshot From 2026-10-23 19-47-39.png" width="256">
<br>
Seuil 1: 262  
Seuil 2: 402

rino.pgm  
<img src="Docs/Screenshot From 2026-10-23 20-22-18.png" width="256">
<br>
Seuil 1: 100  
Seuil 2: 357

Paramètres optimaux:  
Seuil 1 \= (252 \+ 262 \+ 100\) / 3 \= 205  
Seuil 2 \= (494 \+ 402 \+ 357\) / 3 \= 418  
Résultats:  
Performance moyenne: 0.307343  
Taux de faux positifs moyen: 0.252314  
Taux de faux négatifs moyen: 0.559875  
Performance médiane: 0.254617  
Taux de faux positif médian: 0.094860  
Taux de faux négatif médian: 0.759259

### d. **Résumé**

|  | Sobel | Laplace | Canny |
| :---- | :---- | :---- | :---- |
| Performance moyenne | 0.166733 | 0.167314 | 0.307343 |
| Taux de faux positifs moyen | 0.076676 | 0.504273 | 0.252314 |
| Taux de faux négatifs moyen | 0.830091 | 0.402662 | 0.559875 |
| Performance médiane | 0.264139 | 0.155442 | 0.254617 |
| Taux de faux positif médian | 0.128140 | 0.543375 | 0.094860 |
| Taux de faux négatif médian | 0.719694 | 0.372355 | 0.759259 |

En prenant en compte la performance de chacun des détecteurs, on peut voir qu’en moyenne, l’opérateur Canny est plus performant, le plaçant devant Sobel et Laplace. Cependant, si on regarde la performance médiane, on voit que Canny est toujours très bon mais il est dépassé par Sobel. Cela peut signifier que l’opérateur Sobel peut être très performant mais moins régulier. Sobel nécessite des paramètres plus spécifiques pour une image alors que Canny peut être appliqué sur plus d’images avec les mêmes paramètres et donner des résultats corrects.

Pour ce qui est du taux de faux positifs, on peut voir que les opérateurs Sobel et Canny devancent de loin l’opérateur Laplace. Laplace produit des contours très épais, ce qui explique le fort taux de faux positifs. Les opérateurs Sobel et Canny quant à eux produisent des contours très fins, ce qui donne une qualité de contours plus élevée. Avec ces deux opérateurs, on a moins d’incertitude sur les contours détectés, ce qui peut être bénéfique dans des domaines d’applications où la moindre erreur peut être critique.

Pour finir, on peut voir que l'opérateur Laplace domine sur le taux de faux négatifs. En effet, en générant des contours très épais, cet opérateur détecte plus de contours présents sur l’image de référence au prix d’en détecter trop. Cet opérateur est à utiliser si on veut détecter le plus de contours possibles sans se soucier d’en avoir en trop.

## 4\. Conclusion

En conclusion, chacun de ces opérateurs peuvent être utilisés suivant les besoins. Si, on veut un détecteur qui fait le moins d’erreur possible tout en étant utilisable sur un grand lot d’images sans modifier les paramètres, le détecteur Canny est à privilégier. Si, le but est d’obtenir le plus de contours possibles sans se soucier des erreurs, le détecteur Laplace est à privilégier.
