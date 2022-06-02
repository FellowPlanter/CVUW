from uwimg import *
'''
im = load_image("data/dogsmall.jpg")
a = nn_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-nn")

im = load_image("data/dogsmall.jpg")
a = bilinear_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-bl")

im = load_image("data/dog.jpg")
a = nn_resize(im, im.w//7, im.h//7)
save_image(a, "dog7th-bl")


im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")


im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")


im = load_image("data/dog.jpg")
f = make_sharpen_filter()
res = convolve_image(im, f, 1)
feature_normalize(res)
#thumb = nn_resize(blur, blur.w//7, blur.h//7)
save_image(res, "sharpen")

im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)



a = load_image("data/dumbledore.png")
b = load_image("data/ron.png")
f = make_gaussian_filter(4)

lfreq = convolve_image(a, f, 1)
lb = convolve_image(b,f,1)
hfreq = b - lb

reconstruct = lfreq + hfreq
#feature_normalize(reconstruct)
save_image(lfreq, "low-frequency")
save_image(hfreq, "high-frequency")
save_image(reconstruct, "reconstruct")

import numpy as np
im = load_image("data/dog.jpg")
f = make_gaussian_filter(1)
print(f.h,f.w,int(f.h/2))
f1 = make_image(f.w,1,1)
l1 = []
m = 100
for i in range(f.w):    
    l1.append(get_pixel(f,i,int(f.w/2),0))
    set_pixel(f1,i,0,0, get_pixel(f,i,int(f.w/2),0)*m)
f2 = make_image(1,f.h,1)
l2 =[]
for i in range(f.h):
    l2.append(get_pixel(f,int(f.h/2),i,0))
    set_pixel(f2,0,i,0, get_pixel(f,int(f.h/2),i,0)*m)


l1_normalize(f1)
l1_normalize(f2)
l1 = np.asarray(l1).reshape((1,f.h))*m
l2 = np.asarray(l2).reshape((f.h,1))*m
l1 = l1/ np.sum(l1)
l2 = l2 / np.sum(l2)
print(np.sum(np.matmul(l2,l1)))
print(l1)
res = convolve_image(im,f2,1)
res1 = convolve_image(res,f1,1)
#feature_normalize(res)

'''
im = load_image("data/dog.jpg")
save_image(fast_gaussian_blur(im,10), "fun")

