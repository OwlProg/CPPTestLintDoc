
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
        if (full[i] == '}')
        {
            full.replace(i-4, 5, D);
        }

    }
    f.close();
    return full;
}
