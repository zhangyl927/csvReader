#include "DateFrame.h"

//double strToDou(const string& str);
//uint64_t strToInt(string& str);

inline DateFrame& DateFrame::operator=(const DateFrame& rhs)
{
    cout << "operator=(const DateFrame)\n";
    if (this == &rhs) return *this;

    this->index_label = rhs.index_label;
    this->column = rhs.column;
    this->index = rhs.index;
    this->intColumn = rhs.intColumn;
    this->doubleColumn = rhs.doubleColumn;
    this->stringColumn = rhs.stringColumn;
    return *this;
}

inline DateFrame& DateFrame::operator=(DateFrame&& rhs)
{
    // cout << "operator=(DateFrame&&)\n";
    // if (this != &rhs)
    // {
    //     swap(index_label, rhs.);
    // }
}

inline void DateFrame::FromCsv(const char* path, map<string, DataType>& dtypes)
{
    vector<vector<string>> allData;                 // ���������ݱ��浽 allData ��;
    ifstream fin(path, std::ifstream::in);

    string line;                                   // ���ڱ������У�
    while (getline(fin, line))                     // ��ȡһ�У�
    {
        vector<string> curLine;
        istringstream sin(line);
        string singleString;
        while (getline(sin, singleString, ','))
        {
            curLine.push_back(singleString);        // ����ǰ����ÿ��string ���������
        }
        allData.push_back(curLine);                 // ����ǰ�д��������
    }

    index_label = allData[0][0];                    // index_label = datetime;

    int n = allData.size();
    index.resize(n);
    for (int i=1; i<n; ++i)            // �����һ�� index;
    {
        index.push_back(allData[i][0]);
    }


    // ��ȡ dtypes ���ݺ����� kInt64, kDouble, kString
    for (auto dt:dtypes)
    {
        column.push_back(dt.first);             // �����һ������
        int idx = find(allData[0].begin(), allData[0].end(), dt.first)-allData[0].begin();
        
        if (kInt64 == dt.second)                 // dtype Ϊ kInt64 ����
        {
            vector<uint64_t> col;
            for (int i=1; i<allData.size(); ++i)
            {
                string str = allData[i][idx];
                uint64_t element = (""==str)?LLONG_MAX:strToInt(str);
                col.push_back(element);
            }
            intColumn[dt.first] = col;
        }
        else if (kDouble == dt.second)           // dtype Ϊ kDouble ����
        {
            vector<double> col;
            for (int i=1; i<allData.size(); ++i)
            {
                string str = allData[i][idx];
                double element = strToDou(str);
                col.push_back(element);
            }
            doubleColumn[dt.first] = col;
        }
        else if (kString == dt.second)            // dtype Ϊ kDouble ����
        {
            vector<string> col;
            for (int i=1; i<allData.size(); ++i)
            {
                string str = allData[i][idx];
                col.push_back(str);
            }
            stringColumn[dt.first] = col;
        }
    }

    return;
}

inline void DateFrame::ToCsv(const char* path, vector<string>& columns)
{
    ofstream fout(path);

    // �� column ��д���ļ���
    string row(index_label);             // �洢 column �У���һλΪ index_label;
    for (auto col:column)
    {
        auto iter = find(columns.begin(), columns.end(), col);
        if (iter != columns.end())
        {
            row += ','+col;
        }
        else row += ',';
    }
    fout << row << endl;

    // �� columns �е���д���ļ���
    for (int i=0; i<index.size(); ++i)
    {
        string curLine(index[i]);                                                    // index ��
        
        for (int j=0; j<column.size(); ++j)
        {
            auto cols = find(columns.begin(), columns.end(), column[j]);
            if (cols != columns.end())
            {
                if (intColumn.count(*cols) > 0)
                {
                    curLine += ","+to_string((intColumn[*cols])[i]);               // �� int ���� �� map ��
                }
                else if (doubleColumn.count(*cols) > 0)
                {
                    curLine += ","+to_string((doubleColumn[*cols])[i]);             // �� double ���� �� map ��
                }
                else if (stringColumn.count(*cols) > 0)
                {
                    curLine += ","+(stringColumn[*cols])[i];                        // �� string ���� �� map ��
                }                
            }
        }
        curLine += "\n";
        fout << curLine;
    }
    return;
}

