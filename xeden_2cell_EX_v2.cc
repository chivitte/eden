//160620 -> ya no va agregando al cluster cada punto, esto es una gran optimización (la más importante de todas). tambien tiene un número de rechazos (si una partícula ha sido rechazada Nrec veces ya ni nos molestamos en ella)
//160621 -> versión que sólo optimiza lo de no ir creciento las matrices poco a poco

//160706 -> pongo dos centros... y con sombra... hay que incluir los gráficos, no saco ni perfiles ni medidas

#include"Matrix_Red2.h"
#include"EX.h"

void Dibuja_el_Cluster(const Matrix &A,long xsize,long ysize, long Lx, long Ly, long Lm)
{
	long f=xsize/(Lx+2*Lm);
	EX_Color(0, 1.0, 0);
	EX_Rectangle(Lm*f, Lm*f, f*Lx, f*Ly);
	
	EX_Color(0, 0, 1.0);
	for (long i=1; i<=A.N1; i++)
	{
		if((int)A(i,4)==1)
			EX_Fill_Circle(A(i,2)*f, A(i,3)*f, f);	
	}
	EX_Color(1.0, 0, 0);
	for (long i=1; i<=A.N1; i++)
	{
		if((int)A(i,4)==2)
			EX_Fill_Circle(A(i,2)*f, A(i,3)*f, f);	
	}
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
	if (argc!=2){
		printf("serie que hacemos (empieza en cero, define del número aleatorio)\n");
		exit(1);
	}
	
	long Ly=330;
	long Lx=660;
	long Lm=10;
	long serie=(long)atoi(argv[1]);
	
	//inicia las gráficas
	long xsize=1360, ysize=700;
	EX_Start(0,0,xsize,ysize);
	EX_Enable_Buffer();
    long f=xsize/(Lx+2*Lm);
	EX_Color(0, 1.0, 0);
	EX_Rectangle(Lm*f, Lm*f, f*Lx, f*Ly);

	long semilla=serie;
	Rand_Open(semilla);
	
	Matrix Agregado(Lx*Ly,4);
	Table Tabla(Lx+2*Lm, Ly+2*Lm);		
	
	double tiempo=0.01; //el tiempo irá creciendo
	
	Tabla(Lx/3+Lm,Ly/2+Lm)=1;
	Tabla(2*Lx/3+Lm,Ly/2+Lm)=2;
	Agregado(1,1)=tiempo;
	Agregado(1,2)=(double)(Lx/3+Lm);
	Agregado(1,3)=(double)(Ly/2+Lm);
	Agregado(1,4)=1.;
    EX_Color(0, 0, 1.0);
    EX_Fill_Circle(Agregado(1,2)*f, Agregado(1,3)*f, f);
	Agregado(2,1)=tiempo;
	Agregado(2,2)=(double)(2*Lx/3+Lm);
	Agregado(2,3)=(double)(Ly/2+Lm);
	Agregado(2,4)=2.;
    EX_Color(1.0, 0, 0);
    EX_Fill_Circle(Agregado(2,2)*f, Agregado(2,3)*f, f);
	double cellx;
	double celly;
	
	long cell=2;
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
            
            if((int)Agregado(cell,4)==1)
                EX_Color(0, 0, 1.0);
            if((int)Agregado(cell,4)==2)
                EX_Color(1.0, 0, 0);
            EX_Fill_Circle(Agregado(cell,2)*f, Agregado(cell,3)*f, f);
            
//             EX_Clear();
// 			Dibuja_el_Cluster(Agregado,xsize,ysize,Lx,Ly,Lm);
//             EX_Flush();
		}
	}while(cellx>Lm && celly>Lm && cellx<Lx+Lm && celly<Ly+Lm);
	EX_Read_Key();
}
