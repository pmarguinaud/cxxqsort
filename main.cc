
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <utility>


void _swap (int * a, int * b, size_t size)
{
  std::swap (*a, *b);
}

//#define SWAP(a, b, size) _swap (a, b, size)
#define SWAP(a, b, size) std::swap (*a, *b)


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
  size_t size = sizeof (ord[0]);

  char *base_ptr = (char *) &ord[0];
  const size_t max_thresh = MAX_THRESH * size;
  if (ord.size () > MAX_THRESH)
    {

      stack_t<8 * sizeof (size_t), I> stack;

      I * lo = &ord[0];
      I * hi = &lo[total_elems - 1];
      stack.push (nullptr, nullptr);
      while (stack.size ())
        {
          I * left_ptr;
          I * right_ptr;
          I * mid = lo + ((hi - lo) >> 1);
          if (cmp (mid, lo) < 0)
            SWAP (mid, lo, size);
          if (cmp (hi, mid) < 0)
            SWAP (mid, hi, size);
          else
            goto jump_over;
          if (cmp (mid, lo) < 0)
            SWAP (mid, lo, size);
        jump_over:;
          left_ptr  = lo + 1;
          right_ptr = hi - 1;
          do
            {
              while (cmp (left_ptr, mid) < 0)
                left_ptr += 1;
              while (cmp (mid, right_ptr) < 0)
                right_ptr -= 1;
              if (left_ptr < right_ptr)
                {
                  SWAP (left_ptr, right_ptr, size);
                  if (mid == left_ptr)
                    mid = right_ptr;
                  else if (mid == right_ptr)
                    mid = left_ptr;
                  left_ptr += 1;
                  right_ptr -= 1;
                }
              else if (left_ptr == right_ptr)
                {
                  left_ptr += 1;
                  right_ptr -= 1;
                  break;
                }
            }
          while (left_ptr <= right_ptr);
          if ((size_t) (right_ptr - lo) <= MAX_THRESH)
            {
              if ((size_t) (hi - left_ptr) <= MAX_THRESH)
                stack.pop (lo, hi);
              else
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= MAX_THRESH)
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
              stack.push (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
              stack.push (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }
  {
    char * const end_ptr = &base_ptr[size * (total_elems - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = std::min(end_ptr, base_ptr + max_thresh);
    char *run_ptr;
    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      if (cmp (run_ptr, tmp_ptr) < 0)
        tmp_ptr = run_ptr;
    if (tmp_ptr != base_ptr)
      SWAP ((I *)tmp_ptr, (I *)base_ptr, size);
    run_ptr = base_ptr + size;
    while ((run_ptr += size) <= end_ptr)
      {
        tmp_ptr = run_ptr - size;
        while (cmp (run_ptr, tmp_ptr) < 0)
          tmp_ptr -= size;
        tmp_ptr += size;
        if (tmp_ptr != run_ptr)
          {
            char *trav;
            trav = run_ptr + size;
            while (--trav >= run_ptr)
              {
                char c = *trav;
                char *hi, *lo;
                for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
                  *hi = *lo;
                *hi = c;
              }
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



