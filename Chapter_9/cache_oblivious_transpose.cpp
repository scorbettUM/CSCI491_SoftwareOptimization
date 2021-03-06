#include "../Clock.hpp"
#include <iostream>

constexpr unsigned int BLOCK_SIZE = 4;

static void buffered_helper(double*__restrict const dest, const double*__restrict const source, const unsigned long R, const unsigned long C, const unsigned long r_start, const unsigned long r_end, const unsigned long c_start, const unsigned long c_end){
  unsigned long r_span = r_end - r_start;
  unsigned long c_span = c_end - c_start;

  if(r_span <= BLOCK_SIZE && c_span <= BLOCK_SIZE){
    for(unsigned long r=r_start; r<r_end; ++r)
      for(unsigned long c=c_start; c<c_end; ++c)
        dest[c*R + r] = source[r*C + c];
  }
  else {
    if(r_span > c_span){
      buffered_helper(dest, source, R, C, r_start, r_start+r_span/2, c_start, c_end);
      buffered_helper(dest, source, R, C, r_start+r_span/2, r_end, c_start, c_end);
    }
    else {
      buffered_helper(dest, source, R, C, r_start, r_end, c_start, c_start+c_span/2);
      buffered_helper(dest, source, R, C, r_start, r_end, c_start+c_span/2, c_end);
    }
  }
}

__attribute__((noinline))
void recursive_transpose(double*__restrict dest, double*__restrict source, const unsigned long r, const unsigned long c){
  buffered_helper(dest, source, r, c, 0, r, 0, c);
}

int main(){
  unsigned long R = 1<<10;
  unsigned long C = 1<<12;

  double * matrix = new double[R*C];
  for(unsigned long i=0; i<R*C; ++i){
    matrix[i] = i;
  }

  Clock c;
  double * result = new double[C*R];
  recursive_transpose(result, matrix, R, C);
  c.ptock();

  return 0;
}
