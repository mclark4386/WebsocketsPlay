#include <iostream>
#include "websocket_helpers.hpp"

using namespace std;

string test_key = "dGhlIHNhbXBsZSBub25jZQ==";

string print_bool(bool _b){
  return _b ? "true" : "false";
}

int main(int argc, char *argv[])
{
  char* data = new char[10];

  data[0] = 0b10000000;
  data[1] = 0b00000100;
  data[2] = 'H';
  data[3] = 'i';
  data[4] = '!';
  data[5] = 0;
  WebSocketPacket testJustFin{data};//fin should be true

  data[0] = 0b01110000;
  WebSocketPacket testJustRSV{data};//fin should be false, rsv should be 7

  data[0] = 0b11110000;
  WebSocketPacket testFinAndRSV{data};//fin should be true, rsv should be 7

  cout << "just fin{ fin:"<<endl;
  print_packet(testJustFin);
  cout << "just RSV{ fin:"<<endl;
  print_packet(testJustRSV);
  cout << "just fin&RSV"<<endl;
  print_packet(testFinAndRSV);

  data[0] = 0b10010000;
  WebSocketPacket testFinAndRSV1{data};//fin should be true, rsv should be 1
  cout << "just fin&RSV(1)"<<endl;
  print_packet(testFinAndRSV);

  cout<< "Is valid from client packet:"<<print_bool(validFromClientPacket(testFinAndRSV))<<endl;

  cout<<"key for key:"<<websocketKeyCalculate(test_key)<<endl;

  cout<<"\nclient request:"<<createClientHandshakeRequest("/","localhost","80",test_key)<<endl;

  cout<<"\nserver response:"<<createServerResponse(createClientHandshakeRequest("/","localhost","80",test_key))<<endl;

  cout<<"\nserver response with post:"<<createServerResponse("POST / HTTP/1.1\nHost: localhost:80\nSec-WebSocket-Key: bluebirdssmilinatme\n")<<endl; 

  cout<<"\nserver response with no key:"<<createServerResponse("GET / HTTP/1.1\nHost: localhost:80\n")<<endl;

  cout<<"\nget path from request:"<<getValidGetPath(createClientHandshakeRequest("/","localhost","80",test_key))<<endl;

  /*  cout<<"string to sha1:";
  string test = "";
  cin >> test;
  cout<<"sha1("<<test<<"):"<<sha1(test)<<endl;
  */
  return 0;
}
