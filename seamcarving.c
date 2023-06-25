#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "seamcarving.h"


void calc_col_grad(struct rgb_img *im, int * store_col_grad, char x_y, int y, int x, int col_grad){
    //printf("WIDTH-1: %d\n", im->width - 1);
    //printf("HEIGHT-1: %d\n", im->height - 1);
    //im will hold the image data
    //store_col_grad will be the address of the variable that will hold the grad for (Col) in (x or y) direction
    //x_y will tell me whether the grad is in the x or the y direction
    //x and y will be the coordinates of the pixel i want to calculate the energy for
    //col_grad will tell me the color: R is 0, G is 1, B is 2

    //printf("x_y: %c, y: %d, x: %d, col_grad: %d\n", x_y, y, x, col_grad);
    //for X
    //printf("address of store_col_grad: %d\n", store_col_grad);
    if (x_y == 'x'){
        if (x == 0){
            //printf("get_pixel: %d\n", get_pixel(im, y, x+1, col_grad));
            //printf("get_pixel: %d\n", get_pixel(im, y, im->width - 1, col_grad));
            *store_col_grad = get_pixel(im, y, x+1, col_grad) - get_pixel(im, y, im->width - 1, col_grad);
        }
        else if (x == im->width - 1){
            //printf("get_pixel: %d\n", get_pixel(im, y, 0, col_grad));
            //printf("get_pixel: %d\n", get_pixel(im, y, im->width - 2, col_grad));
            *store_col_grad = get_pixel(im, y, 0, col_grad) - get_pixel(im, y, im->width - 2, col_grad);
        }
        else{
            //printf("get_pixel: %d\n", get_pixel(im, y, x + 1, col_grad));
            //printf("get_pixel: %d\n", get_pixel(im, y, x - 1, col_grad));
            *store_col_grad = get_pixel(im, y, x + 1, col_grad) - get_pixel(im, y, x - 1, col_grad);
        }
    }
    //for Y
    if (x_y == 'y'){
        if (y == 0){
            //printf("get_pixel: %d\n", get_pixel(im, 1, x, col_grad));
            //printf("get_pixel: %d\n", get_pixel(im, im->height - 1, x, col_grad));
            *store_col_grad = get_pixel(im, 1, x, col_grad) - get_pixel(im, im->height - 1, x, col_grad);
        }
        else if (y == im->height - 1){
            //printf("get_pixel: %d\n", get_pixel(im, 0, x, col_grad));
            //printf("get_pixel: %d\n", get_pixel(im, im->height - 2, x, col_grad));
            *store_col_grad = get_pixel(im, 0, x, col_grad) - get_pixel(im, im->height - 2, x, col_grad);
        }
        else{
            //printf("get_pixel: %d\n", get_pixel(im, y+1, x, col_grad));
            //printf("get_pixel: %d\n", get_pixel(im, y-1, x , col_grad));
            *store_col_grad = get_pixel(im, y+1, x, col_grad) - get_pixel(im, y-1, x, col_grad);
        }
    }

    //printf("value in store_col_grad: %d\n", *store_col_grad);
}

