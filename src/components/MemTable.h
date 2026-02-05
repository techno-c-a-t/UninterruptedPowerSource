#ifndef MEMTABLE_H
#define MEMTABLE_H
#include "UPSource.h"
#include "SaveOpen.h"
void VectorToGrid(const std::vector<storage::CVMemPair>& data, TStringGrid* grid, int start, int end = -1);
void GridToVector(TStringGrid* grid, std::vector<storage::CVMemPair>& data, int start , int end = -1);

#endif