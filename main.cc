
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <utility>




template <typename V, typename C>
void my_quicksort2 (typename V::iterator b, typename V::iterator e, C cmp)
{
  using itt = typename V::iterator;
  

  class stack_t
  {
  public:
    stack_t () 
    {
      top = &arr[0];
    }
    void push (itt lo, itt hi)
    {
      top->lo = lo; 
      top->hi = hi; 
      top++;
    }
    void pop (itt & lo, itt & hi)
    {
      top--;
      lo = top->lo;
      hi = top->hi;
    }
    bool empty () const
    {
      return arr >= top;
    }
    size_t size () const
    {
      return top - &arr[0];
    }
  private:
    struct node_t
    {
      itt lo;
      itt hi;
    };
    node_t arr[64];
    node_t * top;
  };


  const int max = 4;

  if (e - b == 0)
    return;

  if (e - b > max)
    {

      stack_t stack;

      itt lo = b;
      itt hi = e - 1;
      stack.push (e, e);
      while (stack.size ())
        {
          itt le;
          itt ri;
          itt mid = lo + (hi - lo) / 2;
          if (cmp (*mid, *lo) < 0)
            std::swap (*mid, *lo);
          if (cmp (*hi, *mid) < 0)
            std::swap (*mid, *hi);
          else
            goto jump_over;
          if (cmp (*mid, *lo) < 0)
            std::swap (*mid, *lo);
        jump_over:;
          le  = lo + 1;
          ri = hi - 1;
          do
            {
              while (cmp (*le, *mid) < 0)
                le++;
              while (cmp (*mid, *ri) < 0)
                ri--;
              if (le < ri)
                {
                  std::swap (*le, *ri);
                  if (mid == le)
                    mid = ri;
                  else if (mid == ri)
                    mid = le;
                  le++;
                  ri--;
                }
              else if (le == ri)
                {
                  le++;
                  ri--;
                  break;
                }
            }
          while (le <= ri);
          if ((ri - lo) <= max)
            {
              if ((hi - le) <= max)
                stack.pop (lo, hi);
              else
                lo = le;
            }
          else if ((hi - le) <= max)
            hi = ri;
          else if ((ri - lo) > (hi - le))
            {
              stack.push (lo, ri);
              lo = le;
            }
          else
            {
              stack.push (le, hi);
              hi = ri;
            }
        }
    }
  {
    itt end = e - 1;
    itt tmp = b;
    itt thr = std::min (end, b + max);
    itt run;
    for (run = tmp + 1; run <= thr; run++)
      if (cmp (*run, *tmp) < 0)
        tmp = run;
    if (tmp != b)
      std::swap (*tmp, *b);
    run = b + 1;
    while (++run <= end)
      {
        tmp = run - 1;
        while (cmp (*run, *tmp) < 0)
          tmp--;
        tmp++;
        if (tmp != run)
          {
            itt x1 = tmp;
            itt x2 = run;
            typename V::value_type v2 = *x2;
            for (itt x = x2; x > x1; x--)
              *x = *(x - 1);
            *x1 = v2;
          }
      }
  }

}




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
    fprintf (fp, "%8d\n", vec[i]);
  fclose (fp);
}

int main (int argc, char * argv[])
{

  int n = atoi (argv[1]);

  srand (0);

  std::vector<int> vec1 (n);

  for (int i = 0; i < n; i++)
    vec1[i] = rand () % n;

  std::vector<int> vec2 = vec1;

  my_quicksort1 (&vec1[0], n, sizeof (vec1[0]), compare);

  std::vector<int>::iterator b2 = vec2.begin ();
  std::vector<int>::iterator e2 = vec2.end ();

  auto cmp = [&] (int a, int b) { return compare (&a, &b); };

  my_quicksort2<std::vector<int>,decltype (cmp)> (vec2.begin (), vec2.end (), cmp);

  pr (vec1, "vec1.txt");
  pr (vec2, "vec2.txt");

  return 0;
}



