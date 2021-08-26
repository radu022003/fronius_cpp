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

std::istringstream FroniusHybridSys_GetPowerFlowRealtimeData2(R"!(
{
   "Body" : {
      "Data" : {
         "Inverters" : {
            "1" : {
               "DT" : 81,
               "E_Day" : 15039,
               "E_Total" : 2777190,
               "E_Year" : 2777198.75,
               "P" : 1033
            }
         },
         "Site" : {
            "E_Day" : 15039,
            "E_Total" : 2777190,
            "E_Year" : 2777198.75,
            "Meter_Location" : "grid",
            "Mode" : "meter",
            "P_Akku" : null,
            "P_Grid" : -975.35000000000002,
            "P_Load" : -57.649999999999977,
            "P_PV" : 1033,
            "rel_Autonomy" : 100,
            "rel_SelfConsumption" : 5.5808325266214887
         },
         "Version" : "12"
      }
   },
   "Head" : {
      "RequestArguments" : {},
      "Status" : {
         "Code" : 0,
         "Reason" : "",
         "UserMessage" : ""
      },
      "Timestamp" : "2021-08-25T17:13:09+03:00"
   }
}
)!");

std::istringstream FroniusHybridSys_InverterRealtimeData(R"!(
{
   "Body" : {
      "Data" : {
         "DAY_ENERGY" : {
            "Unit" : "Wh",
            "Values" : {
               "1" : 15173
            }
         },
         "PAC" : {
            "Unit" : "W",
            "Values" : {
               "1" : 955
            }
         },
         "TOTAL_ENERGY" : {
            "Unit" : "Wh",
            "Values" : {
               "1" : 2777329
            }
         },
         "YEAR_ENERGY" : {
            "Unit" : "Wh",
            "Values" : {
               "1" : 2777333
            }
         }
      }
   },
   "Head" : {
      "RequestArguments" : {
         "DeviceClass" : "Inverter",
         "Scope" : "System"
      },
      "Status" : {
         "Code" : 0,
         "Reason" : "",
         "UserMessage" : ""
      },
      "Timestamp" : "2021-08-25T17:21:22+03:00"
   }
}
)!");

std::istringstream FroniusHybridSys_MeterRealtimeData(R"!(
{
   "Body" : {
      "Data" : {
         "Current_AC_Phase_1" : 3.9550000000000001,
         "Current_AC_Sum" : 3.9550000000000001,
         "Details" : {
            "Manufacturer" : "Fronius",
            "Model" : "Smart Meter 63A-1",
            "Serial" : "19280280"
         },
         "Enable" : 1,
         "EnergyReactive_VArAC_Phase_1_Consumed" : 550240,
         "EnergyReactive_VArAC_Phase_1_Produced" : 936080,
         "EnergyReactive_VArAC_Sum_Consumed" : 550240,
         "EnergyReactive_VArAC_Sum_Produced" : 936080,
         "EnergyReal_WAC_Minus_Absolute" : 2584992,
         "EnergyReal_WAC_Phase_1_Consumed" : 206254,
         "EnergyReal_WAC_Phase_1_Produced" : 2584992,
         "EnergyReal_WAC_Plus_Absolute" : 206254,
         "EnergyReal_WAC_Sum_Consumed" : 206254,
         "EnergyReal_WAC_Sum_Produced" : 2584992,
         "Frequency_Phase_Average" : 50,
         "Meter_Location_Current" : 0,
         "PowerApparent_S_Phase_1" : 897.77999999999997,
         "PowerApparent_S_Sum" : 897.77999999999997,
         "PowerFactor_Phase_1" : 0.94999999999999996,
         "PowerFactor_Sum" : 0.94999999999999996,
         "PowerReactive_Q_Phase_1" : 197.62,
         "PowerReactive_Q_Sum" : 197.62,
         "PowerReal_P_Phase_1" : -860.48000000000002,
         "PowerReal_P_Sum" : -860.48000000000002,
         "TimeStamp" : 1629900467,
         "Visible" : 1,
         "Voltage_AC_Phase_1" : 227
      }
   },
   "Head" : {
      "RequestArguments" : {
         "DeviceClass" : "Meter",
         "DeviceId" : "0",
         "Scope" : "Device"
      },
      "Status" : {
         "Code" : 0,
         "Reason" : "",
         "UserMessage" : ""
      },
      "Timestamp" : "2021-08-25T17:07:48+03:00"
   }
}
)!");

std::istringstream FroniusHybridSys_InverterInfo(R"!(
{
   "Body" : {
      "Data" : {
         "1" : {
            "CustomName" : "&#80;&#114;&#105;&#109;&#111;&#32;&#51;&#46;&#48;&#45;&#49;&#32;&#40;&#49;&#41;",
            "DT" : 81,
            "ErrorCode" : 0,
            "PVPower" : 3420,
            "Show" : 1,
            "StatusCode" : 7,
            "UniqueID" : "1148686"
         }
      }
   },
   "Head" : {
      "RequestArguments" : {},
      "Status" : {
         "Code" : 0,
         "Reason" : "",
         "UserMessage" : ""
      },
      "Timestamp" : "2021-08-25T17:15:51+03:00"
   }
}
)!");