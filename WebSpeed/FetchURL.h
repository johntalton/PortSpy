#ifndef FETCH_URL_H
#define FETCH_URL_H
#include <NetBuffer.h> 
#include <NetEndpoint.h> 
#include <String.h>

class FetchUrl
{ 
public: 
        FetchUrl(const char *host_name, 
                const char *path_to_file = "index.html", 
                uint16 port = 80); 

        void SetTo(const char *host_name, 
                const char *path_to_file = "index.html", 
                uint16 port = 80); // like constructor 

        int32 Fetch(); // called to start connection, 
                          // returns when connection is closed 
                          // and retuns the amount of bytes read
        
        const char *RawResponse(); // returns exactly what the server spit out 
        const char *ParsedResponse(); // returns just the webpage 
                                      // that we received from the server 
protected: 
        BString fRaw, fParsed, fHost, fPath; 
        uint16 fPort; 
}; 

#endif