#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <random>
#include <boost/tokenizer.hpp>
#include <twitcurl.h>
#include <oauthlib.h>
#include "json.hpp"
#include <regex>

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
                    /*
                    //old... prob slow
                    map<string,vector<MarcovWord>>::iterator iter; 
                    for(iter = marcovDict.begin(); iter != marcovDict.end(); iter++){
                        if(!(*iter).first.compare(currentWord)){ //find the key that we want to add the value to
                            addToKey(nextWord,(*iter).second);
                            break;
                        }
                    }
                    */
                   //new test way
                   addToKey(nextWord,marcovDict.find(currentWord)->second);
                    
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
            int r = rand()/(RAND_MAX/(marcovDict.size()-1));
            //int r = randomnums(rng);
            int prob = words[0].prob;
            //cout << to_string(prob) + "... ";
            for(int i = 0; i < words.size()-1;i++){
                if(r <= words[i].prob){
                    nextWord = words[i].word;
                    return;
                }
                prob += words[i+1].prob;
            }
            //cout<< to_string(prob) + ": " + words[words.size()-1].word << endl;
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
                    double probability = ((*iter).second[i].count / (double)totalCount) * 10000;
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

void printUsage()
{
    printf( "\nUsage:\ntwitterClient -u username -p password\n" );
}

