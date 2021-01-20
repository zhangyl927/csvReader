#include "DateFrame.h"

int main(int argc, const char* argv[])
{
    string path = argv[1];
    map<string, DataType> dtypes = {{"localtime", kInt64}, {"prev_close", kDouble}, {"ask_prc1", kDouble}, {"bid_vol3", kInt64}};

    DateFrame data;

    // 从csv中读取数据建立DataFrame
    data.FromCsv(argv[1], dtypes);

    // 输出DataFrame到csv文件中
    const char* resultFile = "../outPut/ToCsvFile.csv";
    vector<string> columns = {"localtime", "prev_close", "ask_prc1", "bid_vol3"};
    data.ToCsv(resultFile, columns);
    
    // 查找单个值
    string getData = data.GetData("2019-06-11 09:15:09.000", "ask_prc1");
    cout << "查找单个值 ('2019-06-11 09:15:09.000', 'ask_prc1') : " << getData << endl;

    // 按照index和columns查找多个值
    vector<string> cs = {"localtime", "prev_close", "ask_prc1"};
    data.GetData("2019-06-11 09:16:57.000", "2019-06-11 09:19:21.000", cs);

    // 按照行列号查找单个值
    string getSingle = data.GetData(4, 2);
    cout << "按照行列号查找单个值 (4, 2) : " << getSingle << "\n";

    // 按照行列号查找多个值
    DateFrame newData = data.GetData(5, 10, 1, 3);
    cout << "按照行列号查找多个值 (5, 10, 1, 3) : "  << "\n";

    // 重载赋值运算符
    DateFrame testAssignObj;
    testAssignObj = newData;
    resultFile = "../outPut/operater=.csv";
    testAssignObj.ToCsv(resultFile, columns);
    cout << "重载赋值运算符 结果保存在 " << resultFile << endl;

    // 按某一列排序
    DateFrame sortedObj = data.Sort1("ask_prc1", true, true);
    vector<string> sortRes = {"localtime", "prev_close", "ask_prc1", "bid_vol3"};
    resultFile = "../outPut/sortResult.csv";
    sortedObj.ToCsv(resultFile, sortRes);
    cout << "按某一列排序 结果保存在 " << resultFile << endl;

    // operator +
    DateFrame operaedHelpObj(data);
    DateFrame operaedObj = operator+(data, operaedHelpObj);
    vector<string> operatorRes =  {"localtime", "prev_close", "ask_prc1", "bid_vol3"};
    resultFile = "../outPut/operatorResult.csv";
    operaedObj.ToCsv(resultFile, operatorRes);
    cout << "operator + 结果保存在 " << resultFile << endl;

    // 填充数据
    //Fill(columns，method=”back”)

    return 0;
}