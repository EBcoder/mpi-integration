#include <iostream>
#include <cmath>
#include <cstdlib>
#include <mpi.h>
#include <chrono>




#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

  
int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
    
    int functionid = atoi(argv[1]);
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    int n = atoi(argv[4]);
    int intensity = atoi(argv[5]);
    int rank;
    int size;
    int delta = n / size;
    int *arr = new int[delta];
    double recv_sum = 0.0;
    double sum=0.0;
    
    
                          
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
                          
                          
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
                          
        
	/*
        for(int i=1;i<size;i++){
        
            MPI_Send(&arr, delta, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
	*/
        //for(int i=0;i<n;i++){
    for(int i=(rank * delta);i<((rank+1)*delta);i++){
	    float x = (a + (i + 0.5) * ((float)(b-a)/n));
	    if(functionid == 1){
		    sum += f1(x,intensity)*((float)(b-a)/n);


	    }else if(functionid == 2){
		    sum += f2(x, intensity)*(((float)b-a)/n);
	    }else if(functionid == 3){
		    sum += f3(x, intensity)*(((float)b-a)/n);

	    }else if(functionid == 4){
		    sum += f4(x, intensity)*(((float)b-a)/n);
	    }
    }
    if (rank == 0) {
        for(int i=1;i<size;i++){
            MPI_Status status;
            MPI_Recv(&recv_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            sum = sum + recv_sum;
        }
    }else{
	    MPI_Status status;
            MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    
    if (rank == 0)
    {
        std::cout<<sum<<std::endl;
        std::cerr<<elapsed_seconds.count()<<std::endl;
    }
    MPI_Finalize();
  
  return 0;
}
