#include "src/DateFrame.h"
#include <iostream>

int main(int argc, const char* argv[])
{
    std::string path = argv[1];
    std::map<std::string, readCSV::DataType> dtypes = {{"localtime", readCSV::kInt64}, {"prev_close", readCSV::kDouble}, {"ask_prc1", readCSV::kDouble}, {"bid_vol3", readCSV::kInt64}};

    readCSV::DateFrame data;

    // 从csv中读取数据建立DataFrame
    data.FromCsv(argv[1], dtypes);

    // 输出DataFrame到csv文件中
    const char* resultFile = "../outPut/ToCsvFile.csv";
    std::vector<std::string> columns = {"localtime", "prev_close", "ask_prc1", "bid_vol3"};
    data.ToCsv(resultFile, columns);
    
    // 查找单个值
    std::string getData = data.GetData("2019-06-11 09:15:09.000", "ask_prc1");
    std::cout << "查找单个值 ('2019-06-11 09:15:09.000', 'ask_prc1') : " << getData << std::endl;

    // 按照index和columns查找多个值
    std::vector<std::string> cs = {"localtime", "prev_close", "ask_prc1"};
    data.GetData("2019-06-11 09:16:57.000", "2019-06-11 09:19:21.000", cs);

    // 按照行列号查找单个值
    std::string getSingle = data.GetData(4, 2);
    std::cout << "按照行列号查找单个值 (4, 2) : " << getSingle << "\n";

    // 按照行列号查找多个值
    readCSV::DateFrame newData = data.GetData(5, 10, 1, 3);
    std::cout << "按照行列号查找多个值 (5, 10, 1, 3) : \n";

    // 重载赋值运算符
    readCSV::DateFrame testAssignObj;
    testAssignObj = newData;
    resultFile = "../outPut/operater=.csv";
    testAssignObj.ToCsv(resultFile, columns);
    std::cout << "重载赋值运算符 结果保存在 " << resultFile << std::endl;

    // 按某一列排序
    readCSV::DateFrame sortedObj = data.Sort1("ask_prc1", true, true);
    std::vector<std::string> sortRes = {"localtime", "prev_close", "ask_prc1", "bid_vol3"};
    resultFile = "../outPut/sortResult.csv";
    sortedObj.ToCsv(resultFile, sortRes);
    std::cout << "按某一列排序 结果保存在 " << resultFile << std::endl;

    // operator +
    readCSV::DateFrame operaedHelpObj(data);
    readCSV::DateFrame operaedObj = operator+(data, operaedHelpObj);
    std::vector<std::string> operatorRes =  {"localtime", "prev_close", "ask_prc1", "bid_vol3"};
    resultFile = "../outPut/operatorResult.csv";
    operaedObj.ToCsv(resultFile, operatorRes);
    std::cout << "operator + 结果保存在 " << resultFile << std::endl;

    // 填充数据
    //Fill(columns，method=”back”)

    return 0;
}