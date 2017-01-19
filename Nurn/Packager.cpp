#include "Packager.hpp"

Packager::Packager()
{
	this->transformQueue = new PacketQueue<TransformPacket>(20);
}

Packager::~Packager()
{
	if (this->transformQueue)
	{
		delete this->transformQueue;
		transformQueue = 0;
	}
}

unsigned char * Packager::getPacketPointer()
{
	return (unsigned char*)this->memory;
}

uint16_t Packager::getCurrentNetPacketSize() const
{
	return this->currentNetPacketSize;
}

void Packager::buildNetPacket()
{
	this->currentNetPacketSize = sizeof(uint16_t);

	this->addTransformPackets(this->currentNetPacketSize);
	
	//this->addPacketGroup(TRANSFORM_PACKET, (void*)TransformPacket pack, this->transformQueue, this->currentNetPacketSize);

	// this->currentNetPacketSize should now point at where next MetaDataPacket is to be added.
	// Add the rest of the packets here


	// Add the size of the netpacket at the start
	memcpy(this->memory, &this->currentNetPacketSize, sizeof(uint16_t));
}

void Packager::buildTransformPacket(const uint16_t &ID, const float& pos_x, const float& pos_y, const float& pos_z, const float& lookAt_x, const float& lookAt_y, const float& lookAt_z, const float& rotation_x, const float& rotation_y, const float& rotation_z)
{
	this->transformQueue->push(TransformPacket(ID, pos_x, pos_y, pos_z, lookAt_x, lookAt_y, lookAt_z, rotation_x, rotation_y, rotation_z));
}

void Packager::addTransformPackets(uint16_t &netPacketSize)
{
	//Grab and add all the transformpackets in a loop before adding the MetaDataPacket
	TransformPacket transformPacket;
	uint16_t sizeOfTransformPackets = 0;
	bool breakLoop = false;

	while (this->transformQueue->pop(transformPacket) && breakLoop == false)
	{
		// Only add a packet if there's enough space for another TransformPacket in the buffer
		if ((packetSize - (netPacketSize + sizeof(MetaDataPacket) + sizeOfTransformPackets)) > sizeof(TransformPacket))
		{
			// Add TransformPacket to the memory ( ...[MetaData][Transform][Transform]... )
			memcpy(this->memory + netPacketSize + sizeof(MetaDataPacket) + sizeOfTransformPackets, &transformPacket, sizeof(TransformPacket));
			sizeOfTransformPackets += sizeof(TransformPacket);
		}
		else		
		{
			breakLoop = true;
		}
	}

	this->addMetaDataPacket(TRANSFORM_PACKET, netPacketSize, sizeOfTransformPackets);

	netPacketSize += sizeOfTransformPackets; // Should now point at the location of the next MetaDataPacket
}

void Packager::addMetaDataPacket(uint16_t type, uint16_t &netPacketSize, uint16_t sizeInBytes)
{
	memcpy(this->memory + netPacketSize, &MetaDataPacket(type, sizeInBytes), sizeof(MetaDataPacket));

	netPacketSize += sizeof(MetaDataPacket);
}
