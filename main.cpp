#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include <string>
#include <regex>

using std::cout;
using std::regex;
using std::string;
using std::vector;
using std::endl;

int main(){
    std::ifstream input("data.txt");
    
    if(!input.good()){
        std::cout << "Failas nerastas!";
        return 1;
    }
    
    string line;
    regex r = regex("(((http|https)://)?www\\.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
    std::ofstream output("result.txt");
    std::map<string, int> occurences;
    std::map<string, vector<int>> crossCheck;
    vector<string> url;
    
    int n = 0;
    while (!input.eof()){
        crossCheck.clear();
        n++;
        getline(input, line);

        std::stringstream stream(line);
        string word;

        int numberedWord = 0;
        while (stream >> word){
            numberedWord++;
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });

            if(std::regex_match(word, r)) {
                if (word[word.length() - 1] == ',' || word[word.length() - 1] == '.'){
                    word.resize(word.length() - 1);
                }   
                url.push_back(word);
                word.clear();
            }
            if (isdigit(word[0]) || ispunct(word[0])){
                if (word.find("th") == string::npos){
                    for (int i = 0; i < word.length(); i++){
                        if (isdigit(word[i]) || ispunct(word[i])){
                            word = word.substr(i + 1);
                            i = -1;
                        }
                    }
                }
            }

            if (word[word.length() - 2] == '\''){
                word.resize(word.length() - 2);
            }
            
            if (isdigit(word[word.length() - 1]) || ispunct(word[word.length() - 1])){
                if(word[word.length() - 2] == 'h' && word[word.length() - 3] == 't' && ispunct(word[word.length() - 1])){
                    word.resize(word.length() - 1);
                }
                    
                if (word[word.length() - 1] != 'h' && word[word.length() - 2] != 't'){
                    for (int i = word.length() - 1; i > 0; i--){
                        if (isdigit(word[i]) || ispunct(word[i])){
                            word.resize(i);
                            i = word.length();
                        }
                    }
                }
            }
            
            if (!word.empty()){
                crossCheck.insert({ word, {0} });
                crossCheck[word].push_back(numberedWord);

                occurences.insert({ word, 1 });
                occurences[word]++;
            }
        }

        output << n << "-osios eilutes zodziu numeriai: " << endl;
        for (auto elem : crossCheck){
            if (elem.second.size() > 1){
                output << elem.first << " ";
            }
            
            for (int i = 1; i < elem.second.size(); i++){
                output << elem.second.at(i) << " ";
            }

            output << endl;
        }
        output << endl;
    }
    
    output << "Bendras zodziu skaicius: " << endl;
    for (auto elem : occurences){
        if (elem.second > 1){
            output << elem.first << " " << elem.second << endl;
        }
        output << endl;
    }

    output << "URL: " << endl;
    for (auto &elem : url){
        output << elem << endl;
    }

    input.close();
    output.close();
    return 0;
}