#include "pch.h"
#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>

vector<vector<wstring>> FileManager::GetDataFromCSV(const string& filename)
{
    vector<vector<wstring>> data;
    locale::global(locale("korean"));

    wifstream file(filename);
    if (!file.is_open())
    {
       return data;
    }

    wstring line;
    while (getline(file, line))
    {
        vector<wstring> row;
        wstringstream ss(line);
        wstring cell;

        while (getline(ss, cell, L','))
        {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    file.close();

    return data;
}
