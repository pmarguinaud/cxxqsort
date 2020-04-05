

#include "quicksort.h"

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <utility>
#include <sys/time.h>

extern "C" void my_quicksort1 (void * const, size_t, size_t, int (*)(const void *, const void *));


int compare (const void * _a, const void * _b)
{
  int a = *((int *)_a);
  int b = *((int *)_b);
  return a < b ? -1 : + 1;
}

static void pr (const std::vector<int> & vec, const char * file)
{
  FILE * fp = fopen (file, "w");
  for (int i = 0; i < vec.size (); i++)  
    fprintf (fp, "%12d\n", vec[i]);
  fclose (fp);
}


double ctime ()
{
  struct timeval tv; 
  struct timezone tz;

  gettimeofday (&tv, &tz);

  return double (tv.tv_sec) + double (tv.tv_usec) * 1e-6;
}

int main (int argc, char * argv[])
{

  int n = atoi (argv[1]);

  srand (0);

  std::vector<int> vec1 (n);

  for (int i = 0; i < n; i++)
    vec1[i] = rand () % n;

  std::vector<int> vec2 = vec1;
  std::vector<int> vec3 = vec1;


  double t1a = ctime ();
  my_quicksort1 (&vec1[0], n, sizeof (vec1[0]), compare);
  double t1b = ctime ();

  auto cmp = [] (int a, int b) { return a < b; };

  double t2a = ctime ();
  quicksort (vec2.begin (), vec2.end (), cmp);
  double t2b = ctime ();

  double t3a = ctime ();
  std::sort (vec3.begin (), vec3.end (), cmp);
  double t3b = ctime ();

  pr (vec1, "vec1.txt");
  pr (vec2, "vec2.txt");

  printf (" t1 = %12.4lf\n", t1b-t1a);
  printf (" t2 = %12.4lf\n", t2b-t2a);
  printf (" t3 = %12.4lf\n", t3b-t3a);


  return 0;
}



