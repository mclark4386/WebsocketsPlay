#ifndef WEBSOCKET_HELPERS_H
#define WEBSOCKET_HELPERS_H

#include <iostream>
#include <string>
#include <sstream>

#include "websocket_packet.hpp"
#include "sha1.h"
#include "base64.hpp"


using namespace std;

const string websocket_magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

void print_packet(WebSocketPacket& packet){
  cout<<"WebSocketPacket{"<<endl;
  cout<<"\tfin:\t"<<(packet.fin == true);
  cout<<"\trsv:\t"<<int(packet.rsv);
  cout<<"\topcode:\t"<<int(packet.opcode);
  cout<<"\tmask_bit:\t"<<(packet.mask_bit == true);
  cout<<"\tpayload_length:\t"<<packet.payload_length;
  cout<<"\tmake_key:\t"<<packet.mask_key[0]<<packet.mask_key[1]<<packet.mask_key[2]<<packet.mask_key[3]<<endl;
  cout<<"\tdecoded_payload:\t"<<string(packet.decoded_payload, packet.payload_length)<<endl;
  cout<<"}"<<endl;
}

bool validFromClientPacket(WebSocketPacket& packet){
  return (packet.mask_bit == true);
}

string websocketKeyCalculate(string key){
  return base64_encode(sha1(key+websocket_magic_string));
}

//return the path or an empty string if there is no path or the method isn't GET
string getValidGetPath(string client_request){
  stringstream ss(client_request);
  string method, path;
  if(ss.good()){
    getline(ss, method, ' ');
    if(method != "GET") return ""; 
    if(ss.good()) getline(ss, path, ' ');
  }
  return path;
}

string createClientHandshakeRequest(string path, string host, string port, string key){
  ostringstream os;
  os<<"GET "<<path<<" HTTP/1.1\n"
    <<"Host: "<<host<<":"<<port<<"\n"
    <<"Upgrade: websocket\n"
    <<"Connection: Upgrade\n"
    <<"Sec-WebSocket-Key: "<<key<<"\n"
    <<"Sec-WebSocket-Version: 13\n\n";
  return os.str();
}

string createServerResponse(string client_request){
  stringstream ss(client_request);
  string line, method, path, key;
  if(ss.good()){
    getline(ss, method, ' ');
    if(ss.good()) getline(ss, path, ' ');// incase we need the path later
    getline(ss, line,'\n');
    line="";
    while(ss.good()){
      getline(ss, line, '\n');
      stringstream subss(line);
      if(subss.good()){
	  string k, v;
	  getline(ss, k, ':');
	  while(ss.peek() == ' ') ss.ignore();
	  if(subss.good()) getline(ss,v);
	  if(k == "Sec-WebSocket-Key" && v != "") key = v;
	}
	line="";
	}
  }
  if(key == "" || method != "GET"){
    return "HTTP/1.1 500 Bad Request\nContent-Length: 0\nConnection: close\n\n";
  }

  ostringstream os;
  os<<"HTTP/1.1 101 Switching Protocols\n"
    <<"Update: websocket\n"
    <<"Connection: Upgrade\n"
    <<"Sec-WebSocket-Accept: "<<websocketKeyCalculate(key)<<"\n\n";
  return os.str();
}

#endif /* WEBSOCKET_HELPERS_H */
