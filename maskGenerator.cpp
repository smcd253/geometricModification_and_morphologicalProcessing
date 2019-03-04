#include "morph.hpp"

using namespace std;

// call from j = 0
unsigned char dComboGenerator(unsigned char *dSequence, int numDs, int i, int j)
{
    if (j > i)
    {
        if (j == numDs - 1) return !*(dSequence + j);
        return !*(dSequence + j) | dComboGenerator(dSequence, numDs, i, j + 1);
    }
    else
    {
        if (j == numDs - 1) return *(dSequence + j);
        return *(dSequence + j) | dComboGenerator(dSequence, numDs, i, j + 1); 
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
    for (int m = 0; m < NUM_UNCOND_MASK_TEMPLATES; m++)
    {
        // loop through individual mask to count number of Ds
        int numDs = 0;
        // catch ABs or ABCs with flag
        int abFlag = 0;
        int abcFlag = 0;

        // keep byte of Ds to track binary sequence
        unsigned char _dTracker = 0b00000000;
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
        printf("mask %d: Ds: %d, ABs: %d, ABCs: %d\n", m + 1, numDs, abFlag, abcFlag);

        printf("Mask %d dTracker = ", m + 1);
        for (int i = 0; i < 8; i++) 
        {
            printf("%d", !!((_dTracker << i) & 0x80));
        }
        printf("\n");

        // build an array dLocations that contains only the active dLocations of length numDs
        unsigned char dLocations[numDs];
        int activateArray[numDs][numDs];
        for (int i = 0; i < numDs; i++)
        {
            for(int j = 0; j < numDs; j++)
            {
                if (j >= i)
                    activateArray[i][j] = 1;
                else activateArray[i][j] = 0;
            }
        }
        
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
        int dCombos = (int)pow(2,numDs);
        printf("number d combos = %d\n", dCombos);
        unsigned char possibleDCombos[dCombos];
        for (int i = 0; i < dCombos; i++)
            possibleDCombos[i] = 0;
        d = 0;

        // loop through activate array and build permutations to activate different Ds in input array
        for (int i = 0; i < numDs; i++)
        {
            while(next_permutation(activateArray[i], activateArray[i] + numDs))
            {
                for (int j = 0; j < numDs; j++)
                {
                    possibleDCombos[d] |= activateArray[i][j] * dLocations[j];
                };   
                d++;
            } 
            for (int j = 0; j < numDs; j++)
            {
                possibleDCombos[d] |= activateArray[i][j] * dLocations[j];
            }
            d++;       
        } 
        for (int i = 0; i < dCombos; i++)
        {
            printf("possibleDCombos[%d] = ", i);
            for (int j = 0; j < 8; j++) 
            {
                printf("%d", !!((possibleDCombos[i] << j) & 0x80));
            }
            printf("\n");
        }    
        
       
        // keep byte of ABs to track binary sequence
        unsigned char _abTracker = 0b00000011;
        unsigned char ab_aActive = 0b00000010;
        unsigned char ab_bActive = 0b00000001;

        // keep byte of ABCs to track binary sequence
        unsigned char _abcTracker = 0b00000111;
        unsigned char abc_aActive = 0b00000100;
        unsigned char abc_bActive = 0b00000010;
        unsigned char abc_cActive = 0b00000001;

        // allocate blocksize based on number of variations in mask
        int blockSize = 1;
        if(abFlag)
        {
            blockSize *= 3;
        }
        else if (abcFlag)
        {
            blockSize *= 7;
        }
        if(numDs)
            blockSize *= dCombos;

        printf("Mask %d numVariations: %d\n", m + 1, blockSize);
        numGenMasks += blockSize;

        printf("Mask %d Variations:\n", m + 1);
        int variationNum = 1;
        // loop throuh masks and drop variations into output
        // keep track of each D and make sure you're going through all combinations

        for (int i = 0; i < dCombos; i++)
        {
            // assign temp vals for every variation
            unsigned char abTracker = _abTracker;
            unsigned char abcTracker = _abcTracker;

            if (abFlag)
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        if(uncondMaskTemplates[m][l] == 'A')
                        {
                            if(!!((abcTracker << 6) & 0x80))
                                uncondMasks_ST[blockPointer][l] = 1;
                            else
                                uncondMasks_ST[blockPointer][l] = 0;
                        }
                        else if (uncondMaskTemplates[m][l] == 'B')
                        {
                            if(!!((abcTracker << 7) & 0x80))
                                uncondMasks_ST[blockPointer][l] = 1;
                            else
                                uncondMasks_ST[blockPointer][l] = 0;
                        }
                        // M cases
                        else if(uncondMaskTemplates[m][l] == 'M')
                        {
                            uncondMasks_ST[blockPointer][l] = 1;
                        }
                        // 0 cases
                        else if (uncondMaskTemplates[m][l] == 0) 
                            uncondMasks_ST[blockPointer][l] = 0;
                        // D cases
                        else if (uncondMaskTemplates[m][l] == 'D')
                        {
                            if(!!((possibleDCombos[i] << l) & 0x80)) 
                                uncondMasks_ST[blockPointer][l] = 1;
                            else
                                uncondMasks_ST[blockPointer][l] = 0;
                        }
                    }
                    abTracker--;
                    blockPointer++;
                }
            }
            else if(abcFlag)
            {
                for (int j = 0; j < 7; j++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        if(uncondMaskTemplates[m][l] == 'A')
                        {
                            if(!!((abcTracker << 5) & 0x80))
                                uncondMasks_ST[blockPointer][l] = 1;
                            else
                                uncondMasks_ST[blockPointer][l] = 0;
                        }
                        else if (uncondMaskTemplates[m][l] == 'B')
                        {
                            if(!!((abcTracker << 6) & 0x80))
                                uncondMasks_ST[blockPointer][l] = 1;
                            else
                                uncondMasks_ST[blockPointer][l] = 0;
                        }
                        else if (uncondMaskTemplates[m][l] == 'C')
                        {
                            if(!!((abcTracker << 7) & 0x80))
                                uncondMasks_ST[blockPointer][l] = 1;
                            else
                                uncondMasks_ST[blockPointer][l] = 0;
                        }
                        // M cases
                        else if(uncondMaskTemplates[m][l] == 'M')
                        {
                            uncondMasks_ST[blockPointer][l] = 1;
                        }
                        // 0 cases
                        else if (uncondMaskTemplates[m][l] == 0) 
                            uncondMasks_ST[blockPointer][l] = 0;
                        // D cases
                        else if (uncondMaskTemplates[m][l] == 'D')
                        {
                            if(!!((possibleDCombos[i] << l) & 0x80)) 
                                uncondMasks_ST[blockPointer][l] = 1;
                            else
                                uncondMasks_ST[blockPointer][l] = 0;
                        }
                    }
                    abcTracker--;
                    blockPointer++;
                }
            }
            else
            {   
                for (int l = 0; l < 8; l++)
                {
                    // M cases
                    if(uncondMaskTemplates[m][l] == 'M')
                    {
                        uncondMasks_ST[blockPointer][l] = 1;
                    }
                    // 0 cases
                    else if (uncondMaskTemplates[m][l] == 0) 
                        uncondMasks_ST[blockPointer][l] = 0;
                    // D cases
                    else if (uncondMaskTemplates[m][l] == 'D')
                    {
                        if(!!((possibleDCombos[i] << l) & 0x80)) 
                            uncondMasks_ST[blockPointer][l] = 1;
                        else
                            uncondMasks_ST[blockPointer][l] = 0;
                    }
                }
                blockPointer++;
            }
        }
    }
    for (int i = 0; i < blockPointer; i++)
    {
        printf("uncondMasks_ST[%d] = {", i);
        for (int l = 0; l < 8; l++)
            printf("%d,", uncondMasks_ST[i][l]);
        printf("}\n");
    }
    printf("blockPointer = %d\n", blockPointer);
    printf("numGenMasks = %d\n", numGenMasks);   
    
    return 0;
}