void calc_energy(struct rgb_img *im, struct rgb_img **grad){

    *grad = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*grad)->height = (*im).height;
    //printf("im->height: %zu\n", (*im).height);
    //printf("grad->height: %zu\n", (*grad)->height);
    (*grad)->width = (*im).width;
    //printf("im->height: %zu\n", (*im).height);
    //printf("grad->width: %zu\n", (*grad)->width);
    (*grad)->raster = (uint8_t *)malloc(3 * (*grad)->height * (*grad)->width);

    //printf("height: %d\n",im->height);
    //printf("width: %d\n",im->width);
    for (int y_idx = 0; y_idx < im->height  ; y_idx++){
    //for (int y_idx = 0; y_idx < 1 ; y_idx++){
        for (int x_idx = 0; x_idx < im->width ; x_idx++){
        //for (int x_idx = 0; x_idx < 3 ; x_idx++){
            //for every x and y pair I will calculate:
            //      Ry, Gy, By, gradY 
            //      Rx, Gx, Bx, gradX
            //      calculate the energy of that pixel by doing: (gradX^2) + (gradY^2)
            int r_x = 0;
            //printf("&r_x: %d\n", &r_x);
            int *p_rx = &r_x;
            //printf("p_rx: %d\n", p_rx);
            calc_col_grad(im, p_rx, 'x', y_idx, x_idx, 0);
            //printf("LOOK HEREr_x: %d\n", *p_rx);
            int g_x = 0;
            int *p_gx = &g_x;
            calc_col_grad(im, p_gx, 'x', y_idx, x_idx, 1);
            //printf("LOOK HEREg_x: %d\n", *p_gx);
            int b_x = 0;
            int *p_bx = &b_x;
            calc_col_grad(im, p_bx, 'x', y_idx, x_idx, 2);
            //printf("LOOK HEREb_x: %d\n", *p_bx);

            int x_grad = ((abs(r_x)*abs(r_x)) + (abs(g_x)*abs(g_x)) + (abs(b_x)*abs(b_x))); //Δ2x(y,x)

            int r_y = 0;
            int *p_ry = &r_y;
            calc_col_grad(im, p_ry, 'y', y_idx, x_idx, 0);
            //printf("LOOK HEREr_y: %d\n", *p_ry);

            int g_y = 0;
            int *p_gy = &g_y;
            calc_col_grad(im, p_gy, 'y', y_idx, x_idx, 1);
            //printf("LOOK HEREg_y: %d\n", *p_gy);
            int b_y = 0;
            int *p_by = &b_y;
            calc_col_grad(im, p_by, 'y', y_idx, x_idx, 2);
            //printf("LOOK HEREb_y: %d\n", *p_by);

            int y_grad = ((abs(r_y)*abs(r_y)) + (abs(g_y)*abs(g_y)) + (abs(b_y)*abs(b_y))); //Δ2y(y,x)
            //printf("X_GRAD: %d\n", x_grad);
            //printf("Y_GRAD: %d\n", y_grad);

            double total = (double)x_grad + (double)y_grad;

            double total_grad = sqrt(total);

            //int total_int_grad = (int)total_grad; //do i need to change it to an int. look at how best_arr is a double
            
            //printf("total_grad: %f\n",total_grad);

            int grad_10 = (int)total_grad / 10;

            u_int8_t new_grad_10 = (u_int8_t)grad_10 ;
            //printf("grad / 10: %d\n", new_grad_10);

            //set_pixel
            set_pixel(*grad, y_idx, x_idx, new_grad_10, new_grad_10, new_grad_10);
            //printf("\n");
            
        }
    }
}

double compare(double v1, double v2, double v3){
    if(v1 < v2 && v1 < v3){
        return v1;
    }
    else if(v2 < v1 && v2 < v3){
        return v2;
    }
    else{
        return v3;
    }
}
/*
int main(){
    int one = 1;
    int *temp1 = &one;
    int two = 10;
    int *temp2 = &two;
    int three = 14;
    int *temp3 = &three;

    printf("%d",compare(temp1, temp2, temp3));
    return 0;
}
*/

