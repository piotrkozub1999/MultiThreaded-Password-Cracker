## MultiThreaded Password Cracker

The program is used to test the increase performance of application depending on the number of CPU threads used. 
The algorithm searches the English dictionary to crack the hashed word.
Three hashing methods are used:
- MD5
- SHA1
- SHA256

#### To execute project use on linux use command:

##### mkdir build && cd build && cmake .. && make && ./hash_cracker


Here you can see the results for MD5 alghoritm during 1000 hashes breaking:

### 1 Thread:
- Dictionary Loading: 378 ms
- Hashing:            432680ms
- Total Time:         433058ms       
- Time in minutes:    7:13


### 2 Threads:
- Dictionary Loading: 363 ms
- Hashing:            227922ms
- Total Time:         228285ms        
- Time in minutes:    3:48


### 4 Threads:
- Dictionary Loading: 367 ms
- Hashing:            146766ms
- Total Time:         147134ms         
- Time in minutes:    2:27


### 8 Threads:
- Dictionary Loading: 336 ms
- Hashing:            126794ms
- Total Time:         127131ms
- Time in minutes:    2:0
