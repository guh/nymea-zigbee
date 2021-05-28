/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea-zigbee.
* This project including source code and documentation is protected by copyright law, and
* remains the property of nymea GmbH. All rights, including reproduction, publication,
* editing and translation, are reserved. The use of this project is subject to the terms of a
* license agreement to be concluded with nymea GmbH in accordance with the terms
* of use of nymea GmbH, available under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the terms of the GNU
* Lesser General Public License as published by the Free Software Foundation; version 3.
* this project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with this project.
* If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under contact@nymea.io
* or see our FAQ/Licensing Information on https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ZIGBEE_H
#define ZIGBEE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QByteArray>

// Note: a good source of data https://github.com/wireshark/wireshark/blob/master/epan/dissectors/packet-zbee.h

class Zigbee
{
    Q_GADGET

public:
    enum ZigbeeBackendType {
        ZigbeeBackendTypeDeconz,
        ZigbeeBackendTypeNxp
    };
    Q_ENUM(ZigbeeBackendType)

    enum ZigbeeProfile {
        ZigbeeProfileDevice = 0x0000,
        ZigbeeProfileIndustrialPlantMonitoring = 0x0101,
        ZigbeeProfileHomeAutomation = 0x0104,
        ZigbeeProfileCommercialBuildingAutomation = 0x0105,
        ZigbeeProfileTelecomAutomation = 0x0107,
        ZigbeeProfilePersonalHomeHospitalCare = 0x0108,
        ZigbeeProfileAdvancedMetering = 0x0109,
        ZigbeeProfileLightLink = 0xC05E,
        ZigbeeProfileDevelco = 0xC0C9,
        ZigbeeProfileGreenPower = 0xA1E0
    };
    Q_ENUM(ZigbeeProfile)

    enum ZigbeeChannel {
        ZigbeeChannel11 = 0x00000800,
        ZigbeeChannel12 = 0x00001000,
        ZigbeeChannel13 = 0x00002000,
        ZigbeeChannel14 = 0x00004000,
        ZigbeeChannel15 = 0x00008000,
        ZigbeeChannel16 = 0x00010000,
        ZigbeeChannel17 = 0x00020000,
        ZigbeeChannel18 = 0x00040000,
        ZigbeeChannel19 = 0x00080000,
        ZigbeeChannel20 = 0x00100000,
        ZigbeeChannel21 = 0x00200000,
        ZigbeeChannel22 = 0x00400000,
        ZigbeeChannel23 = 0x00800000,
        ZigbeeChannel24 = 0x01000000,
        ZigbeeChannel25 = 0x02000000,
        ZigbeeChannel26 = 0x04000000
    };
    Q_ENUM(ZigbeeChannel)
    Q_DECLARE_FLAGS(ZigbeeChannels, ZigbeeChannel)

    enum LightLinkDevice {
        // Lightning devices
        LightLinkDeviceOnOffLight                       = 0x0000,
        LightLinkDeviceOnOffPlugin                      = 0x0010,
        LightLinkDeviceDimmableLight                    = 0x0100,
        LightLinkDeviceDimmablePlugin                   = 0x0110,
        LightLinkDeviceColourLight                      = 0x0200,
        LightLinkDeviceExtendedColourLight              = 0x0210,
        LightLinkDeviceColourTemperatureLight           = 0x0220,

        // Controller devices
        LightLinkDeviceColourController                 = 0x0800,
        LightLinkDeviceColourSceneController            = 0x0810,
        LightLinkDeviceNonColourController              = 0x0820,
        LightLinkDeviceNonColourSceneController         = 0x0830,
        LightLinkDeviceControlBridge                    = 0x0840,
        LightLinkDeviceOnOffSensor                      = 0x0850
    };
    Q_ENUM(LightLinkDevice)


    enum HomeAutomationDevice {
        // Generic devices
        HomeAutomationDeviceOnOffSwitch                 = 0x0000,
        HomeAutomationDeviceLevelControlSwitch          = 0x0001,
        HomeAutomationDeviceOnOffOutput                 = 0x0002,
        HomeAutomationDeviceLevelControlableOutput      = 0x0003,
        HomeAutomationDeviceSceneSelector               = 0x0004,
        HomeAutomationDeviceConfigurationTool           = 0x0005,
        HomeAutomationDeviceRemoteControl               = 0x0006,
        HomeAutomationDeviceCombinedInterface           = 0x0007,
        HomeAutomationDeviceRangeExtender               = 0x0008,
        HomeAutomationDeviceMainPowerOutlet             = 0x0009,
        HomeAutomationDeviceDoorLock                    = 0x000A,
        HomeAutomationDeviceDoorLockController          = 0x000B,
        HomeAutomationDeviceSimpleSensor                = 0x000C,
        HomeAutomationDeviceConsumtionAwarenessDevice   = 0x000D,
        HomeAutomationDeviceHomeGateway                 = 0x0050,
        HomeAutomationDeviceSmartPlug                   = 0x0051,
        HomeAutomationDeviceWhiteGoods                  = 0x0052,
        HomeAutomationDeviceMeterInterface              = 0x0053,
        HomeAutomationDeviceColourController            = 0x0800,
        HomeAutomationDeviceColourSceneController       = 0x0810,
        HomeAutomationDeviceNonColourController         = 0x0820,
        HomeAutomationDeviceNonColourSceneController    = 0x0830,
        HomeAutomationDeviceControlBridge               = 0x0840,
        HomeAutomationDeviceOnOffSensor                 = 0x0850,

        // Lightning devices
        HomeAutomationDeviceOnOffLight                  = 0x0100,
        HomeAutomationDeviceDimmableLight               = 0x0101,
        HomeAutomationDeviceDimmableColorLight          = 0x0102,
        HomeAutomationDeviceOnOffLightSwitch            = 0x0103,
        HomeAutomationDeviceDimmableSwitch              = 0x0104,
        HomeAutomationDeviceColourDimmerSwitch          = 0x0105,
        HomeAutomationDeviceLightSensor                 = 0x0106,
        HomeAutomationDeviceOccupacySensor              = 0x0107,
        HomeAutomationDeviceOnOffBallast                = 0x0108,
        HomeAutomationDeviceDimmableBallast             = 0x0109,
        HomeAutomationDeviceOnOffPlugin                 = 0x010A,
        HomeAutomationDeviceDimmablePlugin              = 0x010B,
        HomeAutomationDeviceColourTemperatureLight      = 0x010C,
        HomeAutomationDeviceExtendedColourLight         = 0x010D,
        HomeAutomationDeviceLightLevelSensor            = 0x010E,

        // Closures
        HomeAutomationDeviceShade                       = 0x02000,
        HomeAutomationDeviceShadeController             = 0x02001,
        HomeAutomationWindowCoveringDevice              = 0x02002,
        HomeAutomationWindowCoveringController          = 0x02003,

        // Heating, Ventilation and Air-Conditioning (HVAC) devices
        HomeAutomationDeviceHeatingCoolingUnit          = 0x0300,
        HomeAutomationDeviceThermostat                  = 0x0301,
        HomeAutomationDeviceTemperatureSensor           = 0x0302,
        HomeAutomationDevicePump                        = 0x0303,
        HomeAutomationDevicePumpController              = 0x0304,
        HomeAutomationDevicePressureSensor              = 0x0305,
        HomeAutomationDeviceFlowSensor                  = 0x0306,

        // Intruder Alarm System (IAS) devices
        HomeAutomationDeviceIsaControlEquipment             = 0x0400, // CIE
        HomeAutomationDeviceIsaAncillaryControlEquipment    = 0x0401, // ACE
        HomeAutomationDeviceIsaZone                         = 0x0402,
        HomeAutomationDeviceIsaWarningDevice                = 0x0403  // WD
    };
    Q_ENUM(HomeAutomationDevice)


    enum GreenPowerDevice {
        GreenPowerDeviceProxy = 0x0060,
        GreenPowerDeviceProxyMinimum = 0x0061,
        GreenPowerDeviceProxyTargetPlus = 0x0062,
        GreenPowerDeviceProxyTarget = 0x0063,
        GreenPowerDeviceProxyCommissioningTool = 0x0064,
        GreenPowerDeviceProxyCombo = 0x0065,
        GreenPowerDeviceProxyComboMinimum = 0x0066
    };
    Q_ENUM(GreenPowerDevice)