double get_pixel_2(double *best_arr, int y, int x, int width){
    return (best_arr)[y * width + x]; 
    //col is the color channel (R:0, G:1,B:2)
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    //printf("HEIGHT: %d\n", (*grad).height);
    //printf("WIDTH: %d\n", (*grad).width);
    int total_size = (*grad).height * (*grad).width;
    //printf("TOTAL # OF PIXELS: %d\n", total_size);

    *best_arr = (double *)malloc(sizeof(double) * (int)(*grad).height * (int)(*grad).width);
    //copy the contents of grad into best_arr
    //copying the contents of the original image raster to the second image
    //printf("can this print? \n");
    //memcpy(best_arr,  grad->raster, total_size * sizeof(double));

    for(int i = 0; i < grad->height; i++){
    //for(int i = 0; i < 2; i++){
        for(int j = 0; j < grad->width; j++){
        //for(int j = 0; j < 2; j++){
            (*best_arr)[(i*grad->width)+j] = get_pixel(grad, i, j, 0); 
        }
    }

    //printf("this should be 15 --> %f \n", (*best_arr)[3]);
    //(*best_arr)[i*width+j]

    //starting on row 1
    for (int i = 1; i < (*grad).height ; i++){
    //for (int i = 1; i < 2 ; i++){
        for (int j = 0; j < (*grad).width ; j++){
        //for (int j = 0; j < 2 ; j++){
            //if j ==0, check (i-1, j) and (i-1, j+1)
            //else if j== width, check (i-1, j-1) and (i-1, j)
            //else check (i-1, j-1), (i-1, j), (i-1, j+1)
            double temp1;
            double temp2;
            double temp3;
            if (j==0){
                temp1 = get_pixel_2(*best_arr, i-1, j, (*grad).width);
                temp2 = get_pixel_2(*best_arr, i-1, j+1, (*grad).width);
                temp3 = 256;
            }
            else if (j== (*grad).width -1){
                temp1 = get_pixel_2(*best_arr, i-1, j-1, (*grad).width);
                temp2 = get_pixel_2(*best_arr, i-1, j, (*grad).width);
                temp3 = 256;
            }
            else{
                temp1 = get_pixel_2(*best_arr, i-1, j-1, (*grad).width);
                temp2 = get_pixel_2(*best_arr, i-1, j, (*grad).width);
                temp3 = get_pixel_2(*best_arr, i-1, j+1, (*grad).width);
            }
            //
            //printf("i: %d\n", i);
            //printf("j: %d\n", j);
            //printf("can this print? \n");
            //printf("compare: %f\n", compare(temp1, temp2, temp3));
            (*best_arr)[i * (*grad).width + j] = (*best_arr)[i * (*grad).width + j] + compare(temp1, temp2, temp3);


        }
    }

}

/*
int main(){

    struct rgb_img *im;
    read_in_img(&im, "6x5.bin");
    //print_grad(im);

    struct rgb_img *grad;

    //printf("raster: %s\n", (*im).raster);
    calc_energy(im,  &grad);
  
    print_grad(grad);

    //printf("1\n");

    double *best_arr;

    //printf("2\n");

    dynamic_seam(grad, &best_arr);

    printf("\n");

    for (int i = 0; i<5; i++){
        for (int j = 0; j<6; j++){
            printf("%f ", (best_arr)[i * grad->width + j]);
        }
        printf("\n");
    }

    //int *path;
    //recover_path(best_arr, grad->height, grad->width, &path);

    return 0;
}
*/

void recover_path(double *best, int height, int width, int **path){
    //allocate memory for path

    *path = (int *)malloc(sizeof(int)*height);

    //start at the last row, so i = height
    //      
    //find the lowest value in that row. save that index into the last value of path
    //continue checking (row - 1, index - 1), (row - 1, index), (row - 1, index+1) in each row until you reach the top
    int i = height - 1;
    int first = best[i * width]; //i = height -1 and j = 0 --> this is the first element in the last row
    //printf("first: %d\n", first);
    int *lowest = &first; 
    //printf("lowest: %d\n", *lowest);
    //int path_index = 0;
    int index = 0;
    for (int j = 1; j < width; j++){
        //int *checking;
        //*lowest = (*best)[i * width + j];
        //*checking = best[i * width + j];
        int checking = best[i * width + j];
        //printf("checking: %d\n", checking);
        ///*
        if (checking < *lowest){
            *lowest = checking;
            index = j;
        }
        //*/
        //printf("lowest: %d\n", *lowest);
        //printf("checking: %d\n", checking);

    }
    //printf("lowest: %d", *lowest);
    //int path_index = j;
    //printf("path_index: %d", path_index);
    //printf("%d", index);
    //printf("lowest: %d\n", *lowest);
    (*path)[i] = index;
    //printf("path_index: %d\n", (*path)[i]);

    //now the rest
    ///*
    for (int h = height - 2; h >= 0; h--){
        //int temp_index = (*path)[h+1];//index of previous row
        if ((*path)[h+1]==0){
            int temp1 = best[h * width + ((*path)[h+1])];
            int temp2 = best[h * width + ((*path)[h+1] + 1)];
            int temp3 = 256;
            int smaller = compare(temp1, temp2, temp3);
            if(smaller == temp1){
                (*path)[h] = (*path)[h+1];
            }
            else{(*path)[h] = (*path)[h+1] + 1;}
        }
        else if ((*path)[h+1]== width -1){
            int temp1 = best[h * width + ((*path)[h+1])];
            int temp2 = best[h * width + ((*path)[h+1] - 1)];
            int temp3 = 256;
            int smaller = compare(temp1, temp2, temp3);
            if(smaller == temp1){
                (*path)[h] = (*path)[h+1];
            }
            else{(*path)[h] = (*path)[h+1] - 1;}
        }
        else{
            int temp1 = best[h * width + ((*path)[h+1] - 1)];
            int temp2 = best[h * width + ((*path)[h+1])];
            int temp3 = best[h * width + ((*path)[h+1] + 1)];
            int smaller = compare(temp1, temp2, temp3);
            if(smaller == temp1){
                (*path)[h] = (*path)[h+1] - 1;
            }
            else if(smaller == temp2){
                (*path)[h] = (*path)[h+1];
            }
            else{(*path)[h] = (*path)[h+1] + 1;}
        }
        //(*best_arr)[i * (*grad).width + j] = (*best_arr)[i * (*grad).width + j] + compare(temp1, temp2, temp3);
        //printf("path_index: %d\n", (*path)[h]);
    }
    //*/

}
/*
int main(){

    struct rgb_img *im;
    read_in_img(&im, "6x5.bin");
    //print_grad(im);

    struct rgb_img *grad;

    //printf("raster: %s\n", (*im).raster);
    calc_energy(im,  &grad);
  
    print_grad(grad);

    //printf("1\n");

    double *best_arr;

    //printf("2\n");

    dynamic_seam(grad, &best_arr);

    printf("\n");

    int *path;
    recover_path(best_arr, grad->height, grad->width, &path);

    for (int i = 0; i<5; i++){
        printf("%d ", (path)[i]);
    }

    return 0;
}
*/

