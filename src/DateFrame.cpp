#include "DateFrame.h"
#include "tool.h"


readCSV::DateFrame& readCSV::DateFrame::operator=(const DateFrame& rhs)
{
    std::cout << "operator=(const DateFrame)\n";
    if (this == &rhs) return *this;

    this->index_label = rhs.index_label;
    this->column = rhs.column;
    this->index = rhs.index;
    this->intColumn = rhs.intColumn;
    this->doubleColumn = rhs.doubleColumn;
    this->stringColumn = rhs.stringColumn;
    
    return *this;
}

void readCSV::DateFrame::FromCsv(const char* path, std::map<std::string, DataType>& dtypes)
{
    std::vector<std::vector<std::string>> allData;                  // 将所有数据保存到 allData 中;
    std::ifstream fin(path, std::ifstream::in);

    std::string line;                                               // 用于保存首行；
    while (std::getline(fin, line))                                 // 获取一行；
    {
        std::vector<std::string> curLine;
        std::istringstream sin(line);
        std::string singleString;
        while (getline(sin, singleString, ','))
        {
            curLine.push_back(singleString);                        // 将当前行中每个string 存放起来；
        }
        allData.push_back(curLine);                                 // 将当前行存放起来；
    }

    index_label = allData[0][0];                                    // index_label = datetime;

    int n = allData.size();
    index.resize(n-1);
    for (int i=1; i<n; ++i)                                         // 保存第一列 index;
    {
        index[i-1] = allData[i][0];
    }

    // 读取 dtypes 数据和类型 kInt64, kDouble, kString
    for (auto dt:dtypes)
    {
        column.push_back(dt.first);                 // 保存第一行数据
        readDtype(dt.second, dt.first, allData);
    }
    return;
}

void readCSV::DateFrame::readDtype(const DataType& dtype, const std::string& dtFirst, std::vector<std::vector<std::string>>& allData)
{
    int idx = std::find(allData[0].begin(), allData[0].end(), dtFirst)-allData[0].begin();

    int n = allData.size();
    if (kInt64 == dtype)                 // dtype 为 kInt64 类型
        {
            std::vector<uint64_t> col(n-1);
            for (int i=1; i<n; ++i)
            {
                std::string str = allData[i][idx];
                uint64_t element = (""==str)?LLONG_MAX:readCSV::strToInt(str);
                col[i-1] = element;
            }
            intColumn[dtFirst] = col;
        }
    else if (kDouble == dtype)           // dtype 为 kDouble 类型
    {
        std::vector<double> col(n-1);
        for (int i=1; i<n; ++i)
        {
            std::string str = allData[i][idx];
            double element = readCSV::strToDouble(str);
            col[i-1] = element;
        }
        doubleColumn[dtFirst] = col;
    }
    else if (kString == dtype)            // dtype 为 kDouble 类型
    {
        std::vector<std::string> col(n-1);
        for (int i=1; i<allData.size(); ++i)
        {
            std::string str = allData[i][idx];
            col[i-1] = str;
        }
        stringColumn[dtFirst] = col;
    }
}


void readCSV::DateFrame::ToCsv(const char* path, std::vector<std::string>& columns)
{
    std::ofstream fout(path);

    // 将 column 行写入文件；
    std::string row(index_label);             // 存储 column 行，第一位为 index_label;
    for (auto col:column)
    {
        auto iter = find(columns.begin(), columns.end(), col);
        if (iter != columns.end())
        {
            row += ','+col;
        }
        else row += ',';
    }
    fout << row << std::endl;

    // 将 columns 中的列写入文件；
    for (int i=0; i<index.size(); ++i)
    {
        writeToFile(fout, columns, i); 
    }
    return;
}

// 查找单个值
const std::string readCSV::DateFrame::GetData(const std::string& index_name, const std::string& column_name)
{
    int row = find(index.begin(), index.end(), index_name)-index.begin();
    int col = find(column.begin(), column.end(), column_name)-column.begin();

    return GetData(row, col);
}

// 按照index和columns查找多个值。
const readCSV::DateFrame readCSV::DateFrame::GetData(const std::string& start, const std::string& end, const std::vector<std::string>& columns)
{
    const char* file = "../outPut/getDataByColumns.csv";
    return __GetData(start, end, columns, file);
}