    enum DataType {
        NoData          = 0x00,
        Data8           = 0x08,
        Data16          = 0x09,
        Data24          = 0x0a,
        Data32          = 0x0b,
        Data40          = 0x0c,
        Data48          = 0x0d,
        Data56          = 0x0e,
        Data64          = 0x0f,
        Bool            = 0x10,
        BitMap8         = 0x18,
        BitMap16        = 0x19,
        BitMap24        = 0x1a,
        BitMap32        = 0x1b,
        BitMap40        = 0x1c,
        BitMap48        = 0x1d,
        BitMap56        = 0x1e,
        BitMap64        = 0x1f,
        Uint8           = 0x20,
        Uint16          = 0x21,
        Uint24          = 0x22,
        Uint32          = 0x23,
        Uint40          = 0x24,
        Uint48          = 0x25,
        Uint56          = 0x26,
        Uint64          = 0x27,
        Int8            = 0x28,
        Int16           = 0x29,
        Int24           = 0x2a,
        Int32           = 0x2b,
        Int40           = 0x2c,
        Int48           = 0x2d,
        Int56           = 0x2e,
        Int64           = 0x2f,
        Enum8           = 0x30,
        Enum16          = 0x31,
        FloatSemi       = 0x38,
        FloatSingle     = 0x39,
        FloatDouble     = 0x3a,
        OctetString     = 0x41,
        CharString      = 0x42,
        LongOctetString = 0x43,
        LongCharString  = 0x44,
        Array           = 0x48,
        Structure       = 0x4c,
        Set             = 0x50,
        Bag             = 0x51,
        TimeOfDay       = 0xe0,
        Date            = 0xe1,
        UtcTime         = 0xe2,
        Cluster         = 0xe8,
        Attribute       = 0xe9,
        BacnetId        = 0xea,
        IeeeAddress     = 0xf0,
        BitKey128       = 0xf1,
        Unknown         = 0xff
    };
    Q_ENUM(DataType)

    enum BroadcastAddress {
        BroadcastAddressAllNodes = 0xffff,
        BroadcastAddressAllNonSleepingNodes = 0xfffd,
        BroadcastAddressAllRouters = 0xfffc
    };
    Q_ENUM(BroadcastAddress)

    enum DestinationAddressMode {
        DestinationAddressModeGroup = 0x01,
        DestinationAddressModeShortAddress = 0x02,
        DestinationAddressModeIeeeAddress = 0x03
    };
    Q_ENUM(DestinationAddressMode)

    enum SourceAddressMode {
        SourceAddressModeShortAddress = 0x02,
        SourceAddressModeIeeeAddress = 0x03,
        SourceAddressModeShortAndIeeeAddress = 0x04
    };
    Q_ENUM(SourceAddressMode)

    enum ZigbeeTxOption {
        ZigbeeTxOptionSecurityEnabled = 0x01,
        ZigbeeTxOptionUseNetworkKey = 0x02,
        ZigbeeTxOptionAckTransmission = 0x04,
        ZigbeeTxOptionFragmentationPermitted = 0x08,
        ZigbeeTxOptionIncludeExtendedNonceInSecurityFrame = 0x10
    };
    Q_ENUM(ZigbeeTxOption)
    Q_DECLARE_FLAGS(ZigbeeTxOptions, ZigbeeTxOption)

    enum Manufacturer {
        // RF4CE
        PanasonicRF4CE          = 0x0001,
        SonyRF4CE               = 0x0002,
        SamsungRF4CE            = 0x0003,
        PhilipsRF4CE            = 0x0004,
        FreescaleRF4CE          = 0x0005,
        OkiSemiRF4CE            = 0x0006,
        TiRF4CE                 = 0x0007,

        //  Manufacturer Codes for non RF4CE devices
        Cirronet                = 0x1000,
        Chipcon                 = 0x1001,
        Ember                   = 0x1003,
        Philips                 = 0x100b,
        Develco                 = 0x1015,
        Ikea                    = 0x117C,
        FeiBit                  = 0x117E
    };
    Q_ENUM(Manufacturer)

    enum ZigbeeMacLayerStatus {
        ZigbeeMacLayerStatusSuccess = 0x00,
        ZigbeeMacLayerStatusBeaconLoss = 0xE0,
        ZigbeeMacLayerStatusChannelAccessFailure = 0xE1,
        ZigbeeMacLayerStatusDenied = 0xE2,
        ZigbeeMacLayerStatusDisableTrxFailure = 0xE3,
        ZigbeeMacLayerStatusFailedSecurityCheck = 0xE4,
        ZigbeeMacLayerStatusFrameToLong = 0xE5,
        ZigbeeMacLayerStatusInvalidGts = 0xE6,
        ZigbeeMacLayerStatusInvalidHandle = 0xE7,
        ZigbeeMacLayerStatusInvalidParameter = 0xE8,
        ZigbeeMacLayerStatusNoAck = 0xE9,
        ZigbeeMacLayerStatusNoBeacon = 0xEA,
        ZigbeeMacLayerStatusNoData = 0xEB,
        ZigbeeMacLayerStatusNoShortAddress = 0xEC,
        ZigbeeMacLayerStatusOutOfCap = 0xED,
        ZigbeeMacLayerStatusPanIdConflict = 0xEE,
        ZigbeeMacLayerStatusRealignment = 0xEF,
        ZigbeeMacLayerStatusTransactionExpired = 0xF0,
        ZigbeeMacLayerStatusTransactionOverflow = 0xF1,
        ZigbeeMacLayerStatusTxActive = 0xF2,
        ZigbeeMacLayerStatusUnavailableKey = 0xF3,
        ZigbeeMacLayerStatusUnsupportedAttribute = 0xF4
    };
    Q_ENUM(ZigbeeMacLayerStatus)

    enum ZigbeeNwkLayerStatus {
        ZigbeeNwkLayerStatusSuccess = 0x00,
        ZigbeeNwkLayerStatusInvalidParameter = 0xc1,
        ZigbeeNwkLayerStatusInvalidRequest = 0xc2,
        ZigbeeNwkLayerStatusNotPermitted = 0xc3,
        ZigbeeNwkLayerStatusStartupFailure = 0xc4,
        ZigbeeNwkLayerStatusAlreadyPresent = 0xc5,
        ZigbeeNwkLayerStatusSynchFailure = 0xc6,
        ZigbeeNwkLayerStatusTableFull = 0xc7,
        ZigbeeNwkLayerStatusUnknownDevice = 0xc8,
        ZigbeeNwkLayerStatusUnsupportedAttribute = 0xc9,
        ZigbeeNwkLayerStatusNoNetworks = 0xca,
        ZigbeeNwkLayerStatusMaxFrmCntr = 0xcc,
        ZigbeeNwkLayerStatusNoKey = 0xcd,
        ZigbeeNwkLayerStatusBadCcmOutput = 0xce,
        ZigbeeNwkLayerStatusNoRoutingCapacity = 0xef, // Resource problem on the hardware
        ZigbeeNwkLayerStatusRouteDiscoveryFailed = 0xd0,
        ZigbeeNwkLayerStatusRouteError = 0xd1,
        ZigbeeNwkLayerStatusBtTableFull = 0xd2,
        ZigbeeNwkLayerStatusFrameNotBuffered = 0xd3,
        ZigbeeNwkLayerStatusFrameBuffered = 0xd4 // Buffered until route discovery has been done
    };
    Q_ENUM(ZigbeeNwkLayerStatus)

    enum ZigbeeApsStatus {
        ZigbeeApsStatusSuccess = 0x00,
        ZigbeeApsStatusAsduTooLong = 0xa0,
        ZigbeeApsStatusDefragDeferred = 0xa1,
        ZigbeeApsStatusDefragUnsupported = 0xa2,
        ZigbeeApsStatusIllegalRequest = 0xa3,
        ZigbeeApsStatusInvalidBinding = 0xa4,
        ZigbeeApsStatusInvalidGroup = 0xa5,
        ZigbeeApsStatusInvalidParameter = 0xa6,
        ZigbeeApsStatusNoAck = 0xa7,
        ZigbeeApsStatusNoBoundDevice = 0xa8,
        ZigbeeApsStatusNoShortAddress = 0xa9,
        ZigbeeApsStatusNotSupported = 0xaa,
        ZigbeeApsStatusSecuredLinkKey = 0xab,
        ZigbeeApsStatusSecuredNwkKey = 0xac,
        ZigbeeApsStatusSecurityFail = 0xad,
        ZigbeeApsStatusTableFull = 0xae,
        ZigbeeApsStatusUnsecured = 0xaf,
        ZigbeeApsStatusUnsupportedAttribute = 0xb0
    };
    Q_ENUM(ZigbeeApsStatus)

    // Basic struct for interface data.
    typedef  struct ApsdeDataConfirm {
        quint8 requestId = 0;
        quint8 destinationAddressMode = Zigbee::DestinationAddressModeShortAddress;
        quint16 destinationShortAddress = 0;
        quint64 destinationIeeeAddress = 0;
        quint8 destinationEndpoint = 0;
        quint8 sourceEndpoint = 0;
        quint8 zigbeeStatusCode = 0;
    } ApsdeDataConfirm;

