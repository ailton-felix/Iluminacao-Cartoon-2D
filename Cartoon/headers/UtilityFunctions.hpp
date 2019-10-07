/*
 * UtilityFunctions.hpp
 *
 *  Created on: Nov 27, 2012
 *      Author: ailton
 */

#ifndef UTILITYFUNCTIONS_HPP_
#define UTILITYFUNCTIONS_HPP_

#include <list>
#include <vector>
#include <math.h>

#include "../headers/Point2D.hpp"
#include "../headers/Point3D.hpp"
#include "../headers/Normal.hpp"

using namespace std;

/**
 * Classe que traz algumas funções de utilidade para se oberter alguns resultados
 * de operações matemáticas.
 */
class UtilityFunctions {
public:
	UtilityFunctions();

	static Color colors[];

	/**
	 * Método que calcula a norma de um ponto 'p'.
	 */
	static double norm(Point2D p);

	/**
	 * Método que calcula a norma de um ponto 'p'(double).
	 */
	static double norm(Point3D p);

	/**
	 * Método que calcula o produto interno de dois pontos
	 * 'a' e 'b'.
	 */
	static double scalarProduct(Point2D a, Point2D b);

	/**
	 * Método que retorna um vetor formado pelo segmento 'ab'.
	 */
	static Point2D getVector(Point2D a, Point2D b);

	/**
	 * Método que calcula o pseudo-ângulo de um ponto 'pi' em relação
	 * ao ponto 'p' usando uma noçao de quadrado.
	 */
	static double calculatePseudoAngle(Point2D p, Point2D pi);

	/**
	 * Método que retorna a distância euclidiana entre os pontos 'a' e 'b'.
	 */
	static double euclideanDistance(Point2D a, Point2D b);

//	/**
//	 * Método que retorna a posição da lista 'points' do ponto mais próxima de 'p'.
//	 */
//	static int nearPoint(Point2D p, list<Point2D> points);

//	/**
//	 * Método que reordena uma lista de pontos, fazendo a mesma começar a partir da posição 'i'.
//	 */
//	static void reorderList(int i, list<Point2D> &points);

	/**
	 * Método que retorna o valor do coeficiente k do vetor resultante
	 * do produto vetorial entre 'a' e 'b' (a x b). Que são pontos do R2.
	 */
	static int coefficientOfTheVectorProduct_K(Point2D a, Point2D b);

	/**
	 * Método que coloca a lista de pontos 'points' em ordem anti-horaria no sentido da curva fechada.
	 */
	static void PutOnAntiClockwise(list<Point2D> &points);

	/**
	 * Método que cálcula a difença entre as normais "n1" e "n2", que são normais a um respectivo
	 * ponto a curva, dando assim um valor aproximado da curvatura da curva.
	 */
	static double Curvature(Point3D n1, Point3D n2);

	/**
	 * Retorna o índice tal que 'curvatures[indice] == value'.
	 */
//	static int binarySearch(vector<double> curvatures, int min, int max, double value);

	/**
	 * Função que ordena em ordem DECRESCENTE o vetor 'v' em função do atributo '_index' usando o algoritmo QuickSort.
	 **/
	static void quickSort(vector<point_tuple> &v, int ini, int end);

	/**
	 * Função que retorna o índice do menor elemento do vetor 'v'.
	 */
	static int min(vector<double> v);

};

#endif /* UTILITYFUNCTIONS_HPP_ */
