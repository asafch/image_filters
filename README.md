# Computer Graphics, exercise 1

The purpose of this exercise is to use some basic filters in image processing.
A `256*256 px` bitmap grayscale image is loaded, and presented in the top-left quadrant of a window.
The image is passed through 3 filters:
 1. A [Sobel edge detection](https://en.wikipedia.org/wiki/Sobel_operator) filter, presented in the top-right quadrant of the window
 2. A [half-toning](https://en.wikipedia.org/wiki/Halftone) filter, presented in the bottom-left quadrant of the window
 3. A [Floyd-Steinberg](https://en.wikipedia.org/wiki/Floyd–Steinberg_dithering) dithering filter (8-bit grascale 0> 2-bit grayscale), presented in the bottom-right quadrant of the window

The path to the source image is given as `argv[1]`.
The pixel values of each respective image are written to respective text files, which paths should be hardwired in the code.

### License

The MIT License (MIT)

Copyright (c) 2015 Asaf Chelouche, Ben Ben-Chaya

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.