#ifndef CHAMELEON_H
#define CHAMELEON_H

#include "sensors/IPixySensor.h"
#include "controllers/PixyController.h"


class Chameleon {

private:
    PixyController pixyController;

public:

    // should we store a list of all the blocks that were scanned in that frame?
    // because when we get our target block we want its latest information - as we get closer the position will update
    // then we just use the index to fetch the latest information from the updated list

    void init(IPixySensor* pixy);
    void run();
    
};


#endif