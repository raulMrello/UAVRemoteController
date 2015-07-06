#ifndef GPS_UBLOX_h
#define GPS_UBLOX_h

#include "mbed.h"

#define UBX_MAXPAYLOAD 60

class GPS_UBLOX
{

  public:
    // Methods
	GPS_UBLOX();	
	enum Errcode {BAD_PAYLOAD, BAD_CRC, PENDING, OK};
	void Init();
	Errcode Read(uint8_t data);
	// Properties
	long Time;          //GPS Millisecond Time of Week
	long Lattitude;     // Geographic coordinates
	long Longitude;
	long Altitude;
	long Ground_Speed;
	long Speed_3d;      //Speed (3-D)
	long Ground_Course;
	uint8_t NumSats;      // Number of visible satelites
	uint8_t Fix;        // 1:GPS FIX   0:No FIX (normal logic)
  
  private:
    // Internal variables
	uint8_t ck_a;     // Packet checksum
	uint8_t ck_b;
	uint8_t UBX_step;
	uint8_t UBX_class;
	uint8_t UBX_id;
	uint8_t UBX_payload_length_hi;
	uint8_t UBX_payload_length_lo;
	uint8_t UBX_payload_counter;
	uint8_t UBX_buffer[UBX_MAXPAYLOAD];
	uint8_t UBX_ck_a;
	uint8_t UBX_ck_b;
	long UBX_ecefVZ;
	Errcode parse_ubx_gps();
	void ubx_checksum(uint8_t ubx_data);
	long join_4_bytes(uint8_t Buffer[]);

};


#endif
