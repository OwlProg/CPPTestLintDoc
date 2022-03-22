
#include "Tabs.h"

std::string TabsFormatter (const std::string &FNAME){
    std::vector<std::string> v;
    char ch;
    int pos = 0;
    int number = 0;
    std::string full;
    int key = 0;
    int count = 0;
    const std::string T = "\n    ";
    const std::string D = "}";
    const std::string P1 = "private:";
    const std::string P2 = "public:";
    const std::string P3 = "protected:";

        std::ifstream file;
        file.open(FNAME);

        std::stringstream temp;
        temp << file.rdbuf();

        full = temp.str();

        temp.clear();

        for (int i = 0; i < full.size(); i++)
        {
            if (full[i] == '\n')
            {
                int k = 1;
                while (full[i + k] == ' ')
                {
                full.erase(i+k,1);
                }
            }
        }

    for (int i = 0; i < full.size(); i++)
    {
        if (full[i] == '{')
            key++;
        if (full[i] == '}')
            key--;
        if (full[i] == '\n')
        {
            for (int j = 0; j < key; j++)
            {
                full.replace(i, 1, T);
            }
        }
    }

    for (int i = 0; i < full.size(); i++)
    {
        if (full[i] == '}' && full[i-1] == ' ')
        {
            full.replace(i-4, 5, D);
        }

    }
    int ipri = full.find(P1);
    if (ipri != std::string::npos)
        full.replace(ipri - 4, P1.length()+4, P1);

    int ipub = full.find(P2);
    if (ipub != std::string::npos)
        full.replace(ipub - 4, P2.length()+4, P2);

    int ipro = full.find(P3);
    if (ipro != std::string::npos)
        full.replace(ipro - 4, P3.length()+4, P3);
    file.close();
    return full;
}
