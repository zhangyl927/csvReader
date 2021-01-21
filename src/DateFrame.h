#ifndef __DATEFRAME_H
#define __DATEFRAME_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <climits>
#include <cfloat>

namespace readCSV
{

enum DataType { kInt64, kDouble, kString};

class DateFrame
{
public:
    DateFrame(std::string label="")
        :index_label(label) { };
    DateFrame(const DateFrame&) = default;

    void FromCsv(const char* path, std::map<std::string, DataType>& dtypes);  // Path是文件路径，dtypes是要读取的列和类型
    void ToCsv(const char* path, std::vector<std::string>& columns);          // 输出DataFrame到 out.csv文件中

    DateFrame& operator=(const DateFrame& rhs);

    //   查询接口
    const std::string GetData(const std::string& index_name, const std::string& column_name);
    const DateFrame GetData(const std::string& start, const std::string& end, const std::vector<std::string>& columns);
    std::string GetData(int i, int j);
    DateFrame GetData(int row_start, int row_end, int col_start, int col_end);
    
    DateFrame& Sort1(const std::string& column_name, bool ascend, bool copy);

    friend const DateFrame operator+(DateFrame& lhs, DateFrame& rhs);
    friend void operateHelper(DateFrame& opedObj, DateFrame& lhs, DateFrame& rhs, int l1, int l2);

    std::string getLabel() const { return index_label; }
    std::vector<std::string> getColumn() const { return column; }
    std::vector<std::string> getIndex() const { return index; }
    
    
    ~DateFrame() {  }

//     Fill(vector<>&  columns, string& method);
private:
    std::string index_label;                                    // datetime
    std::vector<std::string> column;                            // 第一行数据
    std::vector<std::string> index;                             // 第一列数据 index
    
    std::map<std::string, std::vector<uint64_t>> intColumn;              // key-列名， value-列数据；
    std::map<std::string, std::vector<double>> doubleColumn;
    std::map<std::string, std::vector<std::string>> stringColumn;            

    template<class T>
    DateFrame& helper(std::vector<T>& sortCol, bool ascend, bool copy);
    
    void readDtype(const DataType& dtype, const std::string& dtFirst, std::vector<std::vector<std::string>>& allData);

    void writeToFile(std::ofstream& fout, const std::vector<std::string>& columns, int idx);

    const std::string __GetData(int row, int col);
    const DateFrame __GetData(const std::string& start, const std::string& end, 
        const std::vector<std::string>& columns, const char* file);
};

const DateFrame operator+(DateFrame& lhs, DateFrame& rhs);
void operateHelper(DateFrame& opedObj, DateFrame& lhs, DateFrame& rhs, int l1, int l2);

}

#endif