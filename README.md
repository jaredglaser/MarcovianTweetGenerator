# MarcovianTweetGenerator

The marcov chain algorithm used is essentially what is described in https://drmysterian.com/how-to-code-a-python-markov-chain-text-generator/, but using 2d arrays to store the parsed data.

# Dependencies
Boost >= 1.61.0

OpenSSL >= 1.1

zlib

cmake >= 3.13.4

# Installation in Ubuntu environment

Gather Dependencies:

  `sudo apt install libboost-all-dev libssl-dev zlib1g cmake`
  
Clone this Repository:

  `git clone https://github.com/jaredglaser/MarcovianTweetGenerator`
  
Clone the Twitter API Repo:

  `git clone github.com/a-n-t-h-o-n-y/Twitter-API-C-Library`
  
Make the Twitter API Repo:

  `cd Twitter-API-C-Library`
  
  `cmake .`
  
Go back to the clone of this Repo, make it, then run it:

  `cd ..\MarcovianTweetGenerator`
  
  `make`
  
  `./a.out`
  

