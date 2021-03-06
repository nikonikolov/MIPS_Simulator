

/* NOTE: PART OF THIS CODE WAS PARTLY COPIED FROM STACKOVERFLOW

http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c

IT'S PURPOSE IS ONLY TO READ A CSV FILE SO IT DOES NOT HAVE ANY 
CONTRIBUTION TO THE FUNCTIONALITY OF THE CPU ITSELF */



#ifndef CSVROW_H
#define CSVROW_H


#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class CSVRow{

public:

    //CSVRow();

    string const& operator[](size_t index) const{
        return m_data[index];
    }
        
    size_t size() const{
        return m_data.size();
    }
        
    void readLine(istream& infile){

        string         line;
        getline(infile,line);

        stringstream   lineStream(line);
        string         cell;

        m_data.clear();
        
        while( getline(lineStream, cell, ',') ){
            m_data.push_back(cell);
        }
    }

    friend istream& operator>>(istream& infile, CSVRow& data){
        data.readLine(infile);
        return infile;
    }   

private:
    vector<string> m_data;
};


#endif
