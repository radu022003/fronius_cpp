// Fronius.h
#pragma once
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <iostream>
#include <string>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

class FroniusClient
{
private:
    /* data */
    FroniusClient(/* args */);

public:
    static FroniusClient *create();
    ~FroniusClient();
};
