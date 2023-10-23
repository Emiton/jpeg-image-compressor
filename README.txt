Arith by Cameron LaFreniere and Emiton Alves

Help Received: Prof Daniels and TA Justin

Currently, we have implemented round trip testing and have successfully
converted RGB ints to RGB floats, RGB floats to YBR Floats, YBR Floats to
averaged Pb and Pr Values with Discrete Cosine Transformations performed upon
each YBR floats' Y values in a 2 by 2 block to arrive at a, b, c, and d floats.
We have converted averaged Pb & Pr, a, b, c, and d to scaled integers.

We implemented round trip testing of stage 1 to n (for example: stage 1 to stage 1 is
RGB scaled int to RGB float and then RGB float to RGB scaled int) and n to 1, with n
being a given stage in the compression/decompression process. We successfully peformed
up to stage 4, which is (stage 1) rgbInt to rgbFloat, (stage 2) rgbFloat to ybrFloat,
(stage 3) ybrFloat to reducedValues [averaged Pb and Pr, a, b, c, and d as floats], and (stage 4)
reducedValues to quantizedValues [averaged Pb and Pr, a, b, c, and d as scaled integers].

We chose to structure the program so that conversions between different data types
were kept in the same file. This means that the conversions of data from RGB values 
to component video were kept within the same. All functions that operated within
this conversion was kept together. The same went for component video to quantized values
and then quantized values to words. Compress and decompress are called from a file
which does a small amount of image preprocessing before starting the converson process.
Because the compress and decompress functions essentially accept the file input, we 
chose to do the processing as soon as the file input is accepted. 

Currently the program can compress and decompress a file, but it will not decompress
correctly. The difference as calculated by ppm is greater than .5, meaning that the
values of the decompressed image is up to 50% different than that of the input image.
Debugging this issue does not seem to be a major challenge, but unfortunately time is 
not on our side and the solution must be submitted as is.

We have spent 15 hours on the design document and 55 hours on programming arith. 
