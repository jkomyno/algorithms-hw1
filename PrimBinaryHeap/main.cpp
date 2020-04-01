// PrimBinaryHeap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BinaryHeap.h"

int main()
{
    std::vector<float> data{
        53.0F,
        82.3F,
        12.2F,
        23.01F,
        03.2F,
        38.1F
    };

    BinaryHeap<float> heap(std::move(data));

    std::cout << "Heap size: " << heap.size() << std::endl;

    while (heap.size() > 0) {
        std::cout << "Min: " << heap.extract_min() << std::endl;
    }
}
