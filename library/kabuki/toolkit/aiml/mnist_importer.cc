

#pragma once
#include <stdafx.h>
#include "nmist_importer.h"

using namespace std;

void MnistImport () {

    // ifstream is used for reading files
    // We'll read from a file called Sample.dat
    ifstream file ("train-labels.idx1-ubyte");

    if (!file) {
        cout << "Could open file \"nmist.ts\" for reading!" << endl;
        exit (1);
    }

    MnistImages labels;

    while (file) {
        file >> labels.magic_number;
        cout << "\nMagic Number:" << labels.magic_number;

        file >> labels.item_count;
        cout << "\nItem Count:" << labels.item_count;

        file >> labels.row_count;
        cout << "\nWidth:" << labels.row_count;

        file >> labels.column_count;
        cout << " Height:" << labels.column_count;
        
        uint32_t n = labels.item_count * labels.row_count *
                     labels.column_count;
        uint8_t* pixels = new uint8_t[n];
        for (; n > 0; --n)
            file >> pixels[n];
    }
}