// ���ҵ���ֵ
inline string DateFrame::GetData(const string& index_name, const string& column_name)
{
    string res;
    int row = find(index.begin(), index.end(), index_name)-index.begin();
    if (intColumn.count(column_name)==0 && doubleColumn.count(column_name)==0 && stringColumn.count(column_name)==0)
    {
        return res;
    }
    else if (intColumn.count(column_name) > 0)
    {
        res = to_string(intColumn[column_name][row]);
    }
    else if (doubleColumn.count(column_name) > 0)
    {
        res = to_string(doubleColumn[column_name][row]);
    }
    else if (stringColumn.count(column_name) > 0)
    {
        res = stringColumn[column_name][row];
    }

    return res;
}

// ����index��columns���Ҷ��ֵ��
inline DateFrame DateFrame::GetData(const string& start, const string& end, const vector<string>& columns)
{
    const char* file = "../outPut/getDataByColumns.csv";
    
    ofstream fout(file);
    string res(index_label);

    for (auto col:column)               // ��� column ��
    {
        auto iter = find(columns.begin(), columns.end(), col);
        if (iter != columns.end())
        {
            res += ','+col;
        }
    }
    fout << res << endl;

    int low = find(index.begin(), index.end(), start)-index.begin();
    int high = find(index.begin(), index.end(), end)-index.begin();
    for (int i=low; i<=high; ++i)
    {
        string curLine(index[i]);                                                    // index ��
        
        for (int j=0; j<column.size(); ++j)
        {
            auto cols = find(columns.begin(), columns.end(), column[j]);
            if (cols != columns.end())
            {
                if (intColumn.count(*cols) > 0)
                {
                    curLine += ","+to_string((intColumn[*cols])[i]);               // �� int ���� �� map ��
                }
                else if (doubleColumn.count(*cols) > 0)
                {
                    curLine += ","+to_string((doubleColumn[*cols])[i]);             // �� double ���� �� map ��
                }
                else if (stringColumn.count(*cols) > 0)
                {
                    curLine += ","+(stringColumn[*cols])[i];                        // �� string ���� �� map ��
                }                
            }
        }
        fout << curLine << endl;
    }
    fout.close();
    
    map<string, DataType> dtypes;
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

// �������кŲ��ҵ���ֵ
inline string DateFrame::GetData(int i, int j)
{
    if (i>index.size() || j>column.size()+1)
    {
        return "row or col out of variable range";
    }
    string column_name = column[j-2];
    string index_name = index[i-1];
    return GetData(index_name, column_name);
}

// �������кŲ��Ҷ��ֵ��
inline DateFrame DateFrame::GetData(int row_start, int row_end, int col_start, int col_end)
{
    if (row_start > row_end) 
    {
        sys_error("row_start be less than row_end\n");
    }
    if (col_start > col_end)
    {
        sys_error("col_start be less than col_end\n");
    }
    if (col_start>=column.size())
    {
        sys_error("col_start out of range\n");
    }
    if (col_end>=column.size())
    {
        col_end = column.size()-1;
    }

    string start = index[row_start-1];
    string end = index[row_end-1];

    vector<string> columns(col_end-col_start+1, "");
    for (int j=0; j<col_end-col_start; ++j)
    {
        columns[j] = column[j+col_start-1];
    }

    return GetData(start, end, columns);
}

// ��ĳһ������
inline DateFrame& DateFrame::Sort1(const string& column_name, bool ascend=true, bool copy=true)
{
    // �� column_name ���� DateFrame �У�ֱ�ӷ���
    if (intColumn.count(column_name)==0 && doubleColumn.count(column_name)==0 && stringColumn.count(column_name)==0)
    {
        return *this;
    }
    else if (intColumn.count(column_name) > 0)
    {
        vector<uint64_t> sortCol = intColumn[column_name];                              // Ҫ�������
        return helper(sortCol, ascend, copy);
    }
    else if (doubleColumn.count(column_name) > 0)
    {
        vector<double> sortCol = doubleColumn[column_name];
        return helper(sortCol, ascend, copy);
    }
    else if (stringColumn.count(column_name) > 0)
    {
        vector<string> sortCol = stringColumn[column_name];
        return helper(sortCol, ascend, copy);
    }

    return *this;
}

// operator +
const DateFrame operator+(DateFrame& lhs, DateFrame& rhs)
{
    int len1 = lhs.index.size(), len2 = rhs.index.size();
    int l1 = 0, l2 = 0;
    DateFrame opedObj;
    opedObj.index_label = lhs.index_label;

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
            /* �Ƚ� column[l1] �Ƿ���� column[l2] */
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

// string to int
uint64_t strToInt(string& str)
{
	int i = 0, point = 0, num = 0;
	uint64_t res = 0;
	for (; i < str.size(); ++i)
	{
		if ('.' == str[i]) point = i;
		if ('E' == str[i]) break;
	}
	if (i == str.size()) return atoi(str.c_str());
	
	for (int j = i + 2; j < str.size(); ++j)
	{
		num = num*10 + str[j] - '0';
	}

	int idx = 0;
	for (int k = 0; k < i; ++k)
	{
		if (point < k) idx = point - k;
		else if (point == k) continue;
		else idx = point-k-1;
		res = res + (str[k] - '0') * pow(10, num+idx);
	}

	return res;
}

// string to double
double strToDou(const string& str)
{
	int point = 0, idx = 0;
	for (; point < str.size(); ++point)
	{
		if ('.' == str[point]) break;
	}

	double res = 0;
	for (int k = 0; k < str.size(); ++k)
	{
		if (point < k) idx = point - k;
		else if (point == k) continue;
		else idx = point - k - 1;
		res = res + (str[k] - '0') * pow(10, idx);
	}

	return res;
}

template<class T>
DateFrame& DateFrame::helper(vector<T>& sortCol, bool ascend, bool copy)
{
    vector<T> temp(sortCol);
    if (false == ascend) sort(temp.begin(), temp.end(), greater<T>());                  // ��������
    else sort(temp.begin(), temp.end());                                                //  ����

    map<string, vector<uint64_t>> newIntColumn;                                         // key-������ value-�����ݣ�
    map<string, vector<double>> newDoubleColumn;
    map<string, vector<string>> newStringColumn;  

    vector<string> newIndex(index.size(), "");

    for (int i=0; i<temp.size(); ++i)
    {
        
        int row = find(sortCol.begin(), sortCol.end(), temp[i])-sortCol.begin();         // ��ȡ temp[i] �� sortCol ���ǵڼ��У�
        newIndex[i] = index[row];
        for (auto c1:intColumn)
        {
            string col_name = c1.first;
            newIntColumn[col_name].push_back(c1.second[row]);
        }
        for (auto c2:doubleColumn)
        {
            string col_name = c2.first;
            newDoubleColumn[col_name].push_back(c2.second[row]);
        }
        for (auto c3:stringColumn)
        {
            string col_name = c3.first;
            newStringColumn[col_name].push_back(c3.second[row]);
        }
    }

    //  �����µ� DataFrame
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

    //  ���ظ� DataFrame
    
    index = newIndex;
    intColumn = newIntColumn;
    doubleColumn = newDoubleColumn;
    stringColumn = newStringColumn;
    return *this;
}

void operateHelper(DateFrame& opedObj, DateFrame& lhs, DateFrame& rhs, int l1, int l2)
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