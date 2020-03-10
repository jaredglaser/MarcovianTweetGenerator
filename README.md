# MarcovianTweetGenerator

The marcov chain algorithm used is essentially what is described in https://drmysterian.com/how-to-code-a-python-markov-chain-text-generator/, but using a map with a word as the key and a vector holding the possible next words with their probabilities. The program picks a random word to start with and then chooses a random number and compares it against the probabilites of each of the possible next words. Whichever next word is chosen by the random number is then located in the map... this will loop until the full tweet is formed.

#Twitter Handle VS File

This can take data from either a file or from a twitter handle. 

Do not include the @ symbol for the handle. Pay attention to caps!

The text should reside in input.txt if you want to ingest from a file.

# Dependencies

Boost >= 1.61.0

Make

OpenSSL >= 1.1

Ryan Geary's fork of twitcurl (https://github.com/theryangeary/twitcurl)

# Installation in Ubuntu environment (tested on Amazon EC2 Ubuntu 18.04)

Gather Dependencies:
  
  `sudo apt update`
  
  `sudo apt install libboost-all-dev libssl-dev make libcurl4-gnutls-dev`
  
Clone this Repository:

  `git clone https://github.com/jaredglaser/MarcovianTweetGenerator`
  
Clone the Twitter API Repo: I'm using Ryan Geary's fork that allows me to get the extended versions of the tweets.

  `git clone https://github.com/theryangeary/twitcurl.git`
  
Make twitcurl:

  `cd twitcurl/libtwitcurl`
  
  `sudo make`
  
Move the library file to /usr/lib/. You may have to copy it as both libtwitcurl.so and libtwitcurl.so.1

  `sudo cp libtwitcurl.so.1.0 /usr/lib/libtwitcurl.so`
  
You may also have to place the directory called /include/curl in /usr/include

  `sudo cp -r include/curl /usr/include`
  
Update the linker so that it can find the new library files we just put into /usr/lib

`sudo /sbin/ldconfig`
 
Go back to the clone of this Repo, make it, then run it:

  `cd ../../MarcovianTweetGenerator`
  
  
  `make`
  
  `./a.out -u twitteremail@email.com -p yourtwitterpassword`
 
  

