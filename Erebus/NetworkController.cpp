#include "NetworkController.hpp"

NetworkController::NetworkController()
{
	initalized = false;
	networkHost = true;
	running = false;
	transformpackTime = 0.0;
	animationpackTime = 0.0;
	aiTransformpackTime = 0.0;
}

NetworkController::~NetworkController()
{
	NetworkController::shutdown();
}

bool NetworkController::initNetworkAsHost()
{
	if (!initalized)
	{
		if (!network.InitializeHost())
		{
			std::cout << "failed to initialize sockets" << std::endl;
			return false;
		}
		initalized = true;
	}

	return true;
}

bool NetworkController::initNetworkAsClient(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4)
{
	if (!initalized)
	{
		if (!network.InitializeClient(ip1, ip2, ip3, ip4))
		{
			std::cout << "failed to initialize sockets" << std::endl;
			return false;
		}
		initalized = true;
	}

	return true;
}

void NetworkController::shutdown()
{
	if (running)
	{
		running = false;
		sendingThread.join();
		receiveThread.join();
	}
	
	if (initalized)
	{
		initalized = false;
		network.Shutdown();
	}
}

void NetworkController::startNetworkSending()
{
	while (running)
	{
		double deltaTime = counter.getNetworkSendDeltaTime();
		if (deltaTime > sendFrequency)
		{
			network.Send();
		}
		else
		{
			long long sleepTime = (long long)(sendFrequency - deltaTime) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			network.Send();
		}
	}
}

void NetworkController::startNetworkReceiving()
{
	while (running)
	{
		double deltaTime = counter.getNetworkRecDeltaTime();
		if (deltaTime > recFrequency)
		{
			network.Receive();
		}
		else
		{
			long long sleepTime = (long long)(recFrequency - deltaTime) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			network.Receive();
		}
	}
}

