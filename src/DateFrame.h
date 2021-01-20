#ifndef _DateFrame_h
#define _DateFrame_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <climits>
#include <cfloat>

using namespace std;

enum DataType { kInt64, kDouble, kString};
double strToDou(const string& str);
uint64_t strToInt(string& str);

void sys_error(const char* str)
{
    cout << str << endl;
    exit(-1);
}

class DateFrame
{
public:
    void FromCsv(const char* path, map<string, DataType>& dtypes);  // Path是文件路径，dtypes是要读取的列和类型

    void ToCsv(const char* path, vector<string>& columns);        // 输出DataFrame到 out.csv文件中

    DateFrame& operator=(const DateFrame& rhs);
    DateFrame& operator=(DateFrame&& rhs);

    //   查询接口
    string GetData(const string& index_name, const string& column_name);
    DateFrame GetData(const string& start, const string& end, const vector<string>& columns);
    string GetData(int i, int j);
    DateFrame GetData(int row_start, int row_end, int col_start, int col_end);
    
    DateFrame& Sort1(const string& column_name, bool ascend, bool copy);

    friend const DateFrame operator+(DateFrame& lhs, DateFrame& rhs);

    friend void operateHelper(DateFrame& opedObj, DateFrame& lhs, DateFrame& rhs, int l1, int l2);

//     Fill(vector<>&  columns, string& method);
private:
    string index_label;                             // datetime
    vector<string> column;                          // 第一行数据
    vector<string> index;                           // 第一列数据 index
    
    map<string, vector<uint64_t>> intColumn;              // key-列名， value-列数据；
    map<string, vector<double>> doubleColumn;
    map<string, vector<string>> stringColumn;            

    template<class T>
    DateFrame& helper(vector<T>& sortCol, bool ascend, bool copy);
};


#endif