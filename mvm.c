#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define PI 3.14159265

int read_mtx_metadata(FILE *f, int *r, int *c, int *nonzs)
{
    int line_len = 1024;
    char line[line_len];
    int num_items_read;

    /* get a line, try to apply "3 numbers" pattern to
     * it. If there were 3 numbers, make sure it's not
     * a comment. If there were not and the line was
     * either comment or empty one, get next line.
     */
    do
    {
        fgets(line, line_len, f);
        num_items_read = sscanf(line, "%d %d %d", r, c, nonzs);

    } while (((line[0] != '%') || (line[0] != ' '))&& (num_items_read != 3));

    return 0;
}

int main(int argc, char *argv[])
{
    FILE *f;
    int mtx_r, mtx_c, nonzs, i;
    int curr_r, curr_c;
    double result_row;
    double curr_val;
    double *mtx;
    double *vtr;
    double *result;
    double sum_of_sq;
    double l2;
    clock_t start, end;
    double time_delta;
    double flops;
    double ops_count;
    double clocking_loop;

    if (argv[1] == NULL){
        printf("No address for data file specified\n");
        return 0;
    }

    if ((f = fopen(argv[1], "r")) == NULL) {
        printf("Cannot open the data file, error reported is '%s'\n", strerror(errno));
        return 0;
    }
    read_mtx_metadata(f, &mtx_r, &mtx_c, &nonzs);
    mtx = (double* ) malloc (mtx_r*mtx_c*sizeof(double));
    vtr = (double* ) malloc (mtx_r*sizeof(double));
    result = (double* ) malloc (mtx_r * sizeof(double));

    //populating matrix with values from file
    for (i=0; i<nonzs; i++)
    {
        fscanf(f, "%d %d %lg\n", &curr_r, &curr_c, &curr_val);
        curr_r--;
        curr_c--;
        mtx[curr_r*mtx_c + curr_c] = curr_val;
    }

//  populating vector with hardcoded values
    for (i = 0; i < mtx_c; i++)
    {
        vtr[i] = sin((2*PI*i)/182);
    }
//    this is for debugging purposes
//    vtr[0] = 0;
//    vtr[1] = 10;

    //multiplying
    clocking_loop = 100000;
    start = clock();
    for (int cl = 0; cl < clocking_loop; cl++)
    {
        for (curr_r = 0; curr_r < mtx_r; curr_r++)
        {
            result_row = 0;

            for (curr_c = 0; curr_c < mtx_c; curr_c++)
            {
                result_row += mtx[curr_r*mtx_c + curr_c]*vtr[curr_c]; //here we have 2 floating operations
            }

            result[curr_r] = result_row;

        }
        end = clock();
        time_delta = ((double)(end - start))/CLOCKS_PER_SEC;
        ops_count = 2.0;
        flops = (ops_count*mtx_c*mtx_r)/time_delta*clocking_loop;
    }



    //calculate l2 norm
    sum_of_sq = 0;
    for (curr_r = 0; curr_r < mtx_r; curr_r++)
    {
        sum_of_sq += result[curr_r]*result[curr_r];
    }
    l2 = sqrt(sum_of_sq);

    //print out results
    printf("Multiplication result has l2 norm of %5.15g\n", l2);
    printf("Speed of multiplication was %2.3g flops\n", flops);

    if (f !=stdin) fclose(f);

    free(mtx);
    free(vtr);
    free(result);

    return 0;
}
