
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define SWAP(a, b, size)                                                      \
  do                                                                              \
    {                                                                              \
      size_t __size = (size);                                                      \
      char *__a = (a), *__b = (b);                                              \
      do                                                                      \
        {                                                                      \
          char __tmp = *__a;                                                      \
          *__a++ = *__b;                                                      \
          *__b++ = __tmp;                                                      \
        } while (--__size > 0);                                                      \
    } while (0)

#define MAX_THRESH  4


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
   struct stack_node
   {
     char *lo;
     char *hi;
   };

#define STACK_SIZE        (CHAR_BIT * sizeof (size_t))
#define PUSH(low, high)        ((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define        POP(low, high)        ((void) (--top, (low = top->lo), (high = top->hi)))
#define        STACK_NOT_EMPTY        (stack < top)

      char *lo = base_ptr;
      char *hi = &lo[size * (total_elems - 1)];
      stack_node stack[STACK_SIZE];
      stack_node *top = stack;
      PUSH (NULL, NULL);
      while (STACK_NOT_EMPTY)
        {
          char *left_ptr;
          char *right_ptr;
          char *mid = lo + size * ((hi - lo) / size >> 1);
          if (cmp ((void *) mid, (void *) lo) < 0)
            SWAP (mid, lo, size);
          if (cmp ((void *) hi, (void *) mid) < 0)
            SWAP (mid, hi, size);
          else
            goto jump_over;
          if (cmp ((void *) mid, (void *) lo) < 0)
            SWAP (mid, lo, size);
        jump_over:;
          left_ptr  = lo + size;
          right_ptr = hi - size;
          do
            {
              while (cmp ((void *) left_ptr, (void *) mid) < 0)
                left_ptr += size;
              while (cmp ((void *) mid, (void *) right_ptr) < 0)
                right_ptr -= size;
              if (left_ptr < right_ptr)
                {
                  SWAP (left_ptr, right_ptr, size);
                  if (mid == left_ptr)
                    mid = right_ptr;
                  else if (mid == right_ptr)
                    mid = left_ptr;
                  left_ptr += size;
                  right_ptr -= size;
                }
              else if (left_ptr == right_ptr)
                {
                  left_ptr += size;
                  right_ptr -= size;
                  break;
                }
            }
          while (left_ptr <= right_ptr);
          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
                POP (lo, hi);
              else
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
              PUSH (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
              PUSH (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }
  {
    char *const end_ptr = &base_ptr[size * (total_elems - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = std::min(end_ptr, base_ptr + max_thresh);
    char *run_ptr;
    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      if (cmp ((void *) run_ptr, (void *) tmp_ptr) < 0)
        tmp_ptr = run_ptr;
    if (tmp_ptr != base_ptr)
      SWAP (tmp_ptr, base_ptr, size);
    run_ptr = base_ptr + size;
    while ((run_ptr += size) <= end_ptr)
      {
        tmp_ptr = run_ptr - size;
        while (cmp ((void *) run_ptr, (void *) tmp_ptr) < 0)
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



