#pragma once

#ifndef _MEMORY_
#define _MEMORY_

#include <stdlib.h>

typedef struct DataPoint
{
    int x,y;
    bool deleted;
}DataPoint;

DataPoint* Data;

int InsertData(DataPoint item){
    int index = sizeof(Data)/sizeof(Data[0]);
    Data = (DataPoint *)calloc(index, sizeof(DataPoint));
    *(Data + (index - 1)) = item;
    return index;
}

DataPoint GetData(int index)
{
    return Data[index -1];
}

DataPoint SetDeleteState(int index)
{
    *(Data + (index - 1)) = GetData(index).deleted =true;
}

void UpdateData(int index,DataPoint item)
{
    *(Data + (index - 1)) = item;
}


DataPoint RemoveData(int index)
{
    DataPoint* temp = (DataPoint*)malloc((sizeof(Data)/sizeof(Data[0]) - 1) * sizeof(DataPoint*));
    free(Data); 
    Data = temp;
}

DataPoint ClearData()
{
    free(Data);
}

#endif // !_MEMORY_