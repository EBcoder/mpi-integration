#include <iostream>
#include <ratio>
#include <chrono>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

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
    int recv_sum = 0;
    int index =0;
    int indexBegin, indexEnd;
    double sum=0.0;
    
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    MPI_Status status;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank != 0)
    {
        
            MPI_Recv(&indexBegin, 1,MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&indexEnd,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
            for(int i=size;i<n;i++){
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
            MPI_Send(&sum, 1,MPI_DOUBLE_PRECISION,0, 0, MPI_COMM_WORLD);
        
        
    }
    else
    {
        for(int i=1; i < size; ++i)
        {            // buff  count dt des tag comm
            MPI_Send(&index,1,MPI_INT,i,0,MPI_COMM_WORLD);
            if(index+delta >= n)index = n;
                   // buff  count dt des tag comm
            MPI_Send(&index,1,MPI_INT,i,0,MPI_COMM_WORLD);
        }
        for(int i=0; i < size;)
        {           // buff  count dt source tag comm status
            MPI_Recv(&recv_sum,1, MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
            sum += recv_sum;
            
            if(index+delta >= n)
            {
                MPI_Send(&n,1,MPI_INT,status.MPI_SOURCE, 0,MPI_COMM_WORLD);
                ++i;
            }
            else
            {
                MPI_Send(&index, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                if(index+delta >= n)index = n;
                MPI_Send(&index,1,MPI_INT,status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            }
            //std::cout<<sum<<std::endl;
        }
        
    }

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    
    if (rank != 0)
    {
        std::cout<<sum<<std::endl;
        std::cerr<<elapsed_seconds.count()<<std::endl;
    }
    MPI_Finalize();
    
    
  return 0;
}
