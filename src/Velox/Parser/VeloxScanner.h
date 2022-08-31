#pragma once

// Local includes
#include "Script/Scanner.h"

// C++ Standard Library includes
#include <memory>

namespace velox {

    class VeloxScanner {
    public:

        VeloxScanner();

    private:
        std::unique_ptr<astu::Scanner> scanner;
    };

}


