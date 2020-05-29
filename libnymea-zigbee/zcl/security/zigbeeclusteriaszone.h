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

#ifndef ZIGBEECLUSTERIASZONE_H
#define ZIGBEECLUSTERIASZONE_H

#include <QObject>

#include "zcl/zigbeecluster.h"
#include "zcl/zigbeeclusterreply.h"

class ZigbeeNode;
class ZigbeeNetwork;
class ZigbeeNodeEndpoint;
class ZigbeeNetworkReply;

class ZigbeeClusterIasZone : public ZigbeeCluster
{
    Q_OBJECT

    friend class ZigbeeNode;
    friend class ZigbeeNetwork;

public:

    enum Attribute {
        // Zone information attribute set
        AttributeZoneState = 0x0000,
        AttributeZoneType = 0x0001,
        AttributeZoneStatus = 0x0002,
        // Zone settings attribute set
        AttributeCieAddress = 0x0010,
        AttributeZoneId = 0x0011,
        AttributeNumberOfZoneSensitivityLevelsSupported = 0x0012,
        AttributeCurrentZoneSensitivityLevel = 0x0013
    };
    Q_ENUM(Attribute)

    enum ZoneState {
        ZoneStateNotEnrolled = 0x00,
        ZoneStateEnrolled = 0x01
    };
    Q_ENUM(ZoneState)

    enum ZoneType {
        ZoneTypeStandardCIE = 0x0000,
        ZoneTypeMotionSensor = 0x000d,
        ZoneTypeContactSwitch = 0x0015,
        ZoneTypeFireSensor = 0x0028,
        ZoneTypeWaterSensor = 0x002a,
        ZoneTypeCarbonMonoxideSensor = 0x002b,
        ZoneTypePersonalEmergencyDevice = 0x002c,
        ZoneTypeVibrationMovementSensor = 0x002d,
        ZoneTypeRemoteControl = 0x010f,
        ZoneTypeKeyFob = 0x0115,
        ZoneTypeKeypad = 0x021d,
        ZoneTypeStandardWarningDevice = 0x0225,
        ZoneTypeGlassBreakSensor = 0x0226,
        ZoneTypeSecurityRepater = 0x0229,
        ZoneTypeInvalidZone = 0xffff
    };
    Q_ENUM(ZoneType)

    enum ZoneStatusFlag {
        ZoneStatusFlagAlarm1 = 0x0001,
        ZoneStatusFlagAlarm2 = 0x0002,
        ZoneStatusFlagTamper = 0x0004,
        ZoneStatusFlagBattery = 0x0008,
        ZoneStatusFlagSupervisionReports = 0x0010,
        ZoneStatusFlagRestoreReports = 0x0020,
        ZoneStatusFlagTrouble = 0x0040,
        ZoneStatusFlagAcMains = 0x0080,
        ZoneStatusFlagTest = 0x0100,
        ZoneStatusFlagBatteryDefect = 0x0200
    };
    Q_ENUM(ZoneStatusFlag)
    Q_DECLARE_FLAGS(ZoneStatusFlags, ZoneStatusFlag)

    enum EnrollResponseCode {
        EnrollResponseCodeSuccess = 0x00,
        EnrollResponseCodeNotSupported = 0x01,
        EnrollResponseCodeNoEnrollPermit = 0x02,
        EnrollResponseCodeToManyZones = 0x03
    };
    Q_ENUM(EnrollResponseCode)

    enum ClientCommand {
        ClientCommandStatusChangedNotification = 0x00, // M
        ClientCommandZoneEnrollRequest = 0x01 // M
    };
    Q_ENUM(ClientCommand)

    enum ServerCommand {
        ServerCommandEnrollResponse = 0x00, // M
        ServerCommandInitNormalOperationMode = 0x01, // O
        ServerCommandInitTestMode = 0x02 // O
    };
    Q_ENUM(ServerCommand)

    explicit ZigbeeClusterIasZone(ZigbeeNetwork *network, ZigbeeNode *node, ZigbeeNodeEndpoint *endpoint, Direction direction, QObject *parent = nullptr);

    // TODO: write server commands

private:
    void setAttribute(const ZigbeeClusterAttribute &attribute) override;

protected:
    void processDataIndication(ZigbeeClusterLibrary::Frame frame) override;

signals:
    void zoneStatusChanged(ZoneStatusFlags zoneStatus, quint8 extendedStatus, quint8 zoneId, quint16 delay);
    void zoneEnrollRequest(ZoneType zoneType, quint16 manufacturerCode);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(ZigbeeClusterIasZone::ZoneStatusFlags)

#endif // ZIGBEECLUSTERIASZONE_H