int main(int argc, char* argv[]){

    /*
    //handle opening the file holding the text
    ifstream inputFile;
    inputFile.open("fullfile.txt");
    stringstream strStream;
    strStream << inputFile.rdbuf();
    */

        /* Get username and password from command line args */
    std::string userName( "" );
    std::string passWord( "" );
    if( argc > 4 )
    {
        for( int i = 1; i < argc; i += 2 )
        {
            if( 0 == strncmp( argv[i], "-u", strlen("-u") ) )
            {
                userName = argv[i+1];
            }
            else if( 0 == strncmp( argv[i], "-p", strlen("-p") ) )
            {
                passWord = argv[i+1];
            }
        }
        if( ( 0 == userName.length() ) || ( 0 == passWord.length() ) )
        {
            printUsage();
            return 0;
        }
    }
    else
    {
        printUsage();
        return 0;
    }

    twitCurl twitterObj;
    std::string tmpStr, tmpStr2;
    std::string replyMsg;
    char tmpBuf[1024];

    /* Set twitter username and password */
    twitterObj.setTwitterUsername( userName );
    twitterObj.setTwitterPassword( passWord );

    /* Set proxy server usename, password, IP and port (if present) */
    memset( tmpBuf, 0, 1024 );
    printf( "\nDo you have a proxy server configured (0 for no; 1 for yes): " );
    fgets( tmpBuf, sizeof( tmpBuf ), stdin );
    tmpStr = tmpBuf;
    if( std::string::npos != tmpStr.find( "1" ) )
    {
        memset( tmpBuf, 0, 1024 );
        printf( "\nEnter proxy server IP: " );
        fgets( tmpBuf, sizeof( tmpBuf ), stdin );
        tmpStr = tmpBuf;
        twitterObj.setProxyServerIp( tmpStr );

        memset( tmpBuf, 0, 1024 );
        printf( "\nEnter proxy server port: " );
        fgets( tmpBuf, sizeof( tmpBuf ), stdin );
        tmpStr = tmpBuf;
        twitterObj.setProxyServerPort( tmpStr );

        memset( tmpBuf, 0, 1024 );
        printf( "\nEnter proxy server username: " );
        fgets( tmpBuf, sizeof( tmpBuf ), stdin );
        tmpStr = tmpBuf;
        twitterObj.setProxyUserName( tmpStr );

        memset( tmpBuf, 0, 1024 );
        printf( "\nEnter proxy server password: " );
        fgets( tmpBuf, sizeof( tmpBuf ), stdin );
        tmpStr = tmpBuf;
        twitterObj.setProxyPassword( tmpStr );
    }

    /* OAuth flow begins */
    /* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
    twitterObj.getOAuth().setConsumerKey( std::string( "jV2tpgvJ4tbaLYaiREEdeBuOk" ) );
    twitterObj.getOAuth().setConsumerSecret( std::string( "zr34AEijcy6V1drM0jULaqyXwkVSApUsw9MdUNsFcY8pQABcnZ" ) );

    /* Step 1: Check if we alredy have OAuth access token from a previous run */
    std::string myOAuthAccessTokenKey("");
    std::string myOAuthAccessTokenSecret("");
    std::ifstream oAuthTokenKeyIn;
    std::ifstream oAuthTokenSecretIn;

    oAuthTokenKeyIn.open( "twitterClient_token_key.txt" );
    oAuthTokenSecretIn.open( "twitterClient_token_secret.txt" );

    memset( tmpBuf, 0, 1024 );
    oAuthTokenKeyIn >> tmpBuf;
    myOAuthAccessTokenKey = tmpBuf;

    memset( tmpBuf, 0, 1024 );
    oAuthTokenSecretIn >> tmpBuf;
    myOAuthAccessTokenSecret = tmpBuf;

    oAuthTokenKeyIn.close();
    oAuthTokenSecretIn.close();

    if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
    {
        /* If we already have these keys, then no need to go through auth again */
        printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

        twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
    }
    else
    {
        /* Step 2: Get request token key and secret */
        std::string authUrl;
        twitterObj.oAuthRequestToken( authUrl );

        /* Step 3: Get PIN  */
        memset( tmpBuf, 0, 1024 );
        printf( "\nDo you want to visit twitter.com for PIN (0 for no; 1 for yes): " );
        fgets( tmpBuf, sizeof( tmpBuf ), stdin );
        tmpStr = tmpBuf;
        if( std::string::npos != tmpStr.find( "1" ) )
        {
            /* Ask user to visit twitter.com auth page and get PIN */
            memset( tmpBuf, 0, 1024 );
            printf( "\nPlease visit this link in web browser and authorize this application:\n%s", authUrl.c_str() );
            printf( "\nEnter the PIN provided by twitter: " );
            fgets( tmpBuf, sizeof( tmpBuf ), stdin );
            tmpStr = tmpBuf;
            twitterObj.getOAuth().setOAuthPin( tmpStr );
        }
        else
        {
            /* Else, pass auth url to twitCurl and get it via twitCurl PIN handling */
            twitterObj.oAuthHandlePIN( authUrl );
        }

        /* Step 4: Exchange request token with access token */
        twitterObj.oAuthAccessToken();

        /* Step 5: Now, save this access token key and secret for future use without PIN */
        twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

        /* Step 6: Save these keys in a file or wherever */
        std::ofstream oAuthTokenKeyOut;
        std::ofstream oAuthTokenSecretOut;

        oAuthTokenKeyOut.open( "twitterClient_token_key.txt" );
        oAuthTokenSecretOut.open( "twitterClient_token_secret.txt" );

        oAuthTokenKeyOut.clear();
        oAuthTokenSecretOut.clear();

        oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
        oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

        oAuthTokenKeyOut.close();
        oAuthTokenSecretOut.close();
    }
    /* OAuth flow ends */

    /* Account credentials verification */
    if( twitterObj.accountVerifyCredGet() )
    {
        twitterObj.getLastWebResponse( replyMsg );
        printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str() );
    }

    //Get all of Donald Trump's tweets
    if(twitterObj.timelineUserGet(true,false,200,"25073877",true,true)){
        twitterObj.getLastWebResponse( replyMsg );
            //printf( "\nList of Tweets: %s\n", replyMsg.c_str() );
    }

    //Now tokenize them and add the strings to a stringstream.
    int numtweets = 0;
    stringstream tweets;
    nlohmann::json tweetjson = nlohmann::json::parse(replyMsg);
    bool first = true;
    for (nlohmann::json::iterator it = tweetjson.begin(); it != tweetjson.end(); ++it) {
        auto textloc = (*it).find("full_text");
        if(textloc != (*it).end()) {
            //we found the tweet text
            //remove all newlines in the tweet text
            string tweettext = textloc.value().get<std::string>();
            tweettext.erase(remove(tweettext.begin(), tweettext.end(), '\n'), tweettext.end());
            tweettext.erase(remove(tweettext.begin(), tweettext.end(), '\r'), tweettext.end());
            //replace all &amp; with &
            tweettext = std::regex_replace(tweettext, std::regex("&amp;"), "   &");
            if(first){
            tweets << tweettext;
            first = false;
            }
            else{
                tweets << " " + tweettext;
            }
            ++numtweets;
        }
    }
    cout<< tweets.str() << endl << endl;

    cout << "Here is your generated tweet...\n";

    //seed the number and begin...
    srand(time(NULL));
    MarcovChain testingChain = MarcovChain(tweets.str());
    //Only do it once for now, expandable in the future...
    bool acceptable = false;
    stringstream tweet;
    while(!acceptable){
       
    int randNum = rand()/(RAND_MAX/((*testingChain.getChain()).size()-1));
    //int randNum = randomnums(rng);

    map<string,vector<MarcovWord>>::iterator iter = (*testingChain.getChain()).begin();
    advance(iter,randNum);
    tweet << iter->first;
    string currentWord(1000,' '); 
    testingChain.chooseNextWord(currentWord,(*iter).second);
    tweet << " " + currentWord + " ";
    for(int i=0; i < 140; i++){
        if((*testingChain.getChain()).find(currentWord) == (*testingChain.getChain()).end()){
            //we got to the end... stop
            break;
        }
        map<string,vector<MarcovWord>>::iterator it = (*testingChain.getChain()).find(currentWord);
        testingChain.chooseNextWord(currentWord,(*it).second);
        tweet << " " + currentWord + " ";
    }

    //now trim it down intelligently to 280 characters
    boost::char_separator<char> sep(" ");
    boost::tokenizer<boost::char_separator<char>> tokens(tweet.str(), sep);
    for (const auto& t : tokens) {
        if(tweet.str().length()+ string(" ").length() + t.length()<280){
        //tweet << t + " ";
        cout << t << endl;
        }
        else{
            break;
        }
    }


    tweet << endl;



    cout << tweet.str();
    //make sure the tweet is good via user input
    printf( "\nGenerate a different tweet? (y/n)" );
    fgets( tmpBuf, sizeof( tmpBuf ), stdin );
    tmpStr = tmpBuf;
    if( std::string::npos != tmpStr.find( "n" ) )
    {
        acceptable = true;
    }
    else{
        tweet.str(string());
    }

    }
    

    return 0;
}




