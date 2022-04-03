#include "SourceFile.h"

// C++ Standard Library includes
#include <fstream>

using namespace std;

namespace velox {

    SourceFile::SourceFile(const string &inFilePath) : filePath(inFilePath) {
        // Intentionally left empty.
    }

    shared_ptr<istream> SourceFile::GetStream() {
        // Input file stream must be binary to conserve line breaks.
        return make_shared<ifstream>(filePath, ios::in | ios::binary);
    }

}
