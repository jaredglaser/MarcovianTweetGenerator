#include <iostream>
#include<string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
using namespace std;

class MarcovWord
{
    public:
    string word;
    double prob;
    int count;
    MarcovWord(string word){
        this->word = word;
        this->count = 1;
        this->prob = 0;
    }
};
namespace std
{
template<> struct less<MarcovWord>
{
           bool operator() (const MarcovWord& lhs, const MarcovWord& rhs) const
       {
           return lhs.word < rhs.word;
}
};
}

// struct marcovWordComparator{
//     bool operator()(const marcovWord& first,const marcovWord& second)
//                 {
//                     return first.word.compare(second.word)==0 ? true : false ;
//                 }
// };

class MarcovChain{
    private: 
        map<string,vector<MarcovWord>> marcovDict;
    public:
        MarcovChain(string input){
            initDictionary(input);
        }
        map<string,vector<MarcovWord>>* getChain(){
            return &marcovDict;
        }
        void initDictionary(string input){
            vector<string> result;
            split(input,result);
            //Begin adding the elements. If the key already exists for a particular element
            //then increment the count for the element that already exists.
            for(int i=0; i<result.size()-1; i++){
                string currentWord = result[i];
                MarcovWord nextWord = MarcovWord(result[i+1]);

                if(marcovDict.count(currentWord) == 0){ //the word does not yet exist, add it!
                    marcovDict.emplace(currentWord,vector<MarcovWord> {nextWord});
                }
                else{ //it already exists
                    
                    //iterate through all of the values of the current word to see if the next word is already in there.
                    bool wordFound = false;
                    map<string,vector<MarcovWord>>::iterator iter; //TODO: end is being added to the first thing in the map. this is wrong i need to actually add it in the right place lmao
                    for(iter = marcovDict.begin(); iter != marcovDict.end(); iter++){
                        if(!(*iter).first.compare(currentWord)){ //find the key that we want to add the value to
                            addToKey(nextWord,(*iter).second);
                            break;
                        }
                    }
                    
                }
            }

            setProbabilities();
        }

        void addToKey(MarcovWord word, vector<MarcovWord> &words){
            for(int j = 0; j < words.size(); j++){
                            MarcovWord element = words[j];
                            if(!element.word.compare(word.word)){ 
                                //if they match then add 1 to the count
                                element.count++;
                            }
                        }
                        words.push_back(word);
                        
        }

        void chooseNextWord(string &nextWord, const vector<MarcovWord> &words){
            int r = rand() % 100;
            int prob = words[0].prob;
            for(int i = 0; i < words.size()-1;i++){
                if(r <= words[i].prob){
                    nextWord = words[i].word;
                    return;
                }
                prob += words[i+1].prob;
            }
            nextWord = words[words.size()-1].word;
        }

        void setProbabilities(){
            map<string,vector<MarcovWord>>::iterator iter; 
            for(iter = marcovDict.begin(); iter != marcovDict.end(); iter++){
                int totalCount = 0;
                for(MarcovWord element : (*iter).second){
                    totalCount += element.count;
                }
                
                for(int i = 0; i < (*iter).second.size(); i++){
                    //truncated to a int probability... may cause problems if there are large numbers of words following...
                    double probability = ((*iter).second[i].count / (double)totalCount) * 100;
                    (*iter).second[i].prob = (int)(probability);
                    string theword = (*iter).second[i].word;
                }
            }
        }

        void split(const string &s, vector<string> &words) {
            string buffer;
            stringstream ss(s);
            while(ss >> buffer){
                words.push_back(buffer);
            }
        }

        
};

int main(){
    //handle opening the file holding the text
    ifstream inputFile;
    inputFile.open("file.txt");
    stringstream strStream;
    strStream << inputFile.rdbuf();
    //seed the number and begin...
    srand(time(0));
    string testingstring = strStream.str();
    MarcovChain testingChain = MarcovChain(testingstring);
    int randNum = rand()%((*testingChain.getChain()).size()-1);
    map<string,vector<MarcovWord>>::iterator iter = (*testingChain.getChain()).begin();
    //advance(iter,randNum);
    string currentWord(1000,' '); 
    testingChain.chooseNextWord(currentWord,(*iter).second);
    cout << " " + currentWord + " ";
    for(int i=0; i < 10000; i++){
        if((*testingChain.getChain()).find(currentWord) == (*testingChain.getChain()).end()){
            //we got to the end... stop
            break;
        }
        map<string,vector<MarcovWord>>::iterator it = (*testingChain.getChain()).find(currentWord);
        testingChain.chooseNextWord(currentWord,(*it).second);
        cout << " " + currentWord + " ";
    }

    return 0;
}




