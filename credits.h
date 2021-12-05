#include <pgmspace.h>
#define CREDITS
#define THINGNAME "YOUR_THING_NAME"                                       //your thingname available on AWS

const char WIFI_SSID[] = "SSID";                                        //WIFI credentials
const char WIFI_PASSWORD[] = "PASS";                                    //WIFI credentials
const char AWS_IOT_ENDPOINT[] = "IOT_ENDPOINT";        //available on AWS

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----


-----END CERTIFICATE-----
)EOF";

// Device Certificate                                               //downloaded file from AWS
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----




)KEY";

// Device Private Key                                               //downloaded file from AWS
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----


)KEY";
