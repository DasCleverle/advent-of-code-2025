#pragma once

#include <stdio.h>

void run(
    int const argc,
    char const* const* const argv,
    void (*part1)(FILE* const),
    void (*part2)(FILE* const)
);
