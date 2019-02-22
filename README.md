HW3 - Spencer McDonough


Problem 2 Notes
1. Defects are any holes in white region that need to be corrected back to white
2. Rice grain type is defined by each grouping

Bond
4-connected:
010
101
010
8-connected:
101
000
101
B = 2*(#4-connected) + (#8-connected)

Problem 2q TODO:
1. Shrinking: Implement all masks for 

Problem 2a:
1. make sure you are exclusively matching with masks (not looking for masks INSIDE input)
2. recursively match every pixel around X (X0-X7) with conditional masks and adjust output accordingly (this is how you build M0-M7)
3. Once this is done, run M0-M7 through the 2nd filter against conditional masks
4. take 2
    1. create M map before running through P filter
    

Problem 1a (transform):
1. rotate image to nearest 90deg
2. child image pieces are too big for hole --> build vector of outer edge and match with vector of inner edge of hole
Questions
Problem 2a (shrinking): 

