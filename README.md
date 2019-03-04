////////////////////////////////////
Spencer McDonough
6341617166
shmcdono@usc.edu

EE569 Homework #3
March 3rd, 2019
////////////////////////////////////

Compilation Instructions:
Problem 1a
g++ extractChild.cpp -o extractChild
./extractChild HW3_images/lighthouse1.raw HW3_images/lighthouse1_extracted.raw 1 0 1
./extractChild HW3_images/lighthouse2.raw HW3_images/lighthouse2_extracted.raw 1 0 0
./extractChild HW3_images/lighthouse3.raw HW3_images/lighthouse3_extracted.raw 0 1 0
g++ placeChild.cpp -o placeChild
./placeChild HW3_images/lighthouse.raw HW3_images/lighthouse1_extracted.raw HW3_images/lighthouse_filled.raw 112 108 1
./placeChild HW3_images/lighthouse_filled.raw HW3_images/lighthouse2_extracted.raw HW3_images/lighthouse_filled.raw 143 142 2
./placeChild HW3_images/lighthouse_filled.raw HW3_images/lighthouse3_extracted.raw HW3_images/lighthouse_filled.raw 209 214 3

Problem 2a
g++ maskGenerator.cpp -o maskGenerator
./maskGenerator
g++ shrinking.cpp -o shrinking
./shrinking HW3_images/pattern1.raw HW3_images/pattern1-shrink.raw
./shrinking HW3_images/pattern2.raw HW3_images/pattern2-shrink.raw
./shrinking HW3_images/pattern3.raw HW3_images/pattern3-shrink.raw
./shrinking HW3_images/pattern4.raw HW3_images/pattern4-shrink.raw