    typedef  struct ApsdeDataIndication {
        quint8 destinationAddressMode = 0;
        quint16 destinationShortAddress = 0;
        quint64 destinationIeeeAddress = 0;
        quint8 destinationEndpoint = 0;
        quint8 sourceAddressMode = 0;
        quint16 sourceShortAddress = 0;
        quint64 sourceIeeeAddress = 0;
        quint8 sourceEndpoint = 0;
        quint16 profileId = 0;
        quint16 clusterId = 0;
        QByteArray asdu;
        quint8 lqi = 0;
        qint8 rssi = 0;
    } ApsdeDataIndication;

    typedef  struct ApsdeDataAck {
        quint8 requestId = 0;
        quint8 destinationAddressMode = Zigbee::DestinationAddressModeShortAddress;
        quint16 destinationAddress = 0;
        quint8 destinationEndpoint = 0;
        quint8 sourceEndpoint = 0;
        quint16 profileId = 0;
        quint16 clusterId = 0;
        quint8 zigbeeStatusCode = 0;
    } ApsdeDataAck;

    ///* Manufacturer Codes */
    ///* Codes less than 0x1000 were issued for RF4CE */
    //#define ZBEE_MFG_CODE_PANASONIC_RF4CE       0x0001
    //#define ZBEE_MFG_CODE_SONY_RF4CE            0x0002
    //#define ZBEE_MFG_CODE_SAMSUNG_RF4CE         0x0003
    //#define ZBEE_MFG_CODE_PHILIPS_RF4CE         0x0004
    //#define ZBEE_MFG_CODE_FREESCALE_RF4CE       0x0005
    //#define ZBEE_MFG_CODE_OKI_SEMI_RF4CE        0x0006
    //#define ZBEE_MFG_CODE_TI_RF4CE              0x0007

    ///* Manufacturer Codes for non RF4CE devices */
    //#define ZBEE_MFG_CODE_CIRRONET              0x1000
    //#define ZBEE_MFG_CODE_CHIPCON
    //#define ZBEE_MFG_CODE_EMBER                 0x1002
    //#define ZBEE_MFG_CODE_NTS                   0x1003
    //#define ZBEE_MFG_CODE_FREESCALE             0x1004
    //#define ZBEE_MFG_CODE_IPCOM                 0x1005
    //#define ZBEE_MFG_CODE_SAN_JUAN              0x1006
    //#define ZBEE_MFG_CODE_TUV                   0x1007
    //#define ZBEE_MFG_CODE_COMPXS                0x1008
    //#define ZBEE_MFG_CODE_BM                    0x1009
    //#define ZBEE_MFG_CODE_AWAREPOINT            0x100a
    //#define ZBEE_MFG_CODE_PHILIPS               0x100b
    //#define ZBEE_MFG_CODE_LUXOFT                0x100c
    //#define ZBEE_MFG_CODE_KORWIN                0x100d
    //#define ZBEE_MFG_CODE_1_RF                  0x100e
    //#define ZBEE_MFG_CODE_STG                   0x100f

    //#define ZBEE_MFG_CODE_TELEGESIS             0x1010
    //#define ZBEE_MFG_CODE_VISIONIC              0x1011
    //#define ZBEE_MFG_CODE_INSTA                 0x1012
    //#define ZBEE_MFG_CODE_ATALUM                0x1013
    //#define ZBEE_MFG_CODE_ATMEL                 0x1014
    //#define ZBEE_MFG_CODE_DEVELCO               0x1015
    //#define ZBEE_MFG_CODE_HONEYWELL1            0x1016
    //#define ZBEE_MFG_CODE_RADIO_PULSE           0x1017
    //#define ZBEE_MFG_CODE_RENESAS               0x1018
    //#define ZBEE_MFG_CODE_XANADU                0x1019
    //#define ZBEE_MFG_CODE_NEC                   0x101a
    //#define ZBEE_MFG_CODE_YAMATAKE              0x101b
    //#define ZBEE_MFG_CODE_TENDRIL               0x101c
    //#define ZBEE_MFG_CODE_ASSA                  0x101d
    //#define ZBEE_MFG_CODE_MAXSTREAM             0x101e
    //#define ZBEE_MFG_CODE_NEUROCOM              0x101f

    //#define ZBEE_MFG_CODE_III                   0x1020
    //#define ZBEE_MFG_CODE_VANTAGE               0x1021
    //#define ZBEE_MFG_CODE_ICONTROL              0x1022
    //#define ZBEE_MFG_CODE_RAYMARINE             0x1023
    //#define ZBEE_MFG_CODE_LSR                   0x1024
    //#define ZBEE_MFG_CODE_ONITY                 0x1025
    //#define ZBEE_MFG_CODE_MONO                  0x1026
    //#define ZBEE_MFG_CODE_RFT                   0x1027
    //#define ZBEE_MFG_CODE_ITRON                 0x1028
    //#define ZBEE_MFG_CODE_TRITECH               0x1029
    //#define ZBEE_MFG_CODE_EMBEDIT               0x102a
    //#define ZBEE_MFG_CODE_S3C                   0x102b
    //#define ZBEE_MFG_CODE_SIEMENS               0x102c
    //#define ZBEE_MFG_CODE_MINDTECH              0x102d
    //#define ZBEE_MFG_CODE_LGE                   0x102e
    //#define ZBEE_MFG_CODE_MITSUBISHI            0x102f

    //#define ZBEE_MFG_CODE_JOHNSON               0x1030
    //#define ZBEE_MFG_CODE_PRI                   0x1031
    //#define ZBEE_MFG_CODE_KNICK                 0x1032
    //#define ZBEE_MFG_CODE_VICONICS              0x1033
    //#define ZBEE_MFG_CODE_FLEXIPANEL            0x1034
    //#define ZBEE_MFG_CODE_PIASIM                0x1035
    //#define ZBEE_MFG_CODE_TRANE                 0x1036
    //#define ZBEE_MFG_CODE_JENNIC                0x1037
    //#define ZBEE_MFG_CODE_LIG                   0x1038
    //#define ZBEE_MFG_CODE_ALERTME               0x1039
    //#define ZBEE_MFG_CODE_DAINTREE              0x103a
    //#define ZBEE_MFG_CODE_AIJI                  0x103b
    //#define ZBEE_MFG_CODE_TEL_ITALIA            0x103c
    //#define ZBEE_MFG_CODE_MIKROKRETS            0x103d
    //#define ZBEE_MFG_CODE_OKI_SEMI              0x103e
    //#define ZBEE_MFG_CODE_NEWPORT               0x103f

    //#define ZBEE_MFG_CODE_C4                    0x1040
    //#define ZBEE_MFG_CODE_STM                   0x1041
    //#define ZBEE_MFG_CODE_ASN                   0x1042
    //#define ZBEE_MFG_CODE_DCSI                  0x1043
    //#define ZBEE_MFG_CODE_FRANCE_TEL            0x1044
    //#define ZBEE_MFG_CODE_MUNET                 0x1045
    //#define ZBEE_MFG_CODE_AUTANI                0x1046
    //#define ZBEE_MFG_CODE_COL_VNET              0x1047
    //#define ZBEE_MFG_CODE_AEROCOMM              0x1048
    //#define ZBEE_MFG_CODE_SI_LABS               0x1049
    //#define ZBEE_MFG_CODE_INNCOM                0x104a
    //#define ZBEE_MFG_CODE_CANNON                0x104b
    //#define ZBEE_MFG_CODE_SYNAPSE               0x104c
    //#define ZBEE_MFG_CODE_FPS                   0x104d
    //#define ZBEE_MFG_CODE_CLS                   0x104e
    //#define ZBEE_MFG_CODE_CRANE                 0x104F

    //#define ZBEE_MFG_CODE_MOBILARM              0x1050
    //#define ZBEE_MFG_CODE_IMONITOR              0x1051
    //#define ZBEE_MFG_CODE_BARTECH               0x1052
    //#define ZBEE_MFG_CODE_MESHNETICS            0x1053
    //#define ZBEE_MFG_CODE_LS_IND                0x1054
    //#define ZBEE_MFG_CODE_CASON                 0x1055
    //#define ZBEE_MFG_CODE_WLESS_GLUE            0x1056
    //#define ZBEE_MFG_CODE_ELSTER                0x1057
    //#define ZBEE_MFG_CODE_SMS_TEC               0x1058
    //#define ZBEE_MFG_CODE_ONSET                 0x1059
    //#define ZBEE_MFG_CODE_RIGA                  0x105a
    //#define ZBEE_MFG_CODE_ENERGATE              0x105b
    //#define ZBEE_MFG_CODE_CONMED                0x105c
    //#define ZBEE_MFG_CODE_POWERMAND             0x105d
    //#define ZBEE_MFG_CODE_SCHNEIDER             0x105e
    //#define ZBEE_MFG_CODE_EATON                 0x105f