bool NetworkController::acceptNetworkCommunication()
{

	if (network.AcceptCommunication())
	{
		return true;
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

	return false;
}

void NetworkController::startCommunicationThreads(PerformanceCounter& counter)
{
	if (initalized)
	{
		running = true;

		this->counter = counter;

		sendingThread = std::thread(&NetworkController::startNetworkSending, this);

		receiveThread = std::thread(&NetworkController::startNetworkReceiving, this);
	}
	else
	{
		std::cout << "Network has not been initalized yet" << std::endl;
	}
}

void NetworkController::setNetworkHost(const bool& networkHost)
{
	this->networkHost = networkHost;
}

bool NetworkController::getNetworkHost()
{
	return this->networkHost;
}

void NetworkController::sendTransformPacket(const TransformPacket& packet)
{
	transformpackTime = counter.getCurrentTime();
	network.pushTransformPacket(packet);
}

bool NetworkController::fetchTransformPacket(TransformPacket &packet)
{
	TransformPacket tempPacket;
	bool isQueueEmpty = network.fetchTransformPacket(tempPacket);
	bool result = isQueueEmpty;
	while (isQueueEmpty)
	{
		packet = tempPacket;
		isQueueEmpty = network.fetchTransformPacket(tempPacket);
	}
	return result;
}

void NetworkController::sendAnimationPacket(const AnimationPacket& packet)
{
	animationpackTime = counter.getCurrentTime();
	network.pushAnimationPacket(packet);
}

bool NetworkController::fetchAnimationPacket(AnimationPacket& packet)
{
	return network.fetchAnimationPacket(packet);
}

void NetworkController::sendAIStatePacket(const AIStatePacket& packet)
{
	network.pushAIStatePacket(packet);
}

bool NetworkController::fetchAIStatePacket(AIStatePacket& packet)
{
	return network.fetchAIPacket(packet);
}

bool NetworkController::fetchSpellPacket(SpellPacket& packet)
{
	return network.fetchSpellPacket(packet);
}

void NetworkController::sendSpellPacket(const SpellPacket& packet)
{
	network.pushSpellPacket(packet);
}

void NetworkController::sendAITransformPacket(const TransformPacket& packet)
{
	aiTransformpackTime = counter.getCurrentTime();
	network.pushAITransformPacket(packet);
}

bool NetworkController::fetchAITransformPacket(TransformPacket &packet)
{
	return network.fetchAITransformPacket(packet);
}

void NetworkController::sendChargingPacket(const ChargingPacket& packet)
{
	network.pushChargingPacket(packet);
}

bool NetworkController::fetchChargingPacket(ChargingPacket &packet)
{
	return network.fetchChargingPacket(packet);
}

void NetworkController::sendQuickBlendPacket(const QuickBlendPacket& packet)
{
	network.pushQuickBlendPacket(packet);
}

bool NetworkController::fetchQuickBlendPacket(QuickBlendPacket& packet)
{
	return network.fetchQuickBlendPacket(packet);
}

void NetworkController::sendDamagePacket(const DamagePacket& packet)
{
	network.pushDamagePacket(packet);
}

bool NetworkController::fetchDamagePacket(DamagePacket& packet)
{
	return network.fetchDamagePacket(packet);
}

void NetworkController::sendChangeSpellsPacket(const ChangeSpellsPacket& packet)
{
	network.pushChangeSpellsPacket(packet);
}

bool NetworkController::fetchChangeSpellsPacket(ChangeSpellsPacket& packet)
{
	return network.fetchChangeSpellsPacket(packet);
}

void NetworkController::sendPlayerEventPacket(const EventPacket& packet)
{
	network.pushPlayerEventPacket(packet);
}

bool NetworkController::fetchPlayerEventPacket(EventPacket& packet)
{
	return network.fetchPlayerEventPacket(packet);
}

void NetworkController::sendAIHealthPacket(const HealthPacket& packet)
{
	network.pushAIHealthPacket(packet);
}

bool NetworkController::fetchAIHealthPacket(HealthPacket& packet)
{
	return network.fetchAIHealthPacket(packet);
}

void NetworkController::sendDashPacket(const DashPacket& packet)
{
	network.pushDashPacket(packet);
}

bool NetworkController::fetchDashPacket(DashPacket& packet)
{
	return network.fetchDashPacket(packet);
}

void NetworkController::sendEndEventPacket(const EventPacket& packet)
{
	network.pushEndEventPacket(packet);
}

bool NetworkController::fetchEndEventPacket(EventPacket& packet)
{
	return network.fetchEndEventPacket(packet);
}

void NetworkController::sendPlayerHealthPacket(const HealthPacket& packet)
{
	network.pushPlayerHealthPacket(packet);
}

bool NetworkController::fetchPlayerHealthPacket(HealthPacket& packet)
{
	return network.fetchPlayerHealthPacket(packet);
}

void NetworkController::sendRessurectionPacket(const HealthPacket& packet)
{
	network.pushRessurectionPacket(packet);
}

bool NetworkController::fetchRessurectionPacket(HealthPacket& packet)
{
	return network.fetchRessurectionPacket(packet);
}

void NetworkController::sendAIDamageTextPacket(const DamagePacket& packet)
{
	network.pushAIDamageTextPacket(packet);
}

bool NetworkController::fetchAIDamageTextPacket(DamagePacket& packet)
{
	return network.fetchAIDamageTextPacket(packet);
}

void NetworkController::sendBossDamageTextPacket(const DamagePacket& packet)
{
	network.pushBossDamageTextPacket(packet);
}

bool NetworkController::fetchBossDamageTextPacket(DamagePacket& packet)
{
	return network.fetchBossDamageTextPacket(packet);
}

void NetworkController::sendBossHealthPacket(const HealthPacket& packet)
{
	network.pushBossHealthPacket(packet);
}

bool NetworkController::fetchBossHealthPacket(HealthPacket& packet)
{
	return network.fetchBossHealthPacket(packet);
}

double NetworkController::timeSinceLastTransformPacket()
{
	return (counter.getCurrentTime() - transformpackTime);
}

double NetworkController::timeSinceLastAnimationPacket()
{
	return (counter.getCurrentTime() - animationpackTime);
}

double NetworkController::timeSinceLastAITransformPacket()
{
	return (counter.getCurrentTime() - aiTransformpackTime);
}

#ifdef DEBUGGING_NETWORK
float NetworkController::getPing()
{
	return network.getPing();
}
#endif