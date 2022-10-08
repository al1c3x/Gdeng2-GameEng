#include "EngineTime.h"

EngineTime* EngineTime::sharedInstance = NULL;


void EngineTime::initialize()
{
	sharedInstance = new EngineTime();
}

double EngineTime::getDeltaTime()
{
	return sharedInstance->deltaTime; 
}

EngineTime::EngineTime()
{

}

EngineTime::~EngineTime()
{
}

void EngineTime::LogFrameStart()
{
	sharedInstance->start = std::chrono::system_clock::now();
}

void EngineTime::LogFrameEnd()
{
	sharedInstance->end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_second = sharedInstance->end - sharedInstance->start;

	sharedInstance->deltaTime = elapsed_second.count();


	//std::cout << "Frame update Finish in:" << sharedInstance->deltaTime << "\n";
}
