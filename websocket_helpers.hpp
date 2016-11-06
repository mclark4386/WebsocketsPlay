#include <cstdint>

struct WebSocketPacket
{
  const char* raw;
  char fin; //is bool, but most systems will have bool be bigger than char.... so just treat it like a bool
  char rsv;
  char opcode;
  char mask_bit;//is bool
  uint64_t payload_length;
  char mask_key[4];
  char* decoded_payload;

  WebSocketPacket(const char* _raw):raw(_raw){
    int offset = 0;
    char eine_char = this->raw[offset];
    this->fin = ((eine_char & 0b10000000) == 0b10000000);
    this->rsv = ((eine_char & 0b01110000) >> 4);
    this->opcode = (eine_char & 0b00001111);

    //move to the next byte
    eine_char = this->raw[++offset];
    this->mask_bit = ((eine_char & 0b10000000) == 0b10000000);
    this->payload_length = (eine_char & 0b01111111);
    if(this->payload_length == 126){//get next 16bits
      this->payload_length = (int(this->raw[offset+1]) << 8) + int(this->raw[offset+2]);
      offset +=2;
    }else if(this->payload_length == 127){//get next 64bits
      this->payload_length = (uint64_t(this->raw[offset+1]) << 24) +(uint64_t(this->raw[offset+2]) << 16) +(uint64_t(this->raw[offset+3]) << 8) + uint64_t(this->raw[offset+4]);
      offset += 4;
    }

    //payload_length should not have the actual length in it
    this->decoded_payload = new char[this->payload_length];
    
    if(this->mask_bit){
      //read mask_key
      for(int i=0;i<4;i++) { this->mask_key[i] = this->raw[++offset]; }
      //demask
      offset++;
      for(int i=0; i < this->payload_length; i++){
	this->decoded_payload[i] = this->raw[offset+i] ^ this->mask_key[i % 4];
      }
    }else{
      //copy
      offset++;
      for(int i=0; i < this->payload_length; i++){
	this->decoded_payload[i] = this->raw[offset+i];
      }
    }
  }
};
