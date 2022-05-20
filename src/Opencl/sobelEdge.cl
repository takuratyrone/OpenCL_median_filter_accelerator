__constant sampler_t image_sampler = CLK_NORMALIZED_COORDS_FALSE
| CLK_ADDRESS_CLAMP_TO_EDGE;

__kernel void sobel_filter_kernel(__read_only image2d_t iimage,
                                __write_only image2d_t oimage,
                                __global float *filter_x_grad,
                                __global float *filter_y_grad,
                                int windowSize)
{
    unsigned int x = get_global_id(0);
    unsigned int y = get_global_id(1);
    int halfWindow = windowSize/2;
    float4 pixelValue;
    float gradientX = 0.0f;
    float gradientY = 0.0f;
    float computedFilter = 0.0f;
    int i, j, ifilter, jfilter;
    float computedGradient;
    
    for(i=-halfWindow, ifilter=0; i<=halfWindow; i++, ifilter++)
    {
        for(j = -halfWindow, jfilter=0;j<=halfWindow; j++,jfilter++)
        {
            pixelValue = read_imagef(iimage, image_sampler, (int2)(x+i, y+j));
            //printf("Pixel value: %i\n",pixelValue.y);
            gradientX += filter_x_grad[ifilter*windowSize+jfilter] *pixelValue.x;
            gradientY += filter_y_grad[ifilter*windowSize+jfilter] *pixelValue.y;
        }
    }
    //gradient and gradient is the image gradient in X and Y axes.
    //Now compute the gradient magnitude
    printf("Gradient: %f\n", gradientX);
    computedGradient = sqrt(gradientX*gradientX + gradientY*gradientY);
    //printf("Gradient: %f\n", computedGradient);
    write_imagef(oimage, (int2)(x, y), (computedGradient));

}