// 按照行列号查找单个值
std::string readCSV::DateFrame::GetData(int i, int j)
{
    return __GetData(i, j);
}

const std::string readCSV::DateFrame::__GetData(int row, int col)
{
    if (row>index.size() || col>column.size()+1)
    {
        return "row or col out of variable range";
    }

    std::string str(column[col-1]);
    if (intColumn.count(str) > 0) return std::to_string(intColumn[str][row-1]);
    else if (doubleColumn.count(str) > 0) return std::to_string(doubleColumn[str][row-1]);
    else if (stringColumn.count(str) > 0) return stringColumn[str][row-1];

    return std::string();
}

// 按照行列号查找多个值。
readCSV::DateFrame readCSV::DateFrame::GetData(int row_start, int row_end, int col_start, int col_end)
{
    if (row_start>row_end || col_start>col_end || col_start>=column.size()) 
    {
        readCSV::sys_error("invalid input parameter\n");
    }
    
    col_end = (col_end>=column.size()) ? (column.size()-1) : col_end;

    std::string start = index[row_start-1];
    std::string end = index[row_end-1];

    int len = col_end-col_start+1;
    std::vector<std::string> columns(column.begin()+col_start, column.begin()+len);

    const char* file = "../outPut/getDataByRowAndCols.csv";

    return __GetData(start, end, columns, file);
}

// 按某一列排序
readCSV::DateFrame& readCSV::DateFrame::Sort1(const std::string& column_name, bool ascend=true, bool copy=true)
{
    // 若 column_name 不在 DateFrame 中，直接返回
    if (intColumn.count(column_name)==0 && doubleColumn.count(column_name)==0 && stringColumn.count(column_name)==0)
    {
        return *this;
    }
    else if (intColumn.count(column_name) > 0)
    {
        std::vector<uint64_t> sortCol = intColumn[column_name];                              // 要排序的列
        return helper(sortCol, ascend, copy);
    }
    else if (doubleColumn.count(column_name) > 0)
    {
        std::vector<double> sortCol = doubleColumn[column_name];
        return helper(sortCol, ascend, copy);
    }
    else if (stringColumn.count(column_name) > 0)
    {
        std::vector<std::string> sortCol = stringColumn[column_name];
        return helper(sortCol, ascend, copy);
    }

    return *this;
}

// operator +
const readCSV::DateFrame readCSV::operator+(readCSV::DateFrame& lhs, readCSV::DateFrame& rhs)
{
    int len1 = lhs.index.size(), len2 = rhs.index.size();
    int l1 = 0, l2 = 0;
    readCSV::DateFrame opedObj(lhs.getLabel());

    for (int i=0; i<lhs.column.size(); ++i)
    {
        auto iter = find(rhs.column.begin(), rhs.column.end(), lhs.column[i]);
        if (iter != rhs.column.end())
        {
            opedObj.column.push_back(lhs.column[i]);
        } 
    }

    while (l1<len1 && l2<len2)
    {
        if (lhs.index[l1] == rhs.index[l2])
        {
            /* 比较 column[l1] 是否等于 column[l2] */
            opedObj.index.push_back(lhs.index[l1]);
            operateHelper(opedObj, lhs, rhs, l1, l2);
            l1++; l2++;
        }
        else if (lhs.index[l1] < rhs.index[l2])
        {
            l1++;
        }
        else if (lhs.index[l1] > rhs.index[l2])
        {
            l2++;
        }
    }

    return opedObj;
}

