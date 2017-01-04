/**
 * @file Create.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief Create class makes possible the creation of packets.
 */


#ifndef CREATE_H
#define CREATE_H


#include "ActionBase.h"
#include <vector>


using namespace std;

enum class type_t {
	UDP_SEND_COMMAND = 0,	// APP.0000
	UDP_BIND_COMMAND,		// APP.0001
	UDP_CONNECT_COMMAND,	// APP.0002
	UDP_CLOSE_COMMAND,		// APP.0003
	TCP_SEND_COMMAND,		// APP.0100
	TCP_OPEN_COMMAND,		// APP.0101
	
	TLREPORT_UDP_SEND,		// APP.0200
	TLREPORT_UDP_DATA,		// APP.0201
	CNCMD_UDP_SEND,			// APP.0300
	CNCMD_UDP_DATA,			// APP.0301
	
	
	UDP_DATA_INDICATION,	// APP.0004
	UDP_ERROR_INDICATION,	// APP.0005
	TCP_CONNECT_INFO,		// APP.0102
	TCP_ERROR_INFO,			// APP.0103
	TCP_COMMAND,			// APP.0104
	
	//A.S
	APPLICATION_PACKET, 	// APP.1000
	SEND_APPLICATION_PACKET,// APP.1001
	MEASUREMENT_DATA,       // APP.1002
	
	UDP_OVER_IPV4,			// TRA.0000
	//UDP_OVER_IPV6,		// TRA.0001
	
	TCP_OVER_IPV4,			// TRA.0010
	//TCP_OVER_IPV6,		// TRA.0011
	
	IPV4DATAGRAM,			// NET.0000
	IPV4DATAGRAM802CTRL,	// NET.0001
	//IPV6DATAGRAM,			// NET.0100
	//IPV6DATAGRAM802CTRL,	// NET.0101

	PPPFRAME,				// MAC.0000
	ETHERNETIIFRAME,		// MAC.0010
	IDEALAIRFRAME,			// MAC.0020
	AIRFRAME				// MAC.0030
	
};


class CreateInfo {
	public:
		int layer;
		type_t type;
	
	public:
		CreateInfo();
		~CreateInfo();
};


class Create : public ActionBase {
	private:
		vector<CreateInfo*> createInfos;

	private:
		void buildNewPacket(cPacket** packet, int layer, type_t type) const;
		type_t getType(int layer, string typeCode);
		bool isOuterPacket(int layer) const;
		
	public:
		/**
		 * @brief Constructor
		 */
		Create(const string layer5Type, const string layer4Type, const string layer3Type, const string layer2Type);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Create();

		/**
		 * @brief execute the create action
		 */
		void execute(cPacket** packet);
			
};

#endif
