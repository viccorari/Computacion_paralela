#include "iostream"
#include "string"
#include <time.h> 
#include <stdio.h> 
#include <stdlib.h> 
using namespace std;

int ** & init_mat(int m, int n) {
	static int ** matriz;
	matriz = new int * [m];
	for (int i = 0;i < m;i++) {
		matriz[i] = new int[n];
	}
	return matriz;
}

void print_mat(int ** & mat, int m, int n) {
	for (int i = 0;i < m; i++) {
		for (int j = 0;j < n;j++) {
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
}

void mat_zeros(int ** & mat, int m, int n) {
	for (int i = 0;i < m; i++) {
		for (int j = 0;j < n;j++) {
			mat[i][j] = 0;
		}
	}
}

void mat_rnd(int ** & mat, int m, int n, int max) {
	for (int i = 0;i < m; i++) {
		for (int j = 0;j < n;j++) {
			mat[i][j] = rand()%max;
		}
	}
}

void mul_mat(int ** & res, int ** & a, int ** & b, int m, int n) {
	for (int i = 0;i < m; i++) {
		for (int j = 0;j < n;j++) {
			for (int k = 0;k < m;k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}


void mul_mat2(int ** & res, int ** & a, int ** & b, int m, int n) {
	for (int i = 0;i < m; i++) {
		for (int j = 0;j < n;j++) {
			for (int k = 0;k < n;k++) {
				res[j][i] += a[j][k] * b[k][i];
			}
		}
	}
}


void mul_block_m (int ** & C, int ** & A, int ** & B, int n, int bsize)
{
	int i, j, k, kk, jj;
	double sum;
	int en = bsize * (n/bsize);

	for (kk = 0; kk < en; kk += bsize) {
			for (jj = 0; jj < en; jj += bsize) {
			for (i = 0; i < n; i++) {
				for (j = jj; j < jj + bsize; j++) {
					sum = C[i][j];
					for (k = kk; k < kk + bsize; k++) {
						sum += A[i][k]*B[k][j];
					}
					C[i][j] = sum;
				}
			}
		}
	}
}

bool compare (int ** & A, int ** & B, int n){
	for (int i=0;i<n;i++)
		for (int j=0;j<n;j++)
			if (A[i][j] != B[i][j]) return false;
	return true;
}

int main(){
	cout << "Creando..."<<endl;
	int m = 1000;
	int n = 1000;
	int max = 100;

	int ** a = init_mat(m, n);
	int ** b = init_mat(m, n);

	mat_rnd(a, m, n, max);
	mat_rnd(b, m, n, max);

	//print_mat(a, m, n);
	//print_mat(b, m, n);
	cout << "Ejecutando..." << endl;
	clock_t t_ini, t_fin;
	double secs;
	
	t_ini = clock();

	int ** r = init_mat(m, n);
	mat_zeros(r, m, n);
	mul_mat(r, a, b, m, n);
	//print_mat(r, m, n);

	t_fin = clock();

	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("Tiempo por filas: %.16g milisegundos\n", secs * 1000.0);
	
	
	t_ini = clock();

	int ** r2 = init_mat(m, n);
	mat_zeros(r2, m, n);
	mul_mat2(r2, a, b, m, n);
	//print_mat(r2, m, n);

	t_fin = clock();
	
	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("Tiempo por columnas: %.16g milisegundos\n", secs * 1000.0);
	

	printf("Por Bloques\n");
	
	t_ini = clock();

	int ** r5 = init_mat(m, n);
	mat_zeros(r5, m, n);
	mul_block_m(r5, a, b, m, 25);
	//print_mat(r5, m, n);
	t_fin = clock();
	
	if(compare(r,r5,n)) cout<<"Correcto\n"; 
	else cout<<"InCorrecto\n";
	
	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	printf("%d,%.16g\n",25, secs * 1000.0);
	
	cout << "Fin de ejecucion" << endl;
	
    return 0;
}
