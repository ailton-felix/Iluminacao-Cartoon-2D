/*
 * UtilityFunctions.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: ailton
 */

#include "../headers/UtilityFunctions.hpp"

Color UtilityFunctions::colors[34] =
{

	Color(1.00,1.00,1.00), //0 - Branco
	Color(1.00,0.00,1.00), //1 - Magenta
	Color(1.00,0.65,0.00), //2 - Laranja
	Color(0.56,0.93,0.56), //3 - LightGreen
	Color(1.00,1.00,0.00), //4 - Amarelo
	Color(0.39,0.58,0.92), //5 - CornflowerBlue
	Color(0.82,0.41,0.11), //6 - Chocolate
	Color(1.00,1.00,1.00), //7 - Branco
	Color(1.00,1.00,0.00), //8 - Amarelo
	Color(1.00,1.00,1.00), //9 - Branco
	Color(1.00,1.00,0.00), //10 - Amarelo
	Color(1.00,1.00,1.00), //11 - Branco
	Color(1.00,1.00,0.00), //12 - Amarelo
	Color(1.00,1.00,1.00), //13 - Branco
	Color(1.00,1.00,0.00), //14 - Amarelo
	Color(1.00,1.00,1.00), //15 - Branco
	Color(1.00,1.00,0.00), //16 - Amarelo
	Color(1.00,1.00,1.00), //17 - Branco
	Color(1.00,1.00,0.00), //18 - Amarelo
	Color(1.00,1.00,1.00), //19 - Branco
	Color(1.00,1.00,0.00), //20 - Amarelo
	Color(1.00,1.00,1.00), //21 - Branco
	Color(1.00,1.00,0.00), //22 - Amarelo
	Color(1.00,1.00,1.00), //23 - Branco
	Color(1.00,1.00,0.00), //24 - Amarelo
	Color(1.00,1.00,1.00), //25 - Branco
	Color(1.00,1.00,0.00), //26 - Amarelo
	Color(1.00,1.00,1.00), //27 - Branco
	Color(1.00,1.00,0.00), //28 - Amarelo
	Color(1.00,1.00,1.00), //29 - Branco
	Color(1.00,1.00,0.00), //30 - Amarelo
	Color(1.00,1.00,1.00), //31 - Branco
	Color(1.00,1.00,0.00), //32 - Amarelo
	Color(1.00,1.00,1.00)  //33 - Branco //todo novo
};

UtilityFunctions::UtilityFunctions() {

}

/*
 * Método que calcula a norma de um ponto 'p'.
 */
double UtilityFunctions::norm(Point2D p)
{
  return sqrt((p.x*p.x) + (p.y*p.y));
}

/*
 * Método que calcula a norma de um ponto 'p'(double).
 */
double UtilityFunctions::norm(Point3D p)
{
  return std::sqrt((p.x*p.x) + (p.y*p.y));
}

/*
 * Método que calcula o produto interno de dois pontos
 * 'a' e 'b'.
 */
double UtilityFunctions::scalarProduct(Point2D a, Point2D b)
{
  return a.x*b.x + a.y*b.y;
}

/*
 * Método que retorna um vetor formado pelo segmento 'ab',
 * ou seja, o vetor formado pela difernaça b - a.
 */
Point2D UtilityFunctions::getVector(Point2D a, Point2D b)
{
  return Point2D(b.x - a.x, b.y - a.y);
}

/*
 * Método que calcula o pseudo-ângulo de um ponto 'pi' em relação
 * ao ponto 'p' usando uma noçao de quadrado.
 */
