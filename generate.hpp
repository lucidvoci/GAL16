//
//  generate.hpp
//  GAL_projekt
//
//  Created by Lucie Dvorakova on 04.12.16.
//
//

#ifndef generate_hpp
#define generate_hpp

#include <stdio.h>

#define VERSION1 (1)
#define VERSION2 (2)
#define VERSION3 (3)
#define VERSION4 (4)
#define VERSION5 (5)
#define VERSION6 (6)

#define SMALL_MAX (100)
#define SMALL_MIN (50)
#define MED_MAX (500)
#define MED_MIN (250)
#define LARGE_MAX (2500)
#define LARGE_MIN (1250)

#define LOW_DENS_COEF   (3)
#define MID_DENS_COEF   (20)
#define HIGH_DENS_COEF  (100)
#define FULL_DENS_COEF  (1000)

#define MAX_WEIGHT (20)
#define MIN_WEIGHT_NEG (-5)
#define MIN_WEIGHT (1)

class Generate {
public:
    int generate(int version);
};

#endif /* generate_hpp */
