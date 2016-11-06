#include <iostream>
#include "websocket_helpers.hpp"

using namespace std;

void print_packet(WebSocketPacket& packet);

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

  return 0;
}

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