template<class T>
readCSV::DateFrame& readCSV::DateFrame::helper(std::vector<T>& sortCol, bool ascend, bool copy)
{
    std::vector<T> temp(sortCol);
    if (false == ascend) sort(temp.begin(), temp.end(), std::greater<T>());                  // 倒序排序
    else sort(temp.begin(), temp.end());                                                //  排序

    std::map<std::string, std::vector<uint64_t>> newIntColumn;                                         // key-列名， value-列数据；
    std::map<std::string, std::vector<double>> newDoubleColumn;
    std::map<std::string, std::vector<std::string>> newStringColumn;  

    std::vector<std::string> newIndex(index.size(), "");

    for (int i=0; i<temp.size(); ++i)
    {
        
        int row = find(sortCol.begin(), sortCol.end(), temp[i])-sortCol.begin();         // 获取 temp[i] 在 sortCol 中是第几行；
        newIndex[i] = index[row];
        for (auto c1:intColumn)
        {
            std::string col_name = c1.first;
            newIntColumn[col_name].push_back(c1.second[row]);
        }
        for (auto c2:doubleColumn)
        {
            std::string col_name = c2.first;
            newDoubleColumn[col_name].push_back(c2.second[row]);
        }
        for (auto c3:stringColumn)
        {
            std::string col_name = c3.first;
            newStringColumn[col_name].push_back(c3.second[row]);
        }
    }

    //  返回新的 DataFrame
    if (copy)                                                                            
    {
        DateFrame* sortedNew = new DateFrame;
        
        sortedNew->index_label = index_label;
        sortedNew->column = column;
        
        sortedNew->index = newIndex;
        sortedNew->intColumn = newIntColumn;
        sortedNew->doubleColumn = newDoubleColumn;
        sortedNew->stringColumn = newStringColumn;

        return *sortedNew;
    }

    //  返回该 DataFrame
    
    index = newIndex;
    intColumn = newIntColumn;
    doubleColumn = newDoubleColumn;
    stringColumn = newStringColumn;
    return *this;
}

void readCSV::operateHelper(readCSV::DateFrame& opedObj, readCSV::DateFrame& lhs, readCSV::DateFrame& rhs, int l1, int l2)
{
    for (auto c:opedObj.column)
    {
        if (lhs.intColumn.count(c)>0 && rhs.intColumn.count(c)>0)
        {
            uint64_t res = lhs.intColumn[c][l1]+rhs.intColumn[c][l2];
            opedObj.intColumn[c].push_back(res);
        }
        else if (lhs.doubleColumn.count(c)>0 && rhs.doubleColumn.count(c)>0)
        {
            double res = lhs.doubleColumn[c][l1]+rhs.doubleColumn[c][l2];
            opedObj.doubleColumn[c].push_back(res);
        }
        else if (lhs.stringColumn.count(c)>0 && rhs.stringColumn.count(c)>0)
        {
            opedObj.stringColumn[c].push_back(lhs.stringColumn[c][l1]);
        }
    }
    return;
}

void readCSV::DateFrame::writeToFile(std::ofstream& fout, const std::vector<std::string>& columns, int i)
{
    std::string curLine(index[i]);                                                    // index 列
    for (int j=0; j<column.size(); ++j)
    {
        auto cols = find(columns.begin(), columns.end(), column[j]);
        if (cols != columns.end())
        {
            if (intColumn.count(*cols) > 0)
            {
                curLine += ","+std::to_string((intColumn[*cols])[i]);               // 在 int 类型 的 map 中
            }
            else if (doubleColumn.count(*cols) > 0)
            {
                curLine += ","+std::to_string((doubleColumn[*cols])[i]);             // 在 double 类型 的 map 中
            }
            else if (stringColumn.count(*cols) > 0)
            {
                curLine += ","+(stringColumn[*cols])[i];                            // 在 string 类型 的 map 中
            }                
        }
    }
    fout << curLine << std::endl;
}


const readCSV::DateFrame readCSV::DateFrame::__GetData(const std::string& start, const std::string& end, const std::vector<std::string>& columns, const char* file)
{
    std::ofstream fout(file);
    std::string res(index_label);
    for (auto col:column)               // 输出 column 行
    {
        auto iter = find(columns.begin(), columns.end(), col);
        if (iter != columns.end())
        {
            res += ','+col;
        }
    }
    fout << res << std::endl;

    int low = find(index.begin(), index.end(), start)-index.begin();
    int high = find(index.begin(), index.end(), end)-index.begin();
    for (int i=low; i<=high; ++i)
    {
        writeToFile(fout, columns, i);  
    }
    fout.close();
    
    std::map<std::string, DataType> dtypes;
    for (auto c:columns)
    {
        if (intColumn.count(c) > 0)
        {
            dtypes[c] = kInt64;
        }
        else if (doubleColumn.count(c) > 0)
        {
            dtypes[c] = kDouble;
        }
        else if (stringColumn.count(c) > 0)
        {
            dtypes[c] = kString;
        }
    }

    DateFrame temp;
    temp.FromCsv(file, dtypes);
    return temp;
}