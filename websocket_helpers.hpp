#include <iostream>
#include "websocket_packet.hpp"

using namespace std;

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
