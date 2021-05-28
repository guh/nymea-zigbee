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

#ifndef ZIGBEECLUSTER_H
#define ZIGBEECLUSTER_H

#include <QObject>

#include "zigbee.h"
#include "zigbeeclusterreply.h"
#include "zigbeeclusterlibrary.h"
#include "zigbeeclusterattribute.h"

struct ZigbeeClusterReportConfigurationRecord {
    quint8 direction;
    Zigbee::DataType dataType;
    quint16 attributeId;
    quint16 minInterval;
    quint16 maxInterval;
    quint16 timeout;
    quint8 change;
};

typedef struct ZigbeeClusterAttributeReport {
    quint16 sourceAddress;
    quint8 endpointId;
    ZigbeeClusterLibrary::ClusterId clusterId;
    quint16 attributeId;
    ZigbeeClusterLibrary::Status attributeStatus;
    Zigbee::DataType dataType;
    QByteArray data;
} ZigbeeClusterAttributeReport;

class ZigbeeNode;
class ZigbeeNetwork;
class ZigbeeNodeEndpoint;
class ZigbeeNetworkReply;

class ZigbeeCluster : public QObject
{
    Q_OBJECT

    friend class ZigbeeNode;
    friend class ZigbeeNetwork;
    friend class ZigbeeNetworkDatabase;

public:
    enum Direction {
        Server,
        Client
    };
    Q_ENUM(Direction)

    explicit ZigbeeCluster(ZigbeeNetwork *network, ZigbeeNode *node, ZigbeeNodeEndpoint *endpoint, ZigbeeClusterLibrary::ClusterId clusterId, Direction direction, QObject *parent = nullptr);

    ZigbeeNode *node() const;
    ZigbeeNodeEndpoint *endpoint() const;

    Direction direction() const;

    ZigbeeClusterLibrary::ClusterId clusterId() const;
    QString clusterName() const;

    QList<ZigbeeClusterAttribute> attributes() const;
    bool hasAttribute(quint16 attributeId) const;
    ZigbeeClusterAttribute attribute(quint16 attributeId);

    // ZCL global commands
    ZigbeeClusterReply *readAttributes(QList<quint16> attributes, quint16 manufacturerCode = 0x0000);
    ZigbeeClusterReply *writeAttributes(QList<ZigbeeClusterLibrary::WriteAttributeRecord> writeAttributeRecords, quint16 manufacturerCode = 0x0000);
    ZigbeeClusterReply *configureReporting(QList<ZigbeeClusterLibrary::AttributeReportingConfiguration> reportingConfigurations, quint16 manufacturerCode = 0x0000);

protected:
    ZigbeeNetwork *m_network = nullptr;
    ZigbeeNode *m_node = nullptr;
    ZigbeeNodeEndpoint *m_endpoint= nullptr;

    ZigbeeClusterLibrary::ClusterId m_clusterId = ZigbeeClusterLibrary::ClusterIdUnknown;
    Direction m_direction = Server;
    QHash<quint16, ZigbeeClusterAttribute> m_attributes;

    // Helper methods for sending cluster specific commands
    ZigbeeNetworkRequest createGeneralRequest();
    quint8 m_transactionSequenceNumber = 0;
    QHash<quint8, ZigbeeClusterReply *> m_pendingReplies;

    // Global commands
    ZigbeeClusterReply *executeGlobalCommand(quint8 command, const QByteArray &payload = QByteArray(), quint16 manufacturerCode = 0x0000);
    ZigbeeClusterReply *executeManufacturerSpecificGlobalCommand(quint8 command, quint16 manufacturerCode, const QByteArray &payload = QByteArray());

    // Cluster specific
    ZigbeeClusterReply *createClusterReply(const ZigbeeNetworkRequest &request, ZigbeeClusterLibrary::Frame frame);
    ZigbeeClusterReply *executeClusterCommand(quint8 command, const QByteArray &payload = QByteArray());

    ZigbeeClusterReply *sendClusterServerResponse(quint8 command, quint8 transactionSequenceNumber, const QByteArray &payload = QByteArray());
    ZigbeeClusterReply *sendDefaultResponse(quint8 transactionSequenceNumber, quint8 command, quint8 status);

    bool verifyNetworkError(ZigbeeClusterReply *zclReply, ZigbeeNetworkReply *networkReply);
    void finishZclReply(ZigbeeClusterReply *zclReply);

    virtual void processDataIndication(ZigbeeClusterLibrary::Frame frame);

    void updateOrAddAttribute(const ZigbeeClusterAttribute &attribute);

private:
    virtual void setAttribute(const ZigbeeClusterAttribute &attribute);

signals:
    void attributeChanged(const ZigbeeClusterAttribute &attribute);

public slots:
    void processApsDataIndication(const QByteArray &asdu, const ZigbeeClusterLibrary::Frame &frame);

};

QDebug operator<<(QDebug debug, ZigbeeCluster *cluster);
QDebug operator<<(QDebug debug, const ZigbeeClusterAttributeReport &attributeReport);

#endif // ZIGBEECLUSTER_H