int match_list(int index, int *new_path, int height){
    int size_new_path = height * 3;
    for (int j = 0; j < size_new_path; j++){
        //if index is not in new_path
        if (index != new_path[j]){
            return 1;
        }
        else{
            return 0;
        }
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    int *new_path[3 * src->height]; //will store the indizes of the RGB
    //this is making new_path
    for (int i = 0; i<(src->height); i++){
        int beginning_of_row = i * src->width * 3;
        new_path[(3 * i)] = beginning_of_row + (3 * path[i]) + 0;
        new_path[(3 * i) + 1] = beginning_of_row + (3 * path[i]) + 1;
        new_path[(3 * i) + 2] = beginning_of_row + (3 * path[i]) + 2;
        //path[i] = i * src->height + path[i];
        //im->raster[3 * (y*(im->width) + x) + 0] = r;
        //im->raster[3 * (y*(im->width) + x) + 1] = g;
        //im->raster[3 * (y*(im->width) + x) + 2] = b;
        //printf("%d \n", (path)[i]);
    }
    //printf("\n");

    //for (int i = 0; i< 3 * src->height; i++){
        //printf("new_path %d \n", (new_path)[i]);
    //}

    create_img(dest, src->height, src->width);
    int total_change = src->height * src->width * 3;
    for(int i = 0; i<total_change; i++){
        int j = 0;
        if (match_list(i, new_path, src->height) == 1){
            (*dest)->raster[j] = src->raster[i];
            j++;
        }
        else{
            ;
        }
    }
    /*
    create_img(dest, src->height, src->width);
    for (int i = 0; i < (src->height); i++){
        for (int j = 0; j < (src->width); j++){
            while((path)[i]!= j){
                //copy them over
                (*dest)->raster[j] = src->raster[j];
                //(*im2)->raster[i] = (*im2)->raster[i]
            }
        }
    }
    */
} 

/*
int main(){

    struct rgb_img *im;
    read_in_img(&im, "6x5.bin");
    //print_grad(im);

    struct rgb_img *grad;

    //printf("raster: %s\n", (*im).raster);
    calc_energy(im,  &grad);
  
    print_grad(grad);

    //printf("1\n");

    double *best_arr;

    //printf("2\n");

    dynamic_seam(grad, &best_arr);

    printf("\n");

    int *path;
    recover_path(best_arr, grad->height, grad->width, &path);

    for (int i = 0; i<5; i++){
        printf("%d \n", (path)[i]);
    }
    //print_grad(grad);
    printf("\n");
    struct rgb_img *dest;
    remove_seam(im, &dest, path);

    struct rgb_img *new_grad;

    calc_energy(im,  &new_grad);
  
    print_grad(new_grad);
    //print_grad(grad);
    return 0;
}

*/