#ifndef __NTP_CLIENT_H__
#define __NTP_CLIENT_H__
#pragma once
#include "includes.h"
#include "define.h"
#include <ctime>

/**
* NTP Fixed-Point Timestamp Format.
* From [RFC 5905](http://tools.ietf.org/html/rfc5905).
*/
struct Timestamp {
	unsigned int seconds;   /**< Seconds since Jan 1, 1900. */
	unsigned int fraction;  /**< Fractional part of seconds. Integer number of 2^-32 seconds. */

	/**
	* Reverses the Endianness of the timestamp.
	* Network byte order is big endian, so it needs to be switched before
	* sending or reading.
	**/
	void ReverseEndian(void);

	/**
	* Convert to time_t.
	* Returns the integer part of the timestamp in unix time_t format,
	* which is seconds since Jan 1, 1970.
	**/
	time_t to_time_t(void);
};

/**
* A Network Time Protocol Message.
* From [RFC 5905](http://tools.ietf.org/html/rfc5905).
*/
struct FlagsNTP
{
	BYTE mode : 3;           /**< Mode of the message sender. 3 = Client, 4 = Server */
	BYTE version : 3;        /**< Protocol version. Should be set to 3. */
	BYTE leap : 2;
};
struct NTPMessage {
	FlagsNTP flags;
	BYTE stratum;          /**< Servers between client and physical timekeeper. 1 = Server is Connected to Physical Source. 0 = Unknown. */
	BYTE poll;             /**< Max Poll Rate. In log2 seconds. */
	BYTE precision;        /**< Precision of the clock. In log2 seconds. */
	DWORD root_delay;       /**< Round-trip to reference clock. NTP Short Format. */
	DWORD root_dispertion;  /**< Dispersion to reference clock. NTP Short Format. */
	BYTE ref_clock_id[4];  /**< Reference ID. For Stratum 1 devices, a 4-byte string. For other devices, 4-byte IP address. */
	Timestamp ref;                  /**< Reference Timestamp. The time when the system clock was last updated. */
	Timestamp orig;                 /**< Origin Timestamp. Send time of the request. Copied from the request. */
	Timestamp rx;                   /**< Recieve Timestamp. Reciept time of the request. */
	Timestamp tx;                   /**< Transmit Timestamp. Send time of the response. If only a single time is needed, use this one. */

	/**
	* Reverses the Endianness of all the timestamps.
	* Network byte order is big endian, so they need to be switched before
	* sending and after reading.
	*
	* Maintaining them in little endian makes them easier to work with
	* locally, though.
	*/
	void ReverseEndian(void);

	/**
	* Recieve an NTPMessage.
	* Overwrites this object with values from the received packet.
	*/
	int recv(int sock);

	/**
	* Send an NTPMessage.
	*/
	int sendto(int sock, struct sockaddr_in* srv_addr);

	/**
	* Zero all the values.
	*/
	void clear();
};

#endif  /* __NTP_CLIENT_H__ */
