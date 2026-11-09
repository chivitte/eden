//160620 -> ya no va agregando al cluster cada punto, esto es una gran optimización (la más importante de todas). tambien tiene un número de rechazos (si una partícula ha sido rechazada Nrec veces ya ni nos molestamos en ella, y ya está bien puesto)
//160620 -> añado el cálculo de los perfiles
// versión sin tener en cuenta las activas
// no sacamos los perfiles, sacamos directamente las medidas
//realamente puede hacer de todo, incluso comprimir los perfiles y borrarlos al final

// #include"Matrix_Red2.h"
#include"Matrix.h"
#include"EX.h"
//////////////////////////////////////////////////////////
void Dibuja_el_Cluster(const Matrix &A,long xsize,long ysize, long L, long Lt, long cell)
{
	double factor=2.;
	EX_Clear();
	EX_Color(0, 1.0, 0);
	EX_Rectangle(xsize/2-Lt*factor, ysize/2-Lt*factor, factor*2*Lt, factor*2*Lt);
	EX_Circle(xsize/2, ysize/2, factor*Lt);
	EX_Color(1.0, 0, 0);
	EX_Circle(xsize/2, ysize/2, factor*L);
	EX_Color(0, 0, 1.0);
	for (long i=1; i<=cell; i++)
		EX_Fill_Circle(xsize/2+A(i,2)*factor, ysize/2+A(i,3)*factor, A(i,4)*factor);
	EX_Flush();
}

/////////////Modelo de Eden - sin contar partículas activas////////////////////////////////////
int Comprueba_que_Cabe(const Matrix &A, const Table &T, double cellx, double celly, long ix, long iy, double radio)
{
	long vec=4; //hasta donde comprueba si se solapan? ¡No hace falta más!
	for(long jx=ix-vec; jx<=ix+vec; jx++)
	{
		for(long jy=iy-vec; jy<=iy+vec; jy++)
		{
			if(T(jx,jy)!=0)
			{
				if(Sqr(cellx-A(T(jx,jy),2))+Sqr(celly-A(T(jx,jy),3))<Sqr(radio+A(T(jx,jy),4)-0.000001))
				{
					return 0;
				}
			}
		}
	}
	return 1;
}
/////////////A por ello - tiene en cuenta todas las partículas del agregado////////////////////////////////////
int main(int argc, char *argv[])
{
	if (argc!=2){
		printf("1: Semilla de los números aleatorios\n");
		exit(1);
	}
	long L=150; //tamaño de la plaqueta
	long semilla=(long)atoi(argv[1]);
	
	long Lt=L+10;// tamaño del marco
	long Lmax2=Sqr(L);
	long ct=Lt+1;//centro de la tabla

	Rand_Open(semilla);
	
	Matrix Agregado(Lmax2,4);
	Table Tabla(2*Lt+1);		
	
	double tiempo=0.0; //el tiempo irá creciendo
	double cellx=0;
	double celly=0;
	Tabla(ct,ct)=1; //primera partícula del cluster
	double radio=1;//Rand(1,2);
	Agregado(1,4)=radio;
	long cell=1;
	
	//inicia las gráficas
	long xsize=700, ysize=700;
	EX_Start(100,0,xsize,ysize);
	EX_Enable_Buffer();
	Dibuja_el_Cluster(Agregado,xsize,ysize,L,Lt,cell);
	EX_Read_Key();
	do
	{
		if(cell<=100)
			tiempo+=0.01/(double)cell;
		else
			tiempo+=1.0/(double)cell; 
		
		long madre=Rand_I(1,cell); //elige una partícula al azar de las activas
		double alpha=Rand(0.,2.*M_PI); //elige un ángulo al azar del cluster
// 		radio=Rand(1,2);
		
		cellx=Agregado(madre,2)+(Agregado(madre,4)+radio)*cos(alpha);
		celly=Agregado(madre,3)+(Agregado(madre,4)+radio)*sin(alpha);
		long ix=floor(cellx)+ct;
		long iy=floor(celly)+ct;
		
		long CC=Comprueba_que_Cabe(Agregado,Tabla,cellx,celly,ix,iy,radio);
		if(CC==1)
		{
			cell++;
			Agregado(cell,1)=tiempo;
			Agregado(cell,2)=cellx;
			Agregado(cell,3)=celly;
			Agregado(cell,4)=radio;
			Tabla(ix,iy)=cell;
			Dibuja_el_Cluster(Agregado,xsize,ysize,L,Lt,cell);
		}
	}while(Sqr(cellx)+Sqr(celly)<=Lmax2);
	Rand_Close();
	EX_Read_Key();
}
