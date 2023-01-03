/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Parser/SourceUtils.h"
#include "Velox/Parser/MementoSource.h"
#include "Util/StringUtils.h"

using namespace velox;
using namespace std;

namespace astu {


    void SourceUtils::PrintBinaryMemento(FastSource &source)
    {
        Memento memento;
        MementoSource::SourceToMemento(source, memento);

        const auto &data = memento.GetData();
        cout << "const size_t size = " << data.size() << ";" << endl;
        cout << "const unsigned char data[] = {" << endl;
        for (size_t i = 0; i < data.size(); ++i) {
            if (i % 15 == 0) {
                cout << endl << "\t";
            }
            cout << StringUtils::ToHexString(data[i], 2);
            if (i < data.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl << "};" << endl;
    }

} // end of namespace