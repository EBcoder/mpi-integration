#include <iostream>

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
    int position = 0;
    int size = 0;
    int delta = n / size;
    int *b = new int[delta];
    int recv_sum = 0;
    
    
    int functionid = atoi(argv[1]);
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    int n = atoi(argv[4]);
    int intensity = atoi(argv[5]);
    int rank = 0;
    int size = 0;
    int delta = n / size;
    int *b = new int[delta];
    int recv_sum = 0;
    
    
    
    
    
    
    
    
  return 0;
}
