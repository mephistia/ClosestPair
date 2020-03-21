#include <iostream>
#include "Point.h"
#include <clocale>
#include <ctime>
#include <algorithm>
#include <vector>

// Fun��o utilit�ria para gerar n�meros aleat�rios
float randEntre(int min, int max) {
	return min + rand() % (max + 1 - min);
}

// Fun��o para medir a dist�ncia euclidiana entre pontos
float dist(Point p, Point q) {
	return std::sqrt(std::pow(q.x - p.x, 2) + std::pow(q.y - p.y, 2));
}


// Encontrar par por Brute Force
std::vector<Point> closestPairBF(std::vector<Point>P, int n) {
	float minDist = INFINITY;
	std::vector<Point> closest(2,Point(1,1));

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			Point p = P[i], q = P[j]; //o ponto em cada posi��o

			// dist�ncia euclidiana
			float d = dist(p, q);
			if (d < minDist) {
				minDist = d;
				closest[0] = p;
				closest[1] = q;
			}
		}
	}
	return closest;

}


// Encontrar par por Divide and Conquer
std::vector<Point> closestPairDC(std::vector<Point> Px, std::vector<Point> Py) {

	// for�a bruta se 3 ou menos
	if (Px.size() <= 3) 
		return closestPairBF(Px, Px.size());
	

	// vari�veis
	int n = Px.size();
	int m = n / 2;

	std::vector<Point> PxRight;
	std::vector<Point> PxLeft;
	std::vector<Point> PyRight;
	std::vector<Point> PyLeft;


	// separar X
	std::copy(std::begin(Px), std::begin(Px) + (n / 2), std::back_inserter(PxLeft)); // de 0 a meio
	std::copy(std::begin(Px) + (n / 2), std::end(Px), std::back_inserter(PxRight)); // de meio ao final
	float xM = Px.at((n - 1) / 2).x; // x m�dio

	// separar Y
	std::copy_if(std::begin(Py), std::end(Py), std::back_inserter(PyLeft), [&xM](Point p) {
		return p.x <= xM; // insere em PyLeft se o x do Py for menor ou = o x do ponto m�dio
	});

	std::copy_if(std::begin(Py), std::end(Py), std::back_inserter(PyRight), [&xM](Point p) {
		return p.x > xM; // insere em PyRight se for maior
	});


	std::vector<Point> pair1 = closestPairDC(PxLeft, PyLeft); // par mais pr�ximo da esquerda
	std::vector<Point> pair2 = closestPairDC(PxRight, PyRight); // par mais pr�ximo da direita


	// calcular dist�ncias
	float dl = dist(pair1[0], pair1[1]);
	float dr = dist(pair2[0], pair2[1]);
	float d;

	std::vector<Point> closestPoints;

	// define os pontos mais pr�ximos, se � na direita ou esquerda
	if (dl < dr) {
		d = dl;
		closestPoints.push_back(pair1[0]);
		closestPoints.push_back(pair1[1]);
	}
	else {
		d = dr;
		closestPoints.push_back(pair2[0]);
		closestPoints.push_back(pair2[1]);
	}


	// faixa entre dl e dr
	std::vector<Point> yStrip;

	// insere pontos Py que tiverem dentro da menor dist�ncia
	std::copy_if(std::begin(Py), std::end(Py), std::back_inserter(yStrip), [&d, &xM](Point p) {
		return std::abs(xM - p.x) < d;
	});

	float result = d;

	// iterar para cada ponto at� o fim-1
	for (auto i = std::begin(yStrip); i != (std::end(yStrip) - 1); ++i) {
		// iterar para cada ponto+1 at� o fim, que a dist�ncia 
		// entre os dois seja menor q a dist�ncia m�nima para cada lado (ou seja, 2x)
		for (auto k = i + 1; k != std::end(yStrip) &&
			((k->y - i->y) < d); ++k) {
			// nova dist�ncia absoluta (valor positivo) � a dist�ncia entre os dois pontos iterados
			float newDistance = std::abs(dist(*k, *i));
			if (newDistance < result) {
				result = newDistance;
				closestPoints[0] = *k;
				closestPoints[1] = *i;
			}
		}
	}
	return closestPoints;
}

