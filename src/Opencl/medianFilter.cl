
__kernel void median_filter(__global float* inImage, __global float* outImage, int width, int height)
{
    int workItemNum = get_global_id(0);
    int workGroupNum = get_group_id(0);

    float arg1 = *inImage;
    uint global_addr = workItemNum;

    int posx = workItemNum;
    int posy = workGroupNum;

    //int max_x = width;
    //int max_y = height;
    
    //printf("hello world\n");

    //printf("WorkItem: %d WorkGroup: %d\n", posx, posy);
    if ((posx == 0) || (posy == 0) || (posx%width == width-1) || (posy == height-1)) {
        //printf("WorkItem: %d WorkGroup: %d\n", posx, posy);
        outImage[posx][posy] = inImage[posx][posy];

    }

}