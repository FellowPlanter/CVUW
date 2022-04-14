#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if(im.c<0||c>=im.c){
        printf("Invalid channel");
        return 0;
    }
    if(x<0) x=0;
    if(x>=im.w) x = im.w-1;
    if(y<0) y=0;
    if(y>=im.h) y = im.h-1;
    return im.data[im.h*im.w*c + y*im.w + x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if(x<0||y<0||y>=im.h||x>=im.w||c<0||c>im.c){
        printf("Invalid arguments!");
        return;
    }
    im.data[im.h*im.w*c + y*im.w + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, im.h*im.w*im.c *sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int y=0;y<im.h;++y){
        for(int x=0;x<im.w;++x){
            float r = get_pixel(im,x,y,0);
            float g = get_pixel(im,x,y,1);
            float b = get_pixel(im,x,y,2);
            set_pixel(gray, x,y,0,r*0.299f + g*0.587f + b*0.114f);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int y=0;y<im.h;++y){
        for(int x=0;x<im.w;++x){
            set_pixel(im, x, y, c, get_pixel(im,x,y,c) + v);
        }
    }
}

void scale_image(image im, int c, float v)
{
    for(int y=0;y<im.h;++y){
        for(int x=0;x<im.w;++x){
            set_pixel(im, x, y, c, get_pixel(im,x,y,c) * v);
        }
    }
}


void clamp_image(image im)
{
    for(int y=0;y<im.h;++y){
        for(int x=0;x<im.w;++x){
            for(int c=0;c<im.c;++c){
                float v = get_pixel(im,x,y,c);
                if(v<0.0) v = 0.0;
                if(v>1.0) v = 1.0;
                set_pixel(im, x, y, c, v);
            }
        }
    }
}

float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for(int y=0;y<im.h;++y){
        for(int x=0;x<im.w;++x){
            float r = get_pixel(im,x,y,0);
            float g = get_pixel(im,x,y,1);
            float b = get_pixel(im,x,y,2);

            float V = three_way_max(r,g,b);
            
            float m = three_way_min(r,g,b);
            float C = V - m;
            float S = V ? C / V : 0;
            
            float H;
            if(C==0) H = 0;
            else if(r>=g && r>=b) H = (g-b)/C;
            else if(g>=r && g>=b) H = (b-r)/C + 2.0f;
            else H = (r-g)/C + 4.0f;
            H = H<0 ? H/6 + 1.0f : H/6;

            set_pixel(im,x,y,0,H);
            set_pixel(im,x,y,1,S);
            set_pixel(im,x,y,2,V);
            
        }
    }
}

void hsv_to_rgb(image im)
{
    for(int y=0;y<im.h;++y){
        for(int x=0;x<im.w;++x){
            float h = get_pixel(im,x,y,0);
            float s = get_pixel(im,x,y,1);
            float v = get_pixel(im,x,y,2);
            assert(h>=0 && h<=1);
            for(int i=0;i<3;++i) set_pixel(im,x,y,i,0);
            // continue;
            if(v==0){
                for(int i=0;i<3;++i) set_pixel(im,x,y,i,0);
                continue;
            }
            
            float C = s*v;
            if(C<=0.1f) {
                for(int i=0;i<3;++i) set_pixel(im,x,y,i,v);
                continue;
            }
            float m = v-C;
            //printf("h:%.6f", h);
           //ss if(h<=.5f) h++;
            if(6*h<=1.0 && 6*h >= 0.0){//r>g>b
                assert(6*h*C+m>=0);
                set_pixel(im,x,y,0,v);
                set_pixel(im,x,y,1, 6*h*C+m);
                set_pixel(im,x,y,2,m);
            }
            else if(6*h<=2.0 && 6*h >= 1.0){//g>r>b
                assert((-6*h+2)*C+m>=0);
                set_pixel(im,x,y,0,(-6*h+2)*C+m);
                set_pixel(im,x,y,1,v);
                set_pixel(im,x,y,2,m);
            }
            else if(6*h<=3.0 && 6*h >= 2.0){//g>b>r
                assert((6*h-2)*C+m>=0);
                set_pixel(im,x,y,0,m);
                set_pixel(im,x,y,1,v);
                set_pixel(im,x,y,2,(6*h-2)*C+m);
            }
            else if(6*h<=4.0 && 6*h >= 3.0){//b>g>r
                assert((-6*h+4)*C+m >= 0);
                set_pixel(im,x,y,0,m);
                set_pixel(im,x,y,1,(-6*h+4)*C+m);
                set_pixel(im,x,y,2,v);
            }
            else if(6*h<=5.0 && 6*h >= 4.0){//b>r>g
                assert((6*h-4)*C+m >= 0);
                set_pixel(im,x,y,0,(6*h-4)*C+m);
                set_pixel(im,x,y,1,m);
                set_pixel(im,x,y,2,v);
            }
            else if(6*h<=6.0 && 6*h >= 5.0){ //r>b>g
                set_pixel(im,x,y,0,v);
                set_pixel(im,x,y,1,m);
                set_pixel(im,x,y,2,(-6*h*C)+m<0 ? 6*C*(1-h)+m : (-6*h*C)+m);
            }
        }
    }
}
