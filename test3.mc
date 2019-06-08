int main()
{
  float a;
  float c;
  char b;
  int x,y,z;
  a = 1.24;
  c = 2.46;
  b = 'x';
  x = 100;
  y = 200; 
  a = sum(a,c);
  write(a);  
  z= sum_i(x,y);
  a += c;
  write(a);
  lf();
  write(b);
}
float sum(float n, float m)
{
  return n+m;
}

int sum_i(int k, int f)
{
  return k+f;
}
