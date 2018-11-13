#include <except.h>
#include <assert.h>
// #include <stdint.h>

int check_laws(uint64_t word, unsigned w, unsigned lsb, unsigned value, unsigned w2, unsigned lsb2);

int main()
{
    FILE *randfp = fopen("/dev/urandom","r");
    uint64_t word;
    size_t rc = fread(&word, sizeof(word), 1, randfp);
    assert(rc == 1);

    // create random w
    // create random lsb

    for(unsigned w = 0; w <= 64; w++)
    {
        for(unsigned lsb = 0; lsb + w <= 64; lsb++)
        {
            for(unsigned trial = 0; trial < 1000; trial++)
            {
                // 1000 random trials
                // set values of other parameters
                check_laws(word, w, lsb, trial, w * 2, lsb * 2);
            }
        }
    }
    return 0;
}

// TODO: Determine type of parameters
int check_laws(uint64_t word, unsigned w, unsigned lsb, unsigned value, unsigned w2, unsigned lsb2)
{
    TRY
        assert(Bitpack_getu(Bitpack_newu(word, w, lsb, val), w, lsb) == val);
        if(lsb2 >= w + lsb)
        {
            assert(getu(newu(word, w, lsb, val), w2, lsb2) == getu(word, w2, lsb2));
        }
       // LAWS FROM BIT PACK HANDOUT
       // Law 1: 
       // Law 2: non overlapping fields should not be affected
    ELSE
        fprintf(stderr, "Exception raised during testing\n");
    END_TRY;
}
