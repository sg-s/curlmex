# curlmex


`curlmex` is a wrapper around [cURL](https://curl.se/) that allows you to use curl from within MATLAB. 


## Why use curlmex

1. You don't have cURL on your computer, and you stubbornly don't want to install it. 
2. You don't want to use cURL using `system` (which can leak sensitive information in your diary)
3. You want to build a logger/analytics daemon for your MATLAB toolbox

## Installation 

Compile using:


```matlab
mex -Llib -lcurl curlmex.c
```