// Main -----------------------------------------
int main() {
	std::setlocale(LC_ALL, "pt_BR");
	srand((unsigned)time(0));

	std::vector<Point> points = {
		Point(-13, 0.5), 
		Point(-10.5, -11.5), 
		Point(-10, 9), 
		Point(-4.5, -2),
		Point(-1, 8.5),
		Point(0.5, 6),
		Point(0.5, -12),
		Point(2, 12.5),
		Point(3.5, 11),
		Point(5.5, 3),
		Point(5.5, -7),
		Point(5, 11.5),
		Point(6.5, 3.2),
		Point(7, -10),
		Point(9, -5),
		Point(11.5, -4)
	};
	

	// define o tamanho do vetor
	int size = points.size();


// M�todo Ing�nuo
	std::vector<Point> par = closestPairBF(points, size);


	// Imprimir na tela
	std::cout << std::endl << "M�TODO BRUTE FORCE" << std::endl << std::endl;
	std::cout << "Pontos predefinidos mais pr�ximos encontrados!" << std::endl;
	std::cout << "Coordenadas 1 = X: " << par[0].x << ", Y: " << par[0].y << std::endl;
	std::cout << "Coordenadas 2 = X: " << par[1].x << ", Y: " << par[1].y << std::endl << std::endl;
	std::cout << "Menor dist�ncia: " << dist(par[0], par[1]) << std::endl << std::endl;



	// Pontos aleat�rios
	std::vector<Point> r_points;
	std::cout << "Gerando pontos aleat�rios..." << std::endl << std::endl;
	for (int i = 0; i < 16; i++) { 
		float randX, randY;
		
		do {
			// criar aleat�rio (problema: s� gera inteiros)
			randX = randEntre(-15, 15); 
			randY = randEntre(-15, 15);

		// enquanto j� existir no array
		} while (std::find(r_points.begin(), r_points.end(), Point(randX,randY)) != r_points.end());
		
		// adicionar o ponto
		r_points.push_back(Point(randX, randY));
		std::cout  << "Coordenadas do ponto aleat�rio " << i << ":  " << r_points[i].x << ", " << r_points[i].y << std::endl;
	}


	// Encontrar posi��o do aleat�rio
	std::vector<Point> par2 = closestPairBF(r_points, 16);


	// Imprimir na tela
	std::cout << std::endl << "Pontos aleat�rios mais pr�ximos encontrados!" << std::endl;
	std::cout << "Coordenadas 1 = X: " << par2[0].x << ", Y: " << par2[0].y << std::endl;
	std::cout << "Coordenadas 2 = X: " << par2[1].x << ", Y: " << par2[1].y << std::endl << std::endl;
	std::cout << "Menor dist�ncia: " << dist(par2[0], par2[1]) << std::endl;


// Divide and Conquer
	std::cout << std::endl << "M�TODO DIVIDE AND CONQUER" << std::endl << std::endl;



	// Ordenar por eixo 
	std::sort(std::begin(points), std::end(points), [](Point a, Point b) {
		return a.x < b.x;
	});
	std::vector<Point> xPoints = points;

	std::sort(std::begin(points), std::end(points), [](Point a, Point b) {
		return a.y < b.y;
	});
	std::vector<Point> yPoints = points;
	
	// chamar a fun��o
	std::vector<Point> closest = closestPairDC(xPoints, yPoints);


	// Imprimir na tela
	std::cout << "Pontos predefinidos mais pr�ximos encontrados!" << std::endl;
	std::cout << "Coordenadas 1 = X: " << closest[0].x << ", Y: " << closest[0].y << std::endl;
	std::cout << "Coordenadas 2 = X: " << closest[1].x << ", Y: " << closest[1].y << std::endl << std::endl;
	std::cout << "Menor dist�ncia: " << dist(closest[0],closest[1]) << std::endl << std::endl;


// Divide and Conquer aleat�rio
	//ordernar por eixo
	std::sort(std::begin(r_points), std::end(r_points), [](Point a, Point b) {
		return a.x < b.x;
	});
	std::vector<Point> xPoints2 = r_points;

	std::sort(std::begin(r_points), std::end(r_points), [](Point a, Point b) {
		return a.y < b.y;
	});
	std::vector<Point> yPoints2 = r_points;

	// chamar a fun��o
	std::vector<Point> closest2 = closestPairDC(xPoints2, yPoints2);

	// Imprimir na tela
	std::cout << "Pontos aleat�rios mais pr�ximos  encontrados!" << std::endl;
	std::cout << "Coordenadas 1 = X: " << closest2[0].x << ", Y: " << closest2[0].y << std::endl;
	std::cout << "Coordenadas 2 = X: " << closest2[1].x << ", Y: " << closest2[1].y << std::endl << std::endl;
	std::cout << "Menor dist�ncia: " << dist(closest2[0], closest2[1]) << std::endl << std::endl;


	std::cin.get();
	return 0;
}
