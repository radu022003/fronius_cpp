#pragma once
#include <sstream>

std::istringstream GetRealTimeFlowDataFull_OK(R"!(
{
    "Body": {
        "Data": {
            "Inverters": {
                "1": {
                    "DT": 102,
                    "E_Day": 1393.2000732421875,
                    "E_Total": 1734796.125,
                    "E_Year": 322593.5,
                    "P": 88
                },
                "2": {
                    "DT": 86,
                    "E_Day": 1618.5,
                    "E_Total": 3026782,
                    "E_Year": 385172.09375,
                    "P": 104
                    65
                },
                "3": {
                    "DT": 106,
                    "E_Day": 1695.800048828125,
                    "E_Total": 3160499.75,
                    "E_Year": 399904.09375,
                    "P": 109
                },
                "55": {
                    "DT": 224,
                    "E_Day": 1699,
                    "E_Total": 3275219.75,
                    "E_Year": 403993.21875,
                    "P": 109
                }
            },
            "Site": {
                "E_Day": 6406.5001220703125,
                "E_Total": 11197297.625,
                "E_Year": 1511662.90625,
                "Meter_Location": "unknown",
                "Mode": "produce -only",
                "P_Akku": null,
                "P_Grid": null,
                "P_Load": null,
                "P_PV": 410,
                "rel_Autonomy": null,
                "rel_SelfConsumption": null
            },
            "Version": "12"
        }
    },
    "Head": {
        "RequestArguments": {},
        "Status": {
            "Code": 0,
            "Reason": "",
            "UserMessage": ""
        },
        "Timestamp": "2019 -06 -12 T15 :31:08+02:00"
    }
})!");

std::istringstream FroniusHybridSys_GetPowerFlowRealtimeData(R"!(
{
    "Body": {
        "Data": {
            "Inverters": {
                "1": {
                    "Battery_Mode": "normal",
                    "DT": 99,
                    "E_Day": 6758,
                    "E_Total": 7604385.5,
                    "E_Year": 1342638.25,
                    "P": 506,
                    "SOC": 55
                }
            },
            "Site": {
                "BatteryStandby": false,
                "E_Day": 6758,
                "E_Total": 7604385.5,
                "E_Year": 1342638.2000000002,
                "Meter_Location": "grid",
                "Mode": "bidirectional",
                "P_Akku": -384.70000000000005,
                "P_Grid": -511.99000000000001,
                "P_Load": 5.9900000000000091,
                "P_PV": 941.60000000000002,
                "rel_Autonomy": 100,
                "rel_SelfConsumption": 0
            },
            "Smartloads": {
                "Ohmpilots": {
                    "720897": {
                        "P_AC_Total": 2635,
                        "State": "normal",
                        "Temperature": 30.7
                    }
                }
            },
            "Version": "12"
        }
    },
    "Head": {
        "RequestArguments": {},
        "Status": {
            "Code": 0,
            "Reason": "",
            "UserMessage": ""
        },
        "Timestamp": "2019 -06 -12 T15 :31:07+02:00"
    }
}
)!");
