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

As it currently stands, our program will compile, but not successfully perform round trip
testing for stage 5: packing the quantizedValues [averaged Pb and Pr, a, b, c, and d as scaled integers]
into a 32-bit codeword. Currently our entire compression algorithm works, but our unpacking
of 32-bit words is incorrect. Every time we compile and run the program, the unpacked word
prints out different quantized values of a, b, c, d, avgPb, and avgPr.

We have spent 15 hours on the design document, 
