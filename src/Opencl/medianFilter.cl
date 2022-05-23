void sort(int *a, int *b, int *c) {
   int swap;
   if(*a > *b) {
      swap = *a;
      *a = *b;
      *b = swap;
   }
   if(*a > *c) {
      swap = *a;
      *a = *c;
      *c = swap;
   }
   if(*b > *c) {
      swap = *b;
      *b = *c;
      *c = swap;
   }
}


__kernel void median_filter(__global int* inImage, __global int* output, int width, int height)
{
    int workItemNum = get_global_id(0);
    int workGroupNum = get_group_id(0);

    float arg1 = *inImage;
    uint global_addr = workItemNum;

    int posx = workItemNum;
    int posy = workGroupNum;

    uint i = width*posy + workItemNum%width;
    //printf("i = %d\n", i);
    
    
    //printf("hello world\n");

    //printf("WorkItem: %d WorkGroup: %d\n", posx, posy);
    if ((posx%width == 0) || (posy == 0) || (posx == width-1) || (posy == height-1)) {
        //printf("WorkItem: %d WorkGroup: %d\n", posx, posy);
        output[i] = inImage[i];

    }
    else {
        int pix0, pix1, pix2, pix3, pix4, pix5, pix6, pix7, pix8;

        pix0 = inImage[i - 1 - width];
        pix1 = inImage[i - width];
        pix2 = inImage[i + 1 - width]; 
        pix3 = inImage[i - 1];
        pix4 = inImage[i];
        pix5 = inImage[i + 1];
        pix6 = inImage[i - 1 + width];
        pix7 = inImage[i + width];
        pix8 = inImage[i + 1 + width];

        //printf("i = %d\n", i);
        //printf("pixels  %d  %d   %d  %d  %d  %d  %d  %d  %d  %d   %d   %d  %d  %d  %d  %d  %d  %d  %d\n", i, i-1-width, i-width, i+1-width, i-1, i, i+1, i-1+width, i+width, i+1+width, pix0, pix1, pix2, pix3, pix4, pix5, pix6, pix7, pix8);

        //sort the rows
        sort( &(pix0), &(pix1), &(pix2) );
        sort( &(pix3), &(pix4), &(pix5) );
        sort( &(pix6), &(pix7), &(pix8) );
        //sort the columns
        sort( &(pix0), &(pix3), &(pix6) );
        sort( &(pix1), &(pix4), &(pix7) );
        sort( &(pix2), &(pix5), &(pix8) );
        //sort the diagonal
        sort( &(pix0), &(pix4), &(pix8) );
        // median is the the middle value of the diagonal
        output[i] = pix4;
        //printf("Pixel 4: %d     %d\n", output[i],i);
    }

}