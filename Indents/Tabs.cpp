
#include "Tabs.h"

std::string TabsFormatter (std::string FNAME){
    std::vector<std::string> v;
    char ch;
    int pos = 0;
    int number = 0;
    std::string full;
    int flag = 0;
    int count = 0;
    std::string T = "\n    ";
    std::string D = "}";
    std::string P1 = "private:";
    std::string P2 = "public:";
    std::string P3 = "protected:";
    std::ifstream f(FNAME);
    while (f.get(ch)) {

        if (!pos) {
            v.emplace_back(std::string());
        }

        if (ch == '\n') {
            number++;
            pos = 0;
        }

        v[number].append(pos + 1, ch);
        count++;
    }


    for (int i = 0; i < count; i++) {
        full += v[i];
    }


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
            flag++;
        if (full[i] == '}')
            flag--;
        if (full[i] == '\n')
        {
            for (int j = 0; j < flag; j++)
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

    int ipri = full.find("private:");
    if (ipri != -1)
        full.replace(ipri - 4, 12, P1);

    int ipub = full.find("public:");
    if (ipub != -1)
        full.replace(ipub - 4, 11, P2);

    int ipro = full.find("protected:");
    if (ipro != -1)
        full.replace(ipro - 4, 14, P3);


    f.close();
    return full;
}
