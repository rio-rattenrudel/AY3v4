void miniDelay()
{
    // Make a really short delay in software to allow for settling
    // time on the bus. The loop needs the NOP command because the
    // compiler optimisations will detect nothing is happening
    // witout it and consequently not put in a loop at all.

    for (byte i = 0; i < 64; i++)
        __asm("nop\n\t");

    PORTC = 0;
}

// sort algo
void isort(byte *a, int n)
{
    for (int i = 1; i < n; ++i) {

        int j = a[i];
        int k;

        for (k = i - 1; (k >= 0) && (j > a[k]); k--)
            a[k + 1] = a[k];

        a[k + 1] = j;
    }
}

// linear conversion as 16bit 0..65535
uint16_t rescale16(uint16_t val, uint16_t minO, uint16_t maxO, uint16_t minN, uint16_t maxN)
{
    return (val - minO) / (float)(maxO - minO) * (maxN - minN) + minN;
}

// linear conversion as 8bit version 0..256
byte rescale8(byte val, byte minO, byte maxO, byte minN, byte maxN)
{
    return (val - minO) / (float)(maxO - minO) * (maxN - minN) + minN;
}

// logarithmic conversion as 16bit version
float rescaleLog(uint16_t val, uint16_t minO, uint16_t maxO, uint16_t minN, uint16_t maxN)
{
    //  Logarithmic linear conversion:
    //  https://stackoverflow.com/questions/19472747/convert-linear-scale-to-logarithmic
    //
    //  X is your input the value in your data to be transformed
    //  Y is your output, the value converted to the new scale
    //  Xn and Yn are the max of your data, and the max of the new scale, respectively
    //  X0 and Y0 are the min of your data, and the min of the new scale, respectively
    //
    //  Y = Y0 + (log(X) - log(X0)) / (log(Xn) - log(X0)) * (Yn - Y0)

    // validation
    float lval  = val  ? log(val)  : 0;
    float lminO = minO ? log(minO) : 0;
    float lmaxO = maxO ? log(maxO) : 0;

    if (!val) return 0;

    return (lval - lminO) / (lmaxO - lminO) * (maxN - minN) + minN;
}