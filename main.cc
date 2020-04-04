
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <utility>


#define MAX_THRESH  4


template <size_t N, typename T>
class stack_t
{
public:
  stack_t () 
  {
    top = &arr[0];
  }
  void push (T * lo, T * hi)
  {
    top->lo = lo; 
    top->hi = hi; 
    top++;
  }
  void pop (T * & lo, T * & hi)
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
    T * lo;
    T * hi;
//  std::vector<T>::iterator lo;
//  std::vector<T>::iterator hi;
  };
  node_t arr[N];
  node_t * top;
};

template <typename I, typename C>
void my_quicksort2 (std::vector<I> & ord, C cmp_)
{
  if (ord.size () == 0)
    return;

  auto cmp = [&cmp_] (const void * _a, const void * _b)
             {
               I a = *((I *)_a);
               I b = *((I *)_b);
               return cmp_ (a, b);
             };

  size_t total_elems = ord.size ();

  if (ord.size () > MAX_THRESH)
    {

      stack_t<8 * sizeof (size_t), I> stack;

      I * lo = &ord[0];
      I * hi = &lo[total_elems - 1];
      stack.push (nullptr, nullptr);
      while (stack.size ())
        {
          I * le;
          I * right_ptr;
          I * mid = lo + (hi - lo) / 2;
          if (cmp (mid, lo) < 0)
            std::swap (*mid, *lo);
          if (cmp (hi, mid) < 0)
            std::swap (*mid, *hi);
          else
            goto jump_over;
          if (cmp (mid, lo) < 0)
            std::swap (*mid, *lo);
        jump_over:;
          le  = lo + 1;
          right_ptr = hi - 1;
          do
            {
              while (cmp (le, mid) < 0)
                le++;
              while (cmp (mid, right_ptr) < 0)
                right_ptr--;
              if (le < right_ptr)
                {
                  std::swap (*le, *right_ptr);
                  if (mid == le)
                    mid = right_ptr;
                  else if (mid == right_ptr)
                    mid = le;
                  le++;
                  right_ptr--;
                }
              else if (le == right_ptr)
                {
                  le++;
                  right_ptr--;
                  break;
                }
            }
          while (le <= right_ptr);
          if ((right_ptr - lo) <= MAX_THRESH)
            {
              if ((hi - le) <= MAX_THRESH)
                stack.pop (lo, hi);
              else
                lo = le;
            }
          else if ((hi - le) <= MAX_THRESH)
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - le))
            {
              stack.push (lo, right_ptr);
              lo = le;
            }
          else
            {
              stack.push (le, hi);
              hi = right_ptr;
            }
        }
    }
  {
    I * const end = &ord[total_elems-1];
    I * tmp = &ord[0];
    I * thr = std::min (end, &ord[0] + MAX_THRESH);
    I * run;
    for (run = tmp + 1; run <= thr; run++)
      if (cmp (run, tmp) < 0)
        tmp = run;
    if (tmp != &ord[0])
      std::swap (*tmp, ord[0]);
    run = &ord[0] + 1;
    while (++run <= end)
      {
        tmp = run - 1;
        while (cmp (run, tmp) < 0)
          tmp--;
        tmp++;
        if (tmp != run)
          {
            I * x1 = tmp;
            I * x2 = run;
            I v2 = *x2;
            for (I * x = x2; x > x1; x--)
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
  my_quicksort2 (vec2, [&] (int a, int b) { return compare (&a, &b); });

  pr (vec1, "vec1.txt");
  pr (vec2, "vec2.txt");


  return 0;
}