/*double UtilityFunctions::calculatePseudoAngle(Point2D p, Point2D pi)
{
	double angle = 0;
	double EPSILON = 0.0000000000001;

	Point3D p_d(p.x, p.y, 0.0);
	Point3D pi_d(pi.x, pi.y, 0.0);
	pi_d.x = pi_d.x - p_d.x;
	pi_d.y = pi_d.y - p_d.y;

	double normMax = std::max(fabs(pi_d.x), fabs(pi_d.y));
	pi_d.x = (pi_d.x / normMax);
	pi_d.y = (pi_d.y / normMax);

	if(-EPSILON < pi_d.x && pi_d.x < EPSILON && -EPSILON < pi_d.y && pi_d.y < EPSILON)
	{
		angle = 0.0;
	} else if(pi_d.x > 0 && -EPSILON < pi_d.y && pi_d.y < EPSILON)
	{
		angle = 0.0;
	} else if(pi_d.x >= pi_d.y && pi_d.x > 0 && pi_d.y > 0)
	{
		angle = pi_d.y;
	} else if(pi_d.x < pi_d.y && pi_d.x > 0 && pi_d.y > 0)
	{
		angle = 2.0 - pi_d.x;
	} else if(-EPSILON < pi_d.x && pi_d.x < EPSILON && pi_d.y > 0)
	{
		angle = 2.0;
	} else if(-pi_d.x <= pi_d.y && pi_d.x < 0 && pi_d.y > 0)
	{
		angle = 2.0 - pi_d.x;
	} else if(-pi_d.x > pi_d.y && pi_d.x < 0 && pi_d.y > 0)
	{
		angle = 4.0 - pi_d.y;
	} else if(pi_d.x < 0 && -EPSILON < pi_d.y && pi_d.y < EPSILON)
	{
		angle = 4.0;
	} else if(pi_d.x <= pi_d.y && pi_d.x < 0 && pi_d.y < 0)
	{
		angle = 4.0 - pi_d.y;
	} else if(pi_d.x > pi_d.y && pi_d.x < 0 && pi_d.y < 0)
	{
		angle = 6.0 + pi_d.x;
	} else if(-EPSILON < pi_d.x && pi_d.x < EPSILON && pi_d.y < 0)
	{
		angle = 6.0;
	} else if(pi_d.x <= -pi_d.y && pi_d.x > 0 && pi_d.y < 0)
	{
		angle = 6.0 + pi_d.x;
	} else if(pi_d.x > -pi_d.y && pi_d.x > 0 && pi_d.y < 0)
	{
		angle = 8.0 + pi_d.y;
	} else { std::cout << "ERRO! Angulo não determinado!\n"; }

	//	cout << angle << endl;

	return angle;
}*/

/*
 * Método que calcula o pseudo-ângulo de um ponto 'pi' em relação
 * ao ponto 'p' usando uma noçao de quadrado.
 */
/*double UtilityFunctions::calculatePseudoAngle(Point2D p, Point2D pi)
{
	double angle = 0.0;

	Point3D p_d(p.x, p.y, 0.0);
	Point3D pi_d(pi.x, pi.y, 0.0);
	pi_d.x = pi_d.x - p_d.x;
	pi_d.y = pi_d.y - p_d.y;

	double normMax = std::max(fabs(pi_d.x), fabs(pi_d.y));
	pi_d.x = (pi_d.x / normMax);
	pi_d.y = (pi_d.y / normMax);

	if(pi_d.x >= pi_d.y && pi_d.x > 0.0 && pi_d.y >= 0.0)
	{
		//cout << "1\n";
		// Distância para o ponto (1,0)
		angle = sqrt((pi_d.x - 1.0)*(pi_d.x - 1.0) + (pi_d.y)*(pi_d.y));

	} else if(pi_d.x <= pi_d.y && pi_d.x > 0.0 && pi_d.y > 0.0) {
		//cout << "2\n";
		// Distância para o ponto (1,1)
		angle = 1.0 + sqrt((pi_d.x - 1.0)*(pi_d.x - 1.0) + (pi_d.y - 1.0)*(pi_d.y - 1.0));

	} else if(fabs(pi_d.x) <= pi_d.y && pi_d.x <= 0.0 && pi_d.y > 0.0) {
		//cout << "3\n";
		// Distância para o ponto (0,1)
		angle = 2.0 + sqrt((pi_d.x)*(pi_d.x) + (pi_d.y - 1.0)*(pi_d.y - 1.0));

	} else if(fabs(pi_d.x) >= pi_d.y && pi_d.x < 0.0 && pi_d.y > 0.0) {
		//cout << "4\n";
		// Distância para o ponto (-1,1)
		angle = 3.0 + sqrt((pi_d.x + 1.0)*(pi_d.x + 1.0) + (pi_d.y - 1.0)*(pi_d.y - 1.0));

	} else if(fabs(pi_d.x) >= fabs(pi_d.y) && pi_d.x < 0.0 && pi_d.y <= 0.0) {
		//cout << "5\n";
		// Distância para o ponto (-1,0)
		angle = 4.0 + sqrt((pi_d.x + 1.0)*(pi_d.x + 1.0) + (pi_d.y)*(pi_d.y));

	} else if(fabs(pi_d.x) <= fabs(pi_d.y) && pi_d.x < 0.0 && pi_d.y < 0.0) {
		//cout << "6\n";
		// Distância para o ponto (-1,-1)
		angle = 5.0 + sqrt((pi_d.x + 1.0)*(pi_d.x + 1.0) + (pi_d.y + 1.0)*(pi_d.y + 1.0));

	} else if(pi_d.x <= fabs(pi_d.y) && pi_d.x >= 0.0 && pi_d.y < 0.0) {
		//cout << "7\n";
		// Distância para o ponto (0,-1)
		angle = 6.0 + sqrt((pi_d.x)*(pi_d.x) + (pi_d.y + 1.0)*(pi_d.y + 1.0));

	} else if(pi_d.x >= fabs(pi_d.y) && pi_d.x > 0.0 && pi_d.y < 0.0) {
		//cout << "8\n";
		// Distância para o ponto (1,-1)
		angle = 7.0 + sqrt((pi_d.x - 1.0)*(pi_d.x - 1.0) + (pi_d.y + 1.0)*(pi_d.y + 1.0));

	} else { std::cout << "ERRO! Angulo não determinado!\n"; }

//	cout << angle << endl;

	return angle;
}*/

