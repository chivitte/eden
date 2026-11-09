// intento de poner varios nucleos de celulas con la idea de poner a cada uno un color y ver si se forman celdas de voronoi

// por ahora no conseguido :-(


#include"Matrix.h"
#include"EX.h"

void Dibuja_el_Cluster(const Matrix &A,long xsize,long ysize, long Lx, long Ly, long Tipos)
{
	EX_Clear();
	
	long f=xsize/Lx;
// 	EX_Color(0, 1.0, 0);
// 	EX_Rectangle(Lm*f, Lm*f, f*Lx, f*Ly);
	
	EX_Color(0, 0, 1.0);
	for (long i=1; i<=A.N1; i++)
	{
// 		EX_Color(A(i,4)/Tipos,0,0);
		EX_Fill_Circle(A(i,2)*f, A(i,3)*f, f);	
	}
	EX_Flush();
}

int Comprueba_que_Cabe(const Matrix &A, const Table &T, double cellx, double celly, long ix, long iy)
{
	long vec=2; //hasta donde comprueba si se solapan? ¡No hace falta más!
	for(long jx=ix-vec; jx<=ix+vec; jx++)
		for(long jy=iy-vec; jy<=iy+vec; jy++)
			if(T(jx,jy)!=0)
				if(sqrt(Sqr(cellx-A(T(jx,jy),2))+Sqr(celly-A(T(jx,jy),3)))<2.0-0.000001)
					return 0;
				return 1;
}

int main(int argc, char *argv[])
{
	if (argc!=3){
		printf("serie que hacemos (empieza en cero, define del número aleatorio) \nnúmero de semillas aleatorias que ponemos \n");
		exit(1);
	}
	
	long Ly=330;
	long Lx=660;
// 	long Lm=10;
	long serie=(long)atoi(argv[1]);
	long Tipos=(long)atoi(argv[2]);
	
	//inicia las gráficas
	long xsize=1360, ysize=700;
	EX_Start(0,0,xsize,ysize);
	EX_Enable_Buffer();
	
	
	long semilla=serie;
	Rand_Open(semilla);
	
	Matrix Agregado(Lx*Ly,4);
	Table Tabla(Lx, Ly);		
	
	double tiempo=0.01; //el tiempo irá creciendo
	
	for(long i=1; i<=Tipos; i++)
	{
		Agregado(i,1)=tiempo;
		Agregado(i,2)=Rand_I(0,Lx);
		Agregado(i,3)=Rand_I(0,Ly);
		Agregado(i,4)=i;
		Tabla(Agregado(i,2),Agregado(i,3))=i;
	}
	double cellx;
	double celly;
	
	long cell=Tipos;
	do
	{
		
		tiempo+=1.0/(double)cell; 
		
		long madre=Rand_I(1,cell); //elige una partícula al azar de las activas
		double alpha=Rand(0.,2.*M_PI); //elige un ángulo al azar del cluster
		
		cellx=Agregado(madre,2)+2.0*cos(alpha);
		celly=Agregado(madre,3)+2.0*sin(alpha);
		long ix=floor(cellx);
		long iy=floor(celly);
		
		long CC=Comprueba_que_Cabe(Agregado,Tabla,cellx,celly,ix,iy);
		
		if(CC==1)
		{
			cell++;
			Agregado(cell,1)=tiempo;
			Agregado(cell,2)=cellx;
			Agregado(cell,3)=celly;
			Agregado(cell,4)=Agregado(madre,4);
			Tabla(ix,iy)=cell;
			Dibuja_el_Cluster(Agregado,xsize,ysize,Lx,Ly,Tipos);
			//             Delay(0.02);
		}
	}while(1);
	EX_Read_Key();
}
