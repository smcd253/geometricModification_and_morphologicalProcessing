#include "morph.hpp"

using namespace std;

// call from j = 0
unsigned char dComboGenerator(unsigned char *dSequence, int numDs, int i, int j)
{
    if (j < i)
    {
        if (j == numDs - 1) return !(*dSequence + j);
        return !*dSequence + j | dComboGenerator(dSequence, numDs, i, j + 1);
    }
    else
    {
        if (j == numDs - 1) return (*dSequence + j);
        return *dSequence + j | dComboGenerator(dSequence, numDs, i, j + 1); 
    }
    
}
int main(int argc, char *argv[])
{
    // unconditional masks

    // define output rules for each letter
    // A || B || C = 1 (7 varations)
    // A || B = 1 (3 variations)
    // D || D ... || D = 0 || 1 ([number Ds]^2 variations)
    // M && M && ... && M = 1 (1 variation)

    // keep track of total generated masks to make sure we don't go over 317
    int numGenMasks = 0;
    // keep track of place in output mask array
    int blockPointer = 0;
    // loop through all mask char sequences
    int _m;
    for (int m = 0; m < NUM_UNCOND_MASK_TEMPLATES; m++)
    {
        // loop through individual mask to count number of Ds
        int numDs = 0;
        // catch ABs or ABCs with flag
        int abFlag = 0;
        int abcFlag = 0;

        // keep byte of Ds to track binary sequence
        unsigned char _dTracker;
        // make an array of binary numbers to check to see if a certain D is active in any given loop
        unsigned char _activeDs[8] = {0};

        for (int i = 0; i < 8; i++)
        {
            if(uncondMaskTemplates[m][i] == 'D')
            {
                numDs++;
                unsigned char temp = pow(2,i);
                _dTracker += temp;
                _activeDs[i] = temp;
            }
            else if(uncondMaskTemplates[m][i] == 'C')
            {
                abcFlag = 1;
                abFlag = 0;
            }
            else if(uncondMaskTemplates[m][i] == 'A' && !abcFlag)
            {
                abFlag = 1;
            }
        }
        // printf("mask %d: Ds: %d, ABs: %d, ABCs: %d\n", m + 1, numDs, abFlag, abcFlag);

        printf("Mask %d dTracker = ", m + 1);
        for (int i = 0; i < 8; i++) 
        {
            printf("%d", !!((_dTracker << i) & 0x80));
        }
        printf("\n");

        // build an array dLocations that contains only the active dLocations of length numDs
        unsigned char dLocations[numDs];
        int d = 0;
        for (int i = 0; i < 8; i++)
        {
            if(_activeDs[i])
            {
                dLocations[d] = _activeDs[i];
                d++;
            } 
        }
        
        // build an array with all possible d combinations of 
        int dCombos = (int)pow(numDs,2);
        unsigned char possibleDCombos[dCombos];
        
        for (int i = 0; i < numDs; i++)
        {
            for(int j = 0; j < numDs; j++)
            {
                possibleDCombos[d] = dComboGenerator(dLocations, numDs, numDs - i, 0);
                printf("possibleDCombos[%d] = ", d);
                for (int i = 0; i < 8; i++) 
                {
                    printf("%d", !!((possibleDCombos[d] << i) & 0x80));
                }
                printf("\n");
                d++;

                // rotate input and perform again
                unsigned char temp = dLocations[0];
                for (int f = 0; f < numDs; f++)
                {
                    dLocations[f] = dLocations[(numDs - 1) - f];
                }
                dLocations[numDs - 1] = temp;
            }
        }
        
    }    
    //     // keep byte of ABs to track binary sequence
    //     unsigned char _abTracker = 0b00000011;
    //     unsigned char ab_aActive = 0b00000010;
    //     unsigned char ab_bActive = 0b00000001;

    //     // keep byte of ABCs to track binary sequence
    //     unsigned char _abcTracker = 0b00000111;
    //     unsigned char abc_aActive = 0b00000100;
    //     unsigned char abc_bActive = 0b00000010;
    //     unsigned char abc_cActive = 0b00000001;

    //     // allocate blocksize based on number of variations in mask
    //     int blockSize = 1;
    //     if(abFlag)
    //     {
    //         blockSize *= 3;
    //     }
    //     else if (abcFlag)
    //     {
    //         blockSize *= 7;
    //     }
    //     if(numDs)
    //         blockSize *= pow(2, numDs);

    //     printf("Mask %d numVariations: %d\n", m + 1, blockSize);
    //     numGenMasks += blockSize;

    //     printf("Mask %d Variations:\n", m + 1);
    //     int variationNum = 1;
    //     // loop throuh masks and drop variations into output
    //     // keep track of each D and make sure you're going through all combinations

    //     for (int i = 0; i < blockSize; i++)
    //     {
    //         // assign temp vals for every variation
    //         unsigned char dTracker = _dTracker;
    //         unsigned char *activeDs = _activeDs;
    //         unsigned char abTracker = _abTracker;
    //         unsigned char abcTracker = _abcTracker;

    //         printf("Variation %d:\n", i);
    //         printf("dTracker = ");
    //         for (int i = 0; i < 8; i++) 
    //         {
    //             printf("%d", !!((dTracker << i) & 0x80));
    //         }
    //         printf("\n");
    //         //printf("{", i + 1);
    //         for (int l = 0; l < 8; l++)
    //         {
    //             // M cases
    //             if(uncondMaskTemplates[m][l] == 'M')
    //             {
    //                 uncondMasks_ST[blockPointer][l] = 1;
    //             }
    //             // 0 cases
    //             else if (uncondMaskTemplates[m][l] == 0) 
    //                 uncondMasks_ST[blockPointer][l] = 0;
    //             // AB cases (if this is a mask containing AB with no C)
    //             // make sure we avoid 0 case by checking to see if abTracker still +
    //             if(abTracker)
    //             {
    //                 if(uncondMaskTemplates[m][l] == 'A')
    //                 {
    //                     if(abTracker && ab_aActive)
    //                         uncondMasks_ST[blockPointer][l] = 1;
    //                     else
    //                         uncondMasks_ST[blockPointer][l] = 0;
    //                 }
    //                 else if (uncondMaskTemplates[m][l] == 'B')
    //                 {
    //                     if(abTracker && ab_bActive)
    //                         uncondMasks_ST[blockPointer][l] = 1;
    //                     else
    //                         uncondMasks_ST[blockPointer][l] = 0;
    //                 }
    //                 abTracker--;
    //             }
    //             // ABC cases (if this is a mask containing ABC, and not AB)
    //             // make sure we avoid 0 case by checking to see if abcTracker still +
    //             else if(abcTracker)
    //             {
    //                 if(uncondMaskTemplates[m][l] == 'A')
    //                 {
    //                     if(abcTracker && abc_aActive)
    //                         uncondMasks_ST[blockPointer][l] = 1;
    //                     else
    //                         uncondMasks_ST[blockPointer][l] = 0;
    //                 }
    //                 else if (uncondMaskTemplates[m][l] == 'B')
    //                 {
    //                     if(abcTracker && abc_bActive)
    //                         uncondMasks_ST[blockPointer][l] = 1;
    //                     else
    //                         uncondMasks_ST[blockPointer][l] = 0;
    //                 }
    //                 else if (uncondMaskTemplates[m][l] == 'C')
    //                 {
    //                     if(abcTracker && abc_cActive)
    //                         uncondMasks_ST[blockPointer][l] = 1;
    //                     else
    //                         uncondMasks_ST[blockPointer][l] = 0;
    //                 }
    //                 abcTracker--;
    //             }
    //             if(numDs)
    //             {
    //                 if(dTracker)
    //                 {
    //                     if (uncondMaskTemplates[m][l] == 'D')
    //                     {
    //                         if(dTracker && activeDs[l]) 
    //                             uncondMasks_ST[blockPointer][l] = 1;
    //                         else
    //                             uncondMasks_ST[blockPointer][l] = 0;
    //                     }
    //                     dTracker--;
    //                 }
    //                 else
    //                 {
    //                     if (uncondMaskTemplates[m][l] == 'D')
    //                     {
    //                         uncondMasks_ST[blockPointer][l] = 0;
    //                     }
    //                 }
    //             }
    //             printf("dTracker round %d = ", l);
    //             for (int i = 0; i < 8; i++) 
    //             {
    //                 printf("%d", !!((dTracker << i) & 0x80));
    //             }
    //             printf("\n");
    //             // print result of check
    //             //printf("%d,",uncondMasks_ST[blockPointer][l]);
    //         }
    //         //printf("}\n");
    //     }
    // }
    // printf("blockPointer = %d\n", blockPointer);
    // printf("numGenMasks = %d\n", numGenMasks);   
    
    return 0;
}