/**
 * Método que retorna a distância euclidiana entre os pontos 'a' e 'b'.
 */
double UtilityFunctions::euclideanDistance(Point2D a, Point2D b)
{
  return sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

/*
 * Método que retorna a posição da lista 'points' do ponto mais próxima de 'p'.
 */
/*int UtilityFunctions::nearPoint(Point2D p, list<Point2D> points)
{
  int index = 0;
  int count = 0;
  double min = euclideanDistance(p, points.front());

  for(list<Point2D>::iterator itPoints = points.begin() ; itPoints != points.end(); ++itPoints, ++count)
  {
	double temp = euclideanDistance(p, *itPoints);
    if(temp < min)
    {
      min = temp;
      index = count;
    }
  }

  return index;
}*/

/*
 * Método que reordena uma lista de pontos, fazendo a mesma começar a partir da posição 'i'.
 */
/*void UtilityFunctions::reorderList(int i, list<Point2D> &points)
{
  list<Point2D> l1;
  list<Point2D> l2;

  // pegando a posição do ponto da lista mais próximo do ponto 'p'.
  list<Point2D>::iterator it = points.begin();
  int count = 0;
  while(i != count++)
  {
	l1.push_back(*it++);
  }

  while(points.end() != it)
  {
    l2.push_back(*it++);
  }

  l2.splice(l2.end(), l1);

  points.clear();
  points = l2;
}*/

/*
 * Método que retorna o valor do coeficiente k do vetor resultante
 * do produto vetorial entre 'a' e 'b' (a x b). Que são pontos do R2.
 */
int UtilityFunctions::coefficientOfTheVectorProduct_K(Point2D a, Point2D b)
{
  return (a.x*b.y - b.x*a.y);
}

/*
 * Método que coloca a lista de pontos 'points' em ordem anti-horaria no sentido da curva fechada.
 */
void UtilityFunctions::PutOnAntiClockwise(list<Point2D> &points)
{
  if(points.size() > 2)
  {
  	list<Point2D>::iterator itPoints = points.begin();
  	Point2D p0, p1, p2;
  	p0 = *itPoints++; p1 = *itPoints++; p2 = *itPoints;
    Point2D v1 = getVector(p0, p1);	 Point2D v2 = getVector(p0, p2);

    if(coefficientOfTheVectorProduct_K(v1, v2) < 0)  points.reverse();
  }
}

/*
 * Método que cálcula a difença entre os vetores "tangCurrent" e "tangPrevious", que são vetores
 * tangentes a curva, dando assim um valor aproximado da curvatura da curva.
 */
double UtilityFunctions::Curvature(Point3D n1, Point3D n2)
{
	Point3D difference = Point3D(n1.x - n2.x, n1.y - n2.y, 0);

	return norm(difference);
}

/*
 * Retorna o índice tal que 'curvatures[indice] == value'.
 */
/*int UtilityFunctions::binarySearch(vector<double> curvatures, int min, int max, double value)
{
	while(min <= max)
	{
		int m = (min + max)/2;
		if(curvatures[m] == value)  return m;

		if(curvatures[m] < value)  	min = m + 1;
		else	max = m - 1;
	}

	return -1;
}*/

/*
 * Função que ordena em ordem DECRESCENTE o vetor 'v' em função do atributo '_index' usando o algoritmo QuickSort.
 * */
void UtilityFunctions::quickSort(vector<point_tuple> &v, int ini, int end)
{

	int i = ini;
	int j = end;
	point_tuple pivot = v[(ini + end) / 2];

	while(i < j)
	{
//		while(v[i]._index < pivot._index) i++;
//		while(v[j]._index > pivot._index) j--;
		while(v[i]._index > pivot._index) i++;
		while(v[j]._index < pivot._index) j--;

		if(i <= j)
		{
			point_tuple aux = v[i];
			v[i] = v[j];
			v[j] = aux;
			i++, j--;
		}
	}

	if(j > ini)  quickSort(v, ini, j);
	if(i < end)  quickSort(v, i, end);
}

/*
 * Função que retorna o índice do menor elemento do vetor 'v'.
 */
int UtilityFunctions::min(vector<double> v)
{
	int index = 0;

	if(!v.empty())
	{
		double min = v[0];

		for(int i = 1; i < v.size(); ++i)
		{
			if(v[i] < min)
			{
				min = v[i];
				index = i;
			}
		}
	}

	return index;
}