    //#define ZBEE_MFG_CODE_TELULAR               0x1060
    //#define ZBEE_MFG_CODE_DELPHI                0x1061
    //#define ZBEE_MFG_CODE_EPISENSOR             0x1062
    //#define ZBEE_MFG_CODE_LANDIS_GYR            0x1063
    //#define ZBEE_MFG_CODE_KABA                  0x1064
    //#define ZBEE_MFG_CODE_SHURE                 0x1065
    //#define ZBEE_MFG_CODE_COMVERGE              0x1066
    //#define ZBEE_MFG_CODE_DBS_LODGING           0x1067
    //#define ZBEE_MFG_CODE_ENERGY_AWARE          0x1068
    //#define ZBEE_MFG_CODE_HIDALGO               0x1069
    //#define ZBEE_MFG_CODE_AIR2APP               0x106a
    //#define ZBEE_MFG_CODE_AMX                   0x106b
    //#define ZBEE_MFG_CODE_EDMI                  0x106c
    //#define ZBEE_MFG_CODE_CYAN                  0x106d
    //#define ZBEE_MFG_CODE_SYS_SPA               0x106e
    //#define ZBEE_MFG_CODE_TELIT                 0x106f

    //#define ZBEE_MFG_CODE_KAGA                  0x1070
    //#define ZBEE_MFG_CODE_4_NOKS                0x1071
    //#define ZBEE_MFG_CODE_CERTICOM              0x1072
    //#define ZBEE_MFG_CODE_GRIDPOINT             0x1073
    //#define ZBEE_MFG_CODE_PROFILE_SYS           0x1074
    //#define ZBEE_MFG_CODE_COMPACTA              0x1075
    //#define ZBEE_MFG_CODE_FREESTYLE             0x1076
    //#define ZBEE_MFG_CODE_ALEKTRONA             0x1077
    //#define ZBEE_MFG_CODE_COMPUTIME             0x1078
    //#define ZBEE_MFG_CODE_REMOTE_TECH           0x1079
    //#define ZBEE_MFG_CODE_WAVECOM               0x107a
    //#define ZBEE_MFG_CODE_ENERGY                0x107b
    //#define ZBEE_MFG_CODE_GE                    0x107c
    //#define ZBEE_MFG_CODE_JETLUN                0x107d
    //#define ZBEE_MFG_CODE_CIPHER                0x107e
    //#define ZBEE_MFG_CODE_CORPORATE             0x107f

    //#define ZBEE_MFG_CODE_ECOBEE                0x1080
    //#define ZBEE_MFG_CODE_SMK                   0x1081
    //#define ZBEE_MFG_CODE_MESHWORKS             0x1082
    //#define ZBEE_MFG_CODE_ELLIPS                0x1083
    //#define ZBEE_MFG_CODE_SECURE                0x1084
    //#define ZBEE_MFG_CODE_CEDO                  0x1085
    //#define ZBEE_MFG_CODE_TOSHIBA               0x1086
    //#define ZBEE_MFG_CODE_DIGI                  0x1087
    //#define ZBEE_MFG_CODE_UBILOGIX              0x1088
    //#define ZBEE_MFG_CODE_ECHELON               0x1089
    ///* */

    //#define ZBEE_MFG_CODE_GREEN_ENERGY          0x1090
    //#define ZBEE_MFG_CODE_SILVER_SPRING         0x1091
    //#define ZBEE_MFG_CODE_BLACK                 0x1092
    //#define ZBEE_MFG_CODE_AZTECH_ASSOC          0x1093
    //#define ZBEE_MFG_CODE_A_AND_D               0x1094
    //#define ZBEE_MFG_CODE_RAINFOREST            0x1095
    //#define ZBEE_MFG_CODE_CARRIER               0x1096
    //#define ZBEE_MFG_CODE_SYCHIP                0x1097
    //#define ZBEE_MFG_CODE_OPEN_PEAK             0x1098
    //#define ZBEE_MFG_CODE_PASSIVE               0x1099
    //#define ZBEE_MFG_CODE_MMB                   0x109a
    //#define ZBEE_MFG_CODE_LEVITON               0x109b
    //#define ZBEE_MFG_CODE_KOREA_ELEC            0x109c
    //#define ZBEE_MFG_CODE_COMCAST1              0x109d
    //#define ZBEE_MFG_CODE_NEC_ELEC              0x109e
    //#define ZBEE_MFG_CODE_NETVOX                0x109f

    //#define ZBEE_MFG_CODE_UCONTROL              0x10a0
    //#define ZBEE_MFG_CODE_EMBEDIA               0x10a1
    //#define ZBEE_MFG_CODE_SENSUS                0x10a2
    //#define ZBEE_MFG_CODE_SUNRISE               0x10a3
    //#define ZBEE_MFG_CODE_MEMTECH               0x10a4
    //#define ZBEE_MFG_CODE_FREEBOX               0x10a5
    //#define ZBEE_MFG_CODE_M2_LABS               0x10a6
    //#define ZBEE_MFG_CODE_BRITISH_GAS           0x10a7
    //#define ZBEE_MFG_CODE_SENTEC                0x10a8
    //#define ZBEE_MFG_CODE_NAVETAS               0x10a9
    //#define ZBEE_MFG_CODE_LIGHTSPEED            0x10aa
    //#define ZBEE_MFG_CODE_OKI                   0x10ab
    //#define ZBEE_MFG_CODE_SISTEMAS              0x10ac
    //#define ZBEE_MFG_CODE_DOMETIC               0x10ad
    //#define ZBEE_MFG_CODE_APLS                  0x10ae
    //#define ZBEE_MFG_CODE_ENERGY_HUB            0x10af

    //#define ZBEE_MFG_CODE_KAMSTRUP              0x10b0
    //#define ZBEE_MFG_CODE_ECHOSTAR              0x10b1
    //#define ZBEE_MFG_CODE_ENERNOC               0x10b2
    //#define ZBEE_MFG_CODE_ELTAV                 0x10b3
    //#define ZBEE_MFG_CODE_BELKIN                0x10b4
    //#define ZBEE_MFG_CODE_XSTREAMHD             0x10b5
    //#define ZBEE_MFG_CODE_SATURN_SOUTH          0x10b6
    //#define ZBEE_MFG_CODE_GREENTRAP             0x10b7
    //#define ZBEE_MFG_CODE_SMARTSYNCH            0x10b8
    //#define ZBEE_MFG_CODE_NYCE                  0x10b9
    //#define ZBEE_MFG_CODE_ICM_CONTROLS          0x10ba
    //#define ZBEE_MFG_CODE_MILLENNIUM            0x10bb
    //#define ZBEE_MFG_CODE_MOTOROLA              0x10bc
    //#define ZBEE_MFG_CODE_EMERSON               0x10bd
    //#define ZBEE_MFG_CODE_RADIO_THERMOSTAT      0x10be
    //#define ZBEE_MFG_CODE_OMRON                 0x10bf

    //#define ZBEE_MFG_CODE_GIINII                0x10c0
    //#define ZBEE_MFG_CODE_FUJITSU               0x10c1
    //#define ZBEE_MFG_CODE_PEEL                  0x10c2
    //#define ZBEE_MFG_CODE_ACCENT                0x10c3
    //#define ZBEE_MFG_CODE_BYTESNAP              0x10c4
    //#define ZBEE_MFG_CODE_NEC_TOKIN             0x10c5
    //#define ZBEE_MFG_CODE_G4S_JUSTICE           0x10c6
    //#define ZBEE_MFG_CODE_TRILLIANT             0x10c7
    //#define ZBEE_MFG_CODE_ELECTROLUX            0x10c8
    //#define ZBEE_MFG_CODE_ONZO                  0x10c9
    //#define ZBEE_MFG_CODE_ENTEK                 0x10ca
    //#define ZBEE_MFG_CODE_PHILIPS2              0x10cb
    //#define ZBEE_MFG_CODE_MAINSTREAM            0x10cc
    //#define ZBEE_MFG_CODE_INDESIT               0x10cd
    //#define ZBEE_MFG_CODE_THINKECO              0x10ce
    //#define ZBEE_MFG_CODE_2D2C                  0x10cf

    //#define ZBEE_MFG_CODE_GREENPEAK             0x10d0
    //#define ZBEE_MFG_CODE_INTERCEL              0x10d1
    //#define ZBEE_MFG_CODE_LG                    0x10d2
    //#define ZBEE_MFG_CODE_MITSUMI1              0x10d3
    //#define ZBEE_MFG_CODE_MITSUMI2              0x10d4
    //#define ZBEE_MFG_CODE_ZENTRUM               0x10d5
    //#define ZBEE_MFG_CODE_NEST                  0x10d6
    //#define ZBEE_MFG_CODE_EXEGIN                0x10d7
    //#define ZBEE_MFG_CODE_HONEYWELL2            0x10d8
    //#define ZBEE_MFG_CODE_TAKAHATA              0x10d9
    //#define ZBEE_MFG_CODE_SUMITOMO              0x10da
    //#define ZBEE_MFG_CODE_GE_ENERGY             0x10db
    //#define ZBEE_MFG_CODE_GE_APPLIANCES         0x10dc
    //#define ZBEE_MFG_CODE_RADIOCRAFTS           0x10dd
    //#define ZBEE_MFG_CODE_CEIVA                 0x10de
    //#define ZBEE_MFG_CODE_TEC_CO                0x10df

