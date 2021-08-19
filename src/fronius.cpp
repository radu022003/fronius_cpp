#include <fronius.h>

FroniusClient::FroniusClient(/* args */)
{
}

FroniusClient::~FroniusClient()
{
}

FroniusClient *FroniusClient::create()
{
    return new FroniusClient();
}