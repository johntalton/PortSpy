#include <stdio.h>

#include "Globals.h"
#include "FetchURL.h"


FetchUrl::FetchUrl(const char *host, const char *path, uint16 port) 
{ 
	// cache the info 
	fHost=host; 
	fPath=path; 
	if(path==NULL) 
		fPath="/index.html"; 
	fPort = port; 
}

void FetchUrl::SetTo(const char *host, const char *path, uint16 port) 
{ 
        // cache the info and reset the variables       
        fHost = host; 
        fPath = path; 
        if(path == NULL) 
                fPath = "/index.html"; 
        fPort = port; 
        fParsed = ""; 
        fRaw = ""; 
}

int32 FetchUrl::Fetch() 
{ 
        BNetEndpoint endpoint; 
        
        status_t status = endpoint.Connect(fHost.String(), fPort); 
        
        if(status != B_OK) 
                return -1; 
        
        BString send("GET "); 
        //if(fPath[0] != '/') 
        //        send += '/'; 
        send += fPath; 
        send += ' '; 
        send += "HTTP/1.0\r\n"; 
        //printf("SEND: %s\n",send.String());
        send.Append(USER_AGENT);

        
        int32 sent = endpoint.Send(send.String(), send.Length()); 

        if(sent == -1) 
                return -1; 
                
        send = "HOST: "; 
        send += fHost; 
        send += "\r\n\n"; 
        sent = endpoint.Send(send.String(), send.Length()); 

        send = "ACCEPT: */*\n\r";       
        sent = endpoint.Send(send.String(), send.Length()); 

        if(sent == -1) 
                return -1; 
        
        int32 amount = 1; 

        BNetBuffer received; 
        int32 bytecount = 0;
        while(amount > 0) { 
                amount = endpoint.Receive(received, 1024); 
                bytecount += amount;
               // printf("%s",received.Data());
        } 
        //fRaw = (char *)received.Data(); 
        //fParsed = fRaw; 
        
        //int32 i = fRaw.FindFirst("\n\r"); 
        
        
        //int32 i = fRaw.FindFirst("\n\r"); 
        //if(i==-1)return(B_ERROR);		//Somebody smacked the door in our face!
	       
        
        
        //i+=2; // go past the \n\r 
        
        //fParsed = fRaw.CopyInto(fParsed, i, fRaw.Length() - i); 
        
        
        endpoint.Close();
        return bytecount; 
        
          
        
} 

/*
BString Fetch(const char *url) 
{       
        // parse url into host, path to file 
        BString host(url); 

        // if there is a http:// at the beginning, get rid of it 
        // exercise for the reader: handle a url like http://www.foo.com:8080/ 
        
        if(host.FindFirst("http://") != -1) { 
                host.Remove(0, 7); 
        } 

        // look for the first slash 
        int32 slash = host.FindFirst("/"); 

        BString path; 

        if(slash == B_ERROR) { 
                path = "index.html";            
        } 
        else { 
                // copy the path from the host string into path, 
                // then get rid of the path from the host string 
                path = host.CopyInto(path, slash, host.Length() - slash); 
                host.Truncate(slash, true); 
        } 
                
        FetchUrl earl(host.String(), path.String()); 
        if(earl.Fetch() == B_OK) { 
                return earl.ParsedResponse(); 
        } 
        return "error"; 
} 
*/