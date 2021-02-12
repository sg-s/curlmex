// curlmex
// mex-compatibale version of a 
// simple wrapper around cURL
// which can be used to hit a server 
// with a request
//
// to compile, you will need a copy of libcurl
// that you can get from 
// https://curl.haxx.se/libcurl/
//
// compile using mex -Llib -lcurl curlmex.c
// and make sure this is in the right folder
// (usually the root of your libcurl)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mex.h"
#include <curl/curl.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // declare pointers to outputs
    double *output_state;


    char *input_buf, *output_buf;
    size_t buflen;
    
    /* check for proper number of arguments */
    if(nrhs!=1) 
      mexErrMsgIdAndTxt( "MATLAB:invalidNumInputs",
              "One input required.");
    else if(nlhs > 1) 
      mexErrMsgIdAndTxt( "MATLAB:maxlhs",
              "Too many output arguments.");

    /* input must be a string */
    if ( mxIsChar(prhs[0]) != 1)
      mexErrMsgIdAndTxt( "MATLAB:inputNotString",
              "Input must be a string.");

    /* input must be a row vector */
    if (mxGetM(prhs[0])!=1)
      mexErrMsgIdAndTxt( "MATLAB:inputNotVector",
              "Input must be a row vector.");
    
    /* get the length of the input string */
    buflen = (mxGetM(prhs[0]) * mxGetN(prhs[0])) + 1;

    /* allocate memory for output string */
    output_buf=mxCalloc(buflen, sizeof(char));

    /* copy the string data from prhs[0] into a C string input_ buf.    */
    input_buf = mxArrayToString(prhs[0]);
    

    char str[100];
	strcpy(str, "");
	strcat(str, input_buf);



    if(input_buf == NULL) {
      return;
    }

    // debug
    mexPrintf("url = %s\n",str);

    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    chunk.size = 0;    /* no data at this point */ 


    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, str);

        // read out data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
          mexPrintf("curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        /* always cleanup */

        curl_easy_cleanup(curl);
    }


    // attempt to read out 
    mexPrintf(chunk.memory);
}





