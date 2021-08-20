#include <fronius.h>

int main(int argc, char **argv)
{

	//auto inverter = FroniusClient::create();
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <uri>" << endl;
		cout << "       fetches the resource identified by <uri> and print it" << endl;
		return -1;
	}

	try
	{
		// prepare session
		URI uri(argv[1]);
		HTTPClientSession session(uri.getHost(), uri.getPort());

		// prepare path
		string path(uri.getPathAndQuery());
		if (path.empty())
			path = "/";

		// send request
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		session.sendRequest(req);

		// get response
		HTTPResponse res;
		cout << res.getStatus() << " " << res.getReason() << endl;

		// print response
		istream &is = session.receiveResponse(res);
		StreamCopier::copyStream(is, cout);
	}
	catch (Exception &ex)
	{
		cerr << "exception occurred: " << ex.displayText() << endl;
		return -1;
	}

	return 0;
}