    //#define ZBEE_MFG_CODE_CHAMELEON             0x10e0
    //#define ZBEE_MFG_CODE_SAMSUNG               0x10e1
    //#define ZBEE_MFG_CODE_RUWIDO                0x10e2
    //#define ZBEE_MFG_CODE_HUAWEI_1              0x10e3
    //#define ZBEE_MFG_CODE_HUAWEI_2              0x10e4
    //#define ZBEE_MFG_CODE_GREENWAVE             0x10e5
    //#define ZBEE_MFG_CODE_BGLOBAL               0x10e6
    //#define ZBEE_MFG_CODE_MINDTECK              0x10e7
    //#define ZBEE_MFG_CODE_INGERSOLL_RAND        0x10e8
    //#define ZBEE_MFG_CODE_DIUS                  0x10e9
    //#define ZBEE_MFG_CODE_EMBEDDED              0x10ea
    //#define ZBEE_MFG_CODE_ABB                   0x10eb
    //#define ZBEE_MFG_CODE_SONY                  0x10ec
    //#define ZBEE_MFG_CODE_GENUS                 0x10ed
    //#define ZBEE_MFG_CODE_UNIVERSAL1            0x10ee
    //#define ZBEE_MFG_CODE_UNIVERSAL2            0x10ef
    //#define ZBEE_MFG_CODE_METRUM                0x10f0
    //#define ZBEE_MFG_CODE_CISCO                 0x10f1
    //#define ZBEE_MFG_CODE_UBISYS                0x10f2
    //#define ZBEE_MFG_CODE_CONSERT               0x10f3
    //#define ZBEE_MFG_CODE_CRESTRON              0x10f4
    //#define ZBEE_MFG_CODE_ENPHASE               0x10f5
    //#define ZBEE_MFG_CODE_INVENSYS              0x10f6
    //#define ZBEE_MFG_CODE_MUELLER               0x10f7
    //#define ZBEE_MFG_CODE_AAC_TECH              0x10f8
    //#define ZBEE_MFG_CODE_U_NEXT                0x10f9
    //#define ZBEE_MFG_CODE_STEELCASE             0x10fa
    //#define ZBEE_MFG_CODE_TELEMATICS            0x10fb
    //#define ZBEE_MFG_CODE_SAMIL                 0x10fc
    //#define ZBEE_MFG_CODE_PACE                  0x10fd
    //#define ZBEE_MFG_CODE_OSBORNE               0x10fe
    //#define ZBEE_MFG_CODE_POWERWATCH            0x10ff
    //#define ZBEE_MFG_CODE_CANDELED              0x1100
    //#define ZBEE_MFG_CODE_FLEXGRID              0x1101
    //#define ZBEE_MFG_CODE_HUMAX                 0x1102
    //#define ZBEE_MFG_CODE_UNIVERSAL             0x1103
    //#define ZBEE_MFG_CODE_ADVANCED_ENERGY       0x1104
    //#define ZBEE_MFG_CODE_BEGA                  0x1105
    //#define ZBEE_MFG_CODE_BRUNEL                0x1106
    //#define ZBEE_MFG_CODE_PANASONIC             0x1107
    //#define ZBEE_MFG_CODE_ESYSTEMS              0x1108
    //#define ZBEE_MFG_CODE_PANAMAX               0x1109
    //#define ZBEE_MFG_CODE_PHYSICAL              0x110a
    //#define ZBEE_MFG_CODE_EM_LITE               0x110b
    //#define ZBEE_MFG_CODE_OSRAM                 0x110c
    //#define ZBEE_MFG_CODE_2_SAVE                0x110d
    //#define ZBEE_MFG_CODE_PLANET                0x110e
    //#define ZBEE_MFG_CODE_AMBIENT               0x110f
    //#define ZBEE_MFG_CODE_PROFALUX              0x1110
    //#define ZBEE_MFG_CODE_BILLION               0x1111
    //#define ZBEE_MFG_CODE_EMBERTEC              0x1112
    //#define ZBEE_MFG_CODE_IT_WATCHDOGS          0x1113
    //#define ZBEE_MFG_CODE_RELOC                 0x1114
    //#define ZBEE_MFG_CODE_INTEL                 0x1115
    //#define ZBEE_MFG_CODE_TREND                 0x1116
    //#define ZBEE_MFG_CODE_MOXA                  0x1117
    //#define ZBEE_MFG_CODE_QEES                  0x1118
    //#define ZBEE_MFG_CODE_SAYME                 0x1119
    //#define ZBEE_MFG_CODE_PENTAIR               0x111a
    //#define ZBEE_MFG_CODE_ORBIT                 0x111b
    //#define ZBEE_MFG_CODE_CALIFORNIA            0x111c
    //#define ZBEE_MFG_CODE_COMCAST2              0x111d
    //#define ZBEE_MFG_CODE_IDT                   0x111e
    //#define ZBEE_MFG_CODE_PIXELA                0x111f
    //#define ZBEE_MFG_CODE_TIVO                  0x1120
    //#define ZBEE_MFG_CODE_FIDURE                0x1121
    //#define ZBEE_MFG_CODE_MARVELL               0x1122
    //#define ZBEE_MFG_CODE_WASION                0x1123
    //#define ZBEE_MFG_CODE_JASCO                 0x1124
    //#define ZBEE_MFG_CODE_SHENZHEN              0x1125
    //#define ZBEE_MFG_CODE_NETCOMM               0x1126
    //#define ZBEE_MFG_CODE_DEFINE                0x1127
    //#define ZBEE_MFG_CODE_IN_HOME_DISP          0x1128
    //#define ZBEE_MFG_CODE_MIELE                 0x1129
    //#define ZBEE_MFG_CODE_TELEVES               0x112a
    //#define ZBEE_MFG_CODE_LABELEC               0x112b
    //#define ZBEE_MFG_CODE_CHINA_ELEC            0x112c
    //#define ZBEE_MFG_CODE_VECTORFORM            0x112d
    //#define ZBEE_MFG_CODE_BUSCH_JAEGER          0x112e
    //#define ZBEE_MFG_CODE_REDPINE               0x112f
    //#define ZBEE_MFG_CODE_BRIDGES               0x1130
    //#define ZBEE_MFG_CODE_SERCOMM               0x1131
    //#define ZBEE_MFG_CODE_WSH                   0x1132
    //#define ZBEE_MFG_CODE_BOSCH                 0x1133
    //#define ZBEE_MFG_CODE_EZEX                  0x1134
    //#define ZBEE_MFG_CODE_DRESDEN               0x1135
    //#define ZBEE_MFG_CODE_MEAZON                0x1136
    //#define ZBEE_MFG_CODE_CROW                  0x1137
    //#define ZBEE_MFG_CODE_HARVARD               0x1138
    //#define ZBEE_MFG_CODE_ANDSON                0x1139
    //#define ZBEE_MFG_CODE_ADHOCO                0x113a
    //#define ZBEE_MFG_CODE_WAXMAN                0x113b
    //#define ZBEE_MFG_CODE_OWON                  0x113c
    //#define ZBEE_MFG_CODE_HITRON                0x113d
    //#define ZBEE_MFG_CODE_SCEMTEC               0x113e
    //#define ZBEE_MFG_CODE_WEBEE                 0x113f
    //#define ZBEE_MFG_CODE_GRID2HOME             0x1140
    //#define ZBEE_MFG_CODE_TELINK                0x1141
    //#define ZBEE_MFG_CODE_JASMINE               0x1142
    //#define ZBEE_MFG_CODE_BIDGELY               0x1143
    //#define ZBEE_MFG_CODE_LUTRON                0x1144
    //#define ZBEE_MFG_CODE_IJENKO                0x1145
    //#define ZBEE_MFG_CODE_STARFIELD             0x1146
    //#define ZBEE_MFG_CODE_TCP                   0x1147
    //#define ZBEE_MFG_CODE_ROGERS                0x1148
    //#define ZBEE_MFG_CODE_CREE                  0x1149
    //#define ZBEE_MFG_CODE_ROBERT_BOSCH          0x114a
    //#define ZBEE_MFG_CODE_IBIS                  0x114b
    //#define ZBEE_MFG_CODE_QUIRKY                0x114c
    //#define ZBEE_MFG_CODE_EFERGY                0x114d
    //#define ZBEE_MFG_CODE_SMARTLABS             0x114e
    //#define ZBEE_MFG_CODE_EVERSPRING            0x114f
    //#define ZBEE_MFG_CODE_SWANN                 0x1150

    ///* Manufacturer Names */
    //#define ZBEE_MFG_CIRRONET                   "Cirronet"
    //#define ZBEE_MFG_CHIPCON                    "Chipcon"
    //#define ZBEE_MFG_EMBER                      "Ember"
    //#define ZBEE_MFG_NTS                        "National Tech"
    //#define ZBEE_MFG_FREESCALE                  "Freescale"
    //#define ZBEE_MFG_IPCOM                      "IPCom"
    //#define ZBEE_MFG_SAN_JUAN                   "San Juan Software"
    //#define ZBEE_MFG_TUV                        "TUV"
    //#define ZBEE_MFG_COMPXS                     "CompXs"
    //#define ZBEE_MFG_BM                         "BM SpA"
    //#define ZBEE_MFG_AWAREPOINT                 "AwarePoint"
    //#define ZBEE_MFG_PHILIPS                    "Philips"
    //#define ZBEE_MFG_LUXOFT                     "Luxoft"
    //#define ZBEE_MFG_KORWIN                     "Korvin"
    //#define ZBEE_MFG_1_RF                       "One RF"
    //#define ZBEE_MFG_STG                        "Software Technology Group"
    //#define ZBEE_MFG_TELEGESIS                  "Telegesis"
    //#define ZBEE_MFG_VISIONIC                   "Visionic"
    //#define ZBEE_MFG_INSTA                      "Insta"
    //#define ZBEE_MFG_ATALUM                     "Atalum"
    //#define ZBEE_MFG_ATMEL                      "Atmel"
    //#define ZBEE_MFG_DEVELCO                    "Develco"
    //#define ZBEE_MFG_HONEYWELL                  "Honeywell"
    //#define ZBEE_MFG_RADIO_PULSE                "RadioPulse"
    //#define ZBEE_MFG_RENESAS                    "Renesas"
    //#define ZBEE_MFG_XANADU                     "Xanadu Wireless"
    //#define ZBEE_MFG_NEC                        "NEC Engineering"
    //#define ZBEE_MFG_YAMATAKE                   "Yamatake"
    //#define ZBEE_MFG_TENDRIL                    "Tendril"
    //#define ZBEE_MFG_ASSA                       "Assa Abloy"
    //#define ZBEE_MFG_MAXSTREAM                  "Maxstream"
    //#define ZBEE_MFG_NEUROCOM                   "Neurocom"

    //#define ZBEE_MFG_III                        "Institute for Information Industry"
    //#define ZBEE_MFG_VANTAGE                    "Vantage Controls"
    //#define ZBEE_MFG_ICONTROL                   "iControl"
    //#define ZBEE_MFG_RAYMARINE                  "Raymarine"
    //#define ZBEE_MFG_LSR                        "LS Research"
    //#define ZBEE_MFG_ONITY                      "Onity"
    //#define ZBEE_MFG_MONO                       "Mono Products"
    //#define ZBEE_MFG_RFT                        "RF Tech"
    //#define ZBEE_MFG_ITRON                      "Itron"
    //#define ZBEE_MFG_TRITECH                    "Tritech"
    //#define ZBEE_MFG_EMBEDIT                    "Embedit"
    //#define ZBEE_MFG_S3C                        "S3C"
    //#define ZBEE_MFG_SIEMENS                    "Siemens"
    //#define ZBEE_MFG_MINDTECH                   "Mindtech"
    //#define ZBEE_MFG_LGE                        "LG Electronics"
    //#define ZBEE_MFG_MITSUBISHI                 "Mitsubishi"
    //#define ZBEE_MFG_JOHNSON                    "Johnson Controls"
    //#define ZBEE_MFG_PRI                        "PRI"
    //#define ZBEE_MFG_KNICK                      "Knick"
    //#define ZBEE_MFG_VICONICS                   "Viconics"
    //#define ZBEE_MFG_FLEXIPANEL                 "Flexipanel"
    //#define ZBEE_MFG_PIASIM                     "Piasim Corporation"
    //#define ZBEE_MFG_TRANE                      "Trane"
    //#define ZBEE_MFG_JENNIC                     "Jennic" -> NXP
    //#define ZBEE_MFG_LIG                        "Living Independently"
    //#define ZBEE_MFG_ALERTME                    "AlertMe"
    //#define ZBEE_MFG_DAINTREE                   "Daintree"
    //#define ZBEE_MFG_AIJI                       "Aiji"
    //#define ZBEE_MFG_TEL_ITALIA                 "Telecom Italia"
    //#define ZBEE_MFG_MIKROKRETS                 "Mikrokrets"
    //#define ZBEE_MFG_OKI_SEMI                   "Oki Semi"
    //#define ZBEE_MFG_NEWPORT                    "Newport Electronics"
    //#define ZBEE_MFG_C4                         "Control4"
    //#define ZBEE_MFG_STM                        "STMicro"
    //#define ZBEE_MFG_ASN                        "Ad-Sol Nissin"
    //#define ZBEE_MFG_DCSI                       "DCSI"
    //#define ZBEE_MFG_FRANCE_TEL                 "France Telecom"
    //#define ZBEE_MFG_MUNET                      "muNet"
    //#define ZBEE_MFG_AUTANI                     "Autani"
    //#define ZBEE_MFG_COL_VNET                   "Colorado vNet"
    //#define ZBEE_MFG_AEROCOMM                   "Aerocomm"
    //#define ZBEE_MFG_SI_LABS                    "Silicon Labs"
    //#define ZBEE_MFG_INNCOM                     "Inncom"
    //#define ZBEE_MFG_CANNON                     "Cannon"
    //#define ZBEE_MFG_SYNAPSE                    "Synapse"
    //#define ZBEE_MFG_FPS                        "Fisher Pierce/Sunrise"
    //#define ZBEE_MFG_CLS                        "CentraLite"
    //#define ZBEE_MFG_CRANE                      "Crane"
    //#define ZBEE_MFG_MOBILARM                   "Mobilarm"
    //#define ZBEE_MFG_IMONITOR                   "iMonitor"
    //#define ZBEE_MFG_BARTECH                    "Bartech"
    //#define ZBEE_MFG_MESHNETICS                 "Meshnetics"
    //#define ZBEE_MFG_LS_IND                     "LS Industrial"
    //#define ZBEE_MFG_CASON                      "Cason"
    //#define ZBEE_MFG_WLESS_GLUE                 "Wireless Glue"
    //#define ZBEE_MFG_ELSTER                     "Elster"
    //#define ZBEE_MFG_SMS_TEC                    "SMS Tec"
    //#define ZBEE_MFG_ONSET                      "Onset Computer"
    //#define ZBEE_MFG_RIGA                       "Riga Development"
    //#define ZBEE_MFG_ENERGATE                   "Energate"
    //#define ZBEE_MFG_CONMED                     "ConMed Linvatec"
    //#define ZBEE_MFG_POWERMAND                  "PowerMand"
    //#define ZBEE_MFG_SCHNEIDER                  "Schneider Electric"
    //#define ZBEE_MFG_EATON                      "Eaton"
    //#define ZBEE_MFG_TELULAR                    "Telular"
    //#define ZBEE_MFG_DELPHI                     "Delphi Medical"
    //#define ZBEE_MFG_EPISENSOR                  "EpiSensor"
    //#define ZBEE_MFG_LANDIS_GYR                 "Landis+Gyr"
    //#define ZBEE_MFG_KABA                       "Kaba Group"
    //#define ZBEE_MFG_SHURE                      "Shure"
    //#define ZBEE_MFG_COMVERGE                   "Comverge"
    //#define ZBEE_MFG_DBS_LODGING                "DBS Lodging"
    //#define ZBEE_MFG_ENERGY_AWARE               "Energy Aware"
    //#define ZBEE_MFG_HIDALGO                    "Hidalgo"
    //#define ZBEE_MFG_AIR2APP                    "Air2App"
    //#define ZBEE_MFG_AMX                        "AMX"
    //#define ZBEE_MFG_EDMI                       "EDMI Pty"
    //#define ZBEE_MFG_CYAN                       "Cyan Ltd"
    //#define ZBEE_MFG_SYS_SPA                    "System SPA"
    //#define ZBEE_MFG_TELIT                      "Telit"
    //#define ZBEE_MFG_KAGA                       "Kaga Electronics"
    //#define ZBEE_MFG_4_NOKS                     "4-noks s.r.l."
    //#define ZBEE_MFG_CERTICOM                   "Certicom"
    //#define ZBEE_MFG_GRIDPOINT                  "Gridpoint"
    //#define ZBEE_MFG_PROFILE_SYS                "Profile Systems"
    //#define ZBEE_MFG_COMPACTA                   "Compacta International"
    //#define ZBEE_MFG_FREESTYLE                  "Freestyle Technology"
    //#define ZBEE_MFG_ALEKTRONA                  "Alektrona"
    //#define ZBEE_MFG_COMPUTIME                  "Computime"
    //#define ZBEE_MFG_REMOTE_TECH                "Remote Technologies"
    //#define ZBEE_MFG_WAVECOM                    "Wavecom"
    //#define ZBEE_MFG_ENERGY                     "Energy Optimizers"
    //#define ZBEE_MFG_GE                         "GE"
    //#define ZBEE_MFG_JETLUN                     "Jetlun"
    //#define ZBEE_MFG_CIPHER                     "Cipher Systems"
    //#define ZBEE_MFG_CORPORATE                  "Corporate Systems Eng"
    //#define ZBEE_MFG_ECOBEE                     "ecobee"
    //#define ZBEE_MFG_SMK                        "SMK"
    //#define ZBEE_MFG_MESHWORKS                  "Meshworks Wireless"
    //#define ZBEE_MFG_ELLIPS                     "Ellips B.V."
    //#define ZBEE_MFG_SECURE                     "Secure electrans"
    //#define ZBEE_MFG_CEDO                       "CEDO"
    //#define ZBEE_MFG_TOSHIBA                    "Toshiba"
    //#define ZBEE_MFG_DIGI                       "Digi International"
    //#define ZBEE_MFG_UBILOGIX                   "Ubilogix"
    //#define ZBEE_MFG_ECHELON                    "Echelon"
    //#define ZBEE_MFG_GREEN_ENERGY               "Green Energy Options"
    //#define ZBEE_MFG_SILVER_SPRING              "Silver Spring Networks"
    //#define ZBEE_MFG_BLACK                      "Black & Decker"
    //#define ZBEE_MFG_AZTECH_ASSOC               "Aztech AssociatesInc."
    //#define ZBEE_MFG_A_AND_D                    "A&D Co"
    //#define ZBEE_MFG_RAINFOREST                 "Rainforest Automation"
    //#define ZBEE_MFG_CARRIER                    "Carrier Electronics"
    //#define ZBEE_MFG_SYCHIP                     "SyChip/Murata"
    //#define ZBEE_MFG_OPEN_PEAK                  "OpenPeak"
    //#define ZBEE_MFG_PASSIVE                    "Passive Systems"
    //#define ZBEE_MFG_G4S_JUSTICE                "G4S JusticeServices"
    //#define ZBEE_MFG_MMB                        "MMBResearch"
    //#define ZBEE_MFG_LEVITON                    "Leviton"
    //#define ZBEE_MFG_KOREA_ELEC                 "Korea Electric Power Data Network"
    //#define ZBEE_MFG_COMCAST                    "Comcast"
    //#define ZBEE_MFG_NEC_ELEC                   "NEC Electronics"
    //#define ZBEE_MFG_NETVOX                     "Netvox"
    //#define ZBEE_MFG_UCONTROL                   "U-Control"
    //#define ZBEE_MFG_EMBEDIA                    "Embedia Technologies"
    //#define ZBEE_MFG_SENSUS                     "Sensus"
    //#define ZBEE_MFG_SUNRISE                    "SunriseTechnologies"
    //#define ZBEE_MFG_MEMTECH                    "MemtechCorp"
    //#define ZBEE_MFG_FREEBOX                    "Freebox"
    //#define ZBEE_MFG_M2_LABS                    "M2 Labs"
    //#define ZBEE_MFG_BRITISH_GAS                "BritishGas"
    //#define ZBEE_MFG_SENTEC                     "Sentec"
    //#define ZBEE_MFG_NAVETAS                    "Navetas"
    //#define ZBEE_MFG_LIGHTSPEED                 "Lightspeed Technologies"
    //#define ZBEE_MFG_OKI                        "Oki Electric"
    //#define ZBEE_MFG_SISTEMAS                   "Sistemas Inteligentes"
    //#define ZBEE_MFG_DOMETIC                    "Dometic"
    //#define ZBEE_MFG_APLS                       "Alps"
    //#define ZBEE_MFG_ENERGY_HUB                 "EnergyHub"
    //#define ZBEE_MFG_KAMSTRUP                   "Kamstrup"
    //#define ZBEE_MFG_ECHOSTAR                   "EchoStar"
    //#define ZBEE_MFG_ENERNOC                    "EnerNOC"
    //#define ZBEE_MFG_ELTAV                      "Eltav"
    //#define ZBEE_MFG_BELKIN                     "Belkin"
    //#define ZBEE_MFG_XSTREAMHD                  "XStreamHD Wireless"
    //#define ZBEE_MFG_SATURN_SOUTH               "Saturn South"
    //#define ZBEE_MFG_GREENTRAP                  "GreenTrapOnline"
    //#define ZBEE_MFG_SMARTSYNCH                 "SmartSynch"
    //#define ZBEE_MFG_NYCE                       "Nyce Control"
    //#define ZBEE_MFG_ICM_CONTROLS               "ICM Controls"
    //#define ZBEE_MFG_MILLENNIUM                 "Millennium Electronics"
    //#define ZBEE_MFG_MOTOROLA                   "Motorola"
    //#define ZBEE_MFG_EMERSON                    "EmersonWhite-Rodgers"
    //#define ZBEE_MFG_RADIO_THERMOSTAT           "Radio Thermostat"
    //#define ZBEE_MFG_OMRON                      "OMRONCorporation"
    //#define ZBEE_MFG_GIINII                     "GiiNii GlobalLimited"
    //#define ZBEE_MFG_FUJITSU                    "Fujitsu GeneralLimited"
    //#define ZBEE_MFG_PEEL                       "Peel Technologies"
    //#define ZBEE_MFG_ACCENT                     "Accent"
    //#define ZBEE_MFG_BYTESNAP                   "ByteSnap Design"
    //#define ZBEE_MFG_NEC_TOKIN                  "NEC TOKIN Corporation"
    //#define ZBEE_MFG_TRILLIANT                  "Trilliant Networks"
    //#define ZBEE_MFG_ELECTROLUX                 "Electrolux Italia"
    //#define ZBEE_MFG_ONZO                       "OnzoLtd"
    //#define ZBEE_MFG_ENTEK                      "EnTekSystems"
    ///**/
    //#define ZBEE_MFG_MAINSTREAM                 "MainstreamEngineering"
    //#define ZBEE_MFG_INDESIT                    "IndesitCompany"
    //#define ZBEE_MFG_THINKECO                   "THINKECO"
    //#define ZBEE_MFG_2D2C                       "2D2C"
    //#define ZBEE_MFG_GREENPEAK                  "GreenPeak"
    //#define ZBEE_MFG_INTERCEL                   "InterCEL"
    //#define ZBEE_MFG_LG                         "LG Electronics"
    //#define ZBEE_MFG_MITSUMI1                   "Mitsumi Electric"
    //#define ZBEE_MFG_MITSUMI2                   "Mitsumi Electric"
    //#define ZBEE_MFG_ZENTRUM                    "Zentrum Mikroelektronik Dresden"
    //#define ZBEE_MFG_NEST                       "Nest Labs"
    //#define ZBEE_MFG_EXEGIN                     "Exegin Technologies"
    //#define ZBEE_MFG_HONEYWELL                  "Honeywell"
    //#define ZBEE_MFG_TAKAHATA                   "Takahata Precision"
    //#define ZBEE_MFG_SUMITOMO                   "Sumitomo Electric Networks"
    //#define ZBEE_MFG_GE_ENERGY                  "GE Energy"
    //#define ZBEE_MFG_GE_APPLIANCES              "GE Appliances"
    //#define ZBEE_MFG_RADIOCRAFTS                "Radiocrafts AS"
    //#define ZBEE_MFG_CEIVA                      "Ceiva"
    //#define ZBEE_MFG_TEC_CO                     "TEC CO Co., Ltd"
    //#define ZBEE_MFG_CHAMELEON                  "Chameleon Technology (UK) Ltd"
    //#define ZBEE_MFG_SAMSUNG                    "Samsung"
    //#define ZBEE_MFG_RUWIDO                     "ruwido austria gmbh"
    //#define ZBEE_MFG_HUAWEI                     "Huawei Technologies Co., Ltd."
    //#define ZBEE_MFG_GREENWAVE                  "Greenwave Reality"
    //#define ZBEE_MFG_BGLOBAL                    "BGlobal Metering Ltd"
    //#define ZBEE_MFG_MINDTECK                   "Mindteck"
    //#define ZBEE_MFG_INGERSOLL_RAND             "Ingersoll-Rand"
    //#define ZBEE_MFG_DIUS                       "Dius Computing Pty Ltd"
    //#define ZBEE_MFG_EMBEDDED                   "Embedded Automation, Inc."
    //#define ZBEE_MFG_ABB                        "ABB"
    //#define ZBEE_MFG_SONY                       "Sony"
    //#define ZBEE_MFG_GENUS                      "Genus Power Infrastructures Limited"
    //#define ZBEE_MFG_UNIVERSA L                 "Universal Electronics, Inc."
    //#define ZBEE_MFG_METRUM                     "Metrum Technologies, LLC"
    //#define ZBEE_MFG_CISCO                      "Cisco"
    //#define ZBEE_MFG_UBISYS                     "Ubisys technologies GmbH"
    //#define ZBEE_MFG_CONSERT                    "Consert"
    //#define ZBEE_MFG_CRESTRON                   "Crestron Electronics"
    //#define ZBEE_MFG_ENPHASE                    "Enphase Energy"
    //#define ZBEE_MFG_INVENSYS                   "Invensys Controls"
    //#define ZBEE_MFG_MUELLER                    "Mueller Systems, LLC"
    //#define ZBEE_MFG_AAC_TECH                   "AAC Technologies Holding"
    //#define ZBEE_MFG_U_NEXT                     "U-NEXT Co., Ltd"
    //#define ZBEE_MFG_STEELCASE                  "Steelcase Inc."
    //#define ZBEE_MFG_TELEMATICS                 "Telematics Wireless"
    //#define ZBEE_MFG_SAMIL                      "Samil Power Co., Ltd"
    //#define ZBEE_MFG_PACE                       "Pace Plc"
    //#define ZBEE_MFG_OSBORNE                    "Osborne Coinage Co."
    //#define ZBEE_MFG_POWERWATCH                 "Powerwatch"
    //#define ZBEE_MFG_CANDELED                   "CANDELED GmbH"
    //#define ZBEE_MFG_FLEXGRID                   "FlexGrid S.R.L"
    //#define ZBEE_MFG_HUMAX                      "Humax"
    //#define ZBEE_MFG_UNIVERSAL                  "Universal Devices"
    //#define ZBEE_MFG_ADVANCED_ENERGY            "Advanced Energy"
    //#define ZBEE_MFG_BEGA                       "BEGA Gantenbrink-Leuchten"
    //#define ZBEE_MFG_BRUNEL                     "Brunel University"
    //#define ZBEE_MFG_PANASONIC                  "Panasonic R&D Center Singapore"
    //#define ZBEE_MFG_ESYSTEMS                   "eSystems Research"
    //#define ZBEE_MFG_PANAMAX                    "Panamax"
    //#define ZBEE_MFG_PHYSICAL                   "Physical Graph Corporation"
    //#define ZBEE_MFG_EM_LITE                    "EM-Lite Ltd."
    //#define ZBEE_MFG_OSRAM                      "Osram Sylvania"
    //#define ZBEE_MFG_2_SAVE                     "2 Save Energy Ltd."
    //#define ZBEE_MFG_PLANET                     "Planet Innovation Products Pty Ltd"
    //#define ZBEE_MFG_AMBIENT                    "Ambient Devices, Inc."
    //#define ZBEE_MFG_PROFALUX                   "Profalux"
    //#define ZBEE_MFG_BILLION                    "Billion Electric Company (BEC)"
    //#define ZBEE_MFG_EMBERTEC                   "Embertec Pty Ltd"
    //#define ZBEE_MFG_IT_WATCHDOGS               "IT Watchdogs"
    //#define ZBEE_MFG_RELOC                      "Reloc"
    //#define ZBEE_MFG_INTEL                      "Intel Corporation"
    //#define ZBEE_MFG_TREND                      "Trend Electronics Limited"
    //#define ZBEE_MFG_MOXA                       "Moxa"
    //#define ZBEE_MFG_QEES                       "QEES"
    //#define ZBEE_MFG_SAYME                      "SAYME Wireless Sensor Networks"
    //#define ZBEE_MFG_PENTAIR                    "Pentair Aquatic Systems"
    //#define ZBEE_MFG_ORBIT                      "Orbit Irrigation"
    //#define ZBEE_MFG_CALIFORNIA                 "California Eastern Laboratories"
    //#define ZBEE_MFG_COMCAST                    "Comcast"
    //#define ZBEE_MFG_IDT                        "IDT Technology Limited"
    //#define ZBEE_MFG_PIXELA                     "Pixela"
    //#define ZBEE_MFG_TIVO                       "TiVo"
    //#define ZBEE_MFG_FIDURE                     "Fidure"
    //#define ZBEE_MFG_MARVELL                    "Marvell Semiconductor"
    //#define ZBEE_MFG_WASION                     "Wasion Group"
    //#define ZBEE_MFG_JASCO                      "Jasco Products"
    //#define ZBEE_MFG_SHENZHEN                   "Shenzhen Kaifa Technology"
    //#define ZBEE_MFG_NETCOMM                    "Netcomm Wireless"
    //#define ZBEE_MFG_DEFINE                     "Define Instruments"
    //#define ZBEE_MFG_IN_HOME_DISP               "In Home Displays"
    //#define ZBEE_MFG_MIELE                      "Miele & Cie. KG"
    //#define ZBEE_MFG_TELEVES                    "Televes S.A."
    //#define ZBEE_MFG_LABELEC                    "Labelec"
    //#define ZBEE_MFG_CHINA_ELEC                 "China Electronics Standardization Institute"
    //#define ZBEE_MFG_VECTORFORM                 "Vectorform"
    //#define ZBEE_MFG_BUSCH_JAEGER               "Busch-Jaeger Elektro"
    //#define ZBEE_MFG_REDPINE                    "Redpine Signals"
    //#define ZBEE_MFG_BRIDGES                    "Bridges Electronic Technology"
    //#define ZBEE_MFG_SERCOMM                    "Sercomm"
    //#define ZBEE_MFG_WSH                        "WSH GmbH wirsindheller"
    //#define ZBEE_MFG_BOSCH                      "Bosch Security Systems"
    //#define ZBEE_MFG_EZEX                       "eZEX Corporation"
    //#define ZBEE_MFG_DRESDEN                    "Dresden Elektronik Ingenieurtechnik GmbH"
    //#define ZBEE_MFG_MEAZON                     "MEAZON S.A."
    //#define ZBEE_MFG_CROW                       "Crow Electronic Engineering"
    //#define ZBEE_MFG_HARVARD                    "Harvard Engineering"
    //#define ZBEE_MFG_ANDSON                     "Andson(Beijing) Technology"
    //#define ZBEE_MFG_ADHOCO                     "Adhoco AG"
    //#define ZBEE_MFG_WAXMAN                     "Waxman Consumer Products Group"
    //#define ZBEE_MFG_OWON                       "Owon Technology"
    //#define ZBEE_MFG_HITRON                     "Hitron Technologies"
    //#define ZBEE_MFG_SCEMTEC                    "Scemtec Steuerungstechnik GmbH"
    //#define ZBEE_MFG_WEBEE                      "Webee"
    //#define ZBEE_MFG_GRID2HOME                  "Grid2Home"
    //#define ZBEE_MFG_TELINK                     "Telink Micro"
    //#define ZBEE_MFG_JASMINE                    "Jasmine Systems"
    //#define ZBEE_MFG_BIDGELY                    "Bidgely"
    //#define ZBEE_MFG_LUTRON                     "Lutron"
    //#define ZBEE_MFG_IJENKO                     "IJENKO"
    //#define ZBEE_MFG_STARFIELD                  "Starfield Electronic"
    //#define ZBEE_MFG_TCP                        "TCP"
    //#define ZBEE_MFG_ROGERS                     "Rogers Communications Partnership"
    //#define ZBEE_MFG_CREE                       "Cree"
    //#define ZBEE_MFG_ROBERT_BOSCH               "Robert Bosch"
    //#define ZBEE_MFG_IBIS                       "Ibis Networks"
    //#define ZBEE_MFG_QUIRKY                     "Quirky"
    //#define ZBEE_MFG_EFERGY                     "Efergy Technologies"
    //#define ZBEE_MFG_SMARTLABS                  "Smartlabs"
    //#define ZBEE_MFG_EVERSPRING                 "Everspring Industry"
    //#define ZBEE_MFG_SWANN                      "Swann Communications"
    //#define ZBEE_MFG_TI                         "Texas Instruments"

};

QDebug operator<<(QDebug debug, const Zigbee::ApsdeDataConfirm &confirm);
QDebug operator<<(QDebug debug, const Zigbee::ApsdeDataIndication &indication);
QDebug operator<<(QDebug debug, const Zigbee::ApsdeDataAck &acknowledgement);

Q_DECLARE_OPERATORS_FOR_FLAGS(Zigbee::ZigbeeChannels)
Q_DECLARE_OPERATORS_FOR_FLAGS(Zigbee::ZigbeeTxOptions)

#endif // ZIGBEE_H
