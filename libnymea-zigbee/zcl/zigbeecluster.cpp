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

#include "zigbeeutils.h"
#include "zigbeenetwork.h"
#include "zigbeecluster.h"
#include "loggingcategory.h"
#include "zigbeenetworkreply.h"
#include "zigbeeclusterlibrary.h"
#include "zigbeenetworkrequest.h"

#include <QDataStream>

ZigbeeCluster::ZigbeeCluster(ZigbeeNetwork *network, ZigbeeNode *node, ZigbeeNodeEndpoint *endpoint, ZigbeeClusterLibrary::ClusterId clusterId, Direction direction, QObject *parent) :
    QObject(parent),
    m_network(network),
    m_node(node),
    m_endpoint(endpoint),
    m_clusterId(clusterId),
    m_direction(direction)
{
    //qCDebug(dcZigbeeCluster()) << "Create cluster" << ZigbeeUtils::convertUint16ToHexString(clusterId) << direction;
}

ZigbeeNode *ZigbeeCluster::node() const
{
    return m_node;
}

ZigbeeNodeEndpoint *ZigbeeCluster::endpoint() const
{
    return m_endpoint;
}

ZigbeeCluster::Direction ZigbeeCluster::direction() const
{
    return m_direction;
}

ZigbeeClusterLibrary::ClusterId ZigbeeCluster::clusterId() const
{
    return m_clusterId;
}

QString ZigbeeCluster::clusterName() const
{
    return ZigbeeUtils::clusterIdToString(static_cast<ZigbeeClusterLibrary::ClusterId>(m_clusterId));
}

QList<ZigbeeClusterAttribute> ZigbeeCluster::attributes() const
{
    return m_attributes.values();
}

bool ZigbeeCluster::hasAttribute(quint16 attributeId) const
{
    if (m_attributes.keys().isEmpty())
        return false;

    return m_attributes.keys().contains(attributeId);
}

ZigbeeClusterAttribute ZigbeeCluster::attribute(quint16 attributeId) const
{
    return m_attributes.value(attributeId);
}

void ZigbeeCluster::setAttribute(const ZigbeeClusterAttribute &attribute)
{
    qCDebug(dcZigbeeCluster()) << "Update attribute" << m_node << m_endpoint << this << attribute;
    if (hasAttribute(attribute.id())) {
        m_attributes[attribute.id()] = attribute;
        emit attributeChanged(attribute);
    } else {
        m_attributes.insert(attribute.id(), attribute);
        emit attributeChanged(attribute);
    }
}

ZigbeeClusterReply *ZigbeeCluster::readAttributes(QList<quint16> attributes)
{
    qCDebug(dcZigbeeCluster()) << "Read attributes from" << m_node << m_endpoint << this << attributes;

    // ZCL payload
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    foreach (quint16 attribute, attributes) {
        stream << attribute;
    }

    return executeGlobalCommand(ZigbeeClusterLibrary::CommandReadAttributes, payload);
}

ZigbeeClusterReply *ZigbeeCluster::writeAttributes(QList<ZigbeeClusterLibrary::WriteAttributeRecord> writeAttributeRecords)
{
    qCDebug(dcZigbeeCluster()) << "Write attributes on" << m_node << m_endpoint << this;
    QByteArray payload;
    foreach (const ZigbeeClusterLibrary::WriteAttributeRecord &writeAttributeRecord, writeAttributeRecords) {
        payload += ZigbeeClusterLibrary::buildWriteAttributeRecord(writeAttributeRecord);
    }

    return executeGlobalCommand(ZigbeeClusterLibrary::CommandWriteAttributes, payload);
}

ZigbeeClusterReply *ZigbeeCluster::configureReporting(QList<ZigbeeClusterLibrary::AttributeReportingConfiguration> reportingConfigurations)
{
    qCDebug(dcZigbeeCluster()) << "Configure reporting on" << m_node << m_endpoint << this << reportingConfigurations;

    QByteArray payload;
    foreach (const ZigbeeClusterLibrary::AttributeReportingConfiguration reportingConfiguration, reportingConfigurations) {
        payload += ZigbeeClusterLibrary::buildAttributeReportingConfiguration(reportingConfiguration);
    }

    return executeGlobalCommand(ZigbeeClusterLibrary::CommandConfigureReporting, payload);
}


ZigbeeClusterReply *ZigbeeCluster::executeGlobalCommand(quint8 command, const QByteArray &payload)
{
    // Build the request
    ZigbeeNetworkRequest request = createGeneralRequest();

    // Build ZCL frame

    // Note: for basic commands the frame control files has to be zero accoring to spec ZCL 2.4.1.1
    ZigbeeClusterLibrary::FrameControl frameControl;
    frameControl.frameType = ZigbeeClusterLibrary::FrameTypeGlobal;
    frameControl.manufacturerSpecific = false;
    if (m_direction == Direction::Server) {
        frameControl.direction = ZigbeeClusterLibrary::DirectionClientToServer;
    } else {
        frameControl.direction = ZigbeeClusterLibrary::DirectionServerToClient;
    }
    frameControl.disableDefaultResponse = true;

    // ZCL header
    ZigbeeClusterLibrary::Header header;
    header.frameControl = frameControl;
    header.command = command;
    header.transactionSequenceNumber = m_transactionSequenceNumber++;

    // Put them together
    ZigbeeClusterLibrary::Frame frame;
    frame.header = header;
    frame.payload = payload;

    request.setTxOptions(Zigbee::ZigbeeTxOptions(Zigbee::ZigbeeTxOptionAckTransmission));
    request.setAsdu(ZigbeeClusterLibrary::buildFrame(frame));

    ZigbeeClusterReply *zclReply = createClusterReply(request, frame);
    ZigbeeNetworkReply *networkReply = m_network->sendRequest(request);
    connect(networkReply, &ZigbeeNetworkReply::finished, this, [this, networkReply, zclReply](){
        if (!verifyNetworkError(zclReply, networkReply)) {
            qCWarning(dcZigbeeClusterLibrary()) << "Failed to read attributes from" << m_node << m_endpoint << this << networkReply->error();
            finishZclReply(zclReply);
            return;
        }

        // The request was successfully sent to the device
        // Now check if the expected indication response received already
        if (zclReply->isComplete()) {
            finishZclReply(zclReply);
            return;
        }
    });

    return zclReply;
}


ZigbeeClusterReply *ZigbeeCluster::createClusterReply(const ZigbeeNetworkRequest &request, ZigbeeClusterLibrary::Frame frame)
{
    ZigbeeClusterReply *zclReply = new ZigbeeClusterReply(request, frame, this);
    connect(zclReply, &ZigbeeClusterReply::finished, zclReply, &ZigbeeClusterReply::deleteLater, Qt::QueuedConnection);
    zclReply->m_transactionSequenceNumber = frame.header.transactionSequenceNumber;
    m_pendingReplies.insert(zclReply->transactionSequenceNumber(), zclReply);
    return zclReply;
}

ZigbeeClusterReply *ZigbeeCluster::executeClusterCommand(quint8 command, const QByteArray &payload)
{
    ZigbeeNetworkRequest request = createGeneralRequest();

    // Build ZCL frame control
    ZigbeeClusterLibrary::FrameControl frameControl;
    frameControl.frameType = ZigbeeClusterLibrary::FrameTypeClusterSpecific;
    frameControl.manufacturerSpecific = false;
    frameControl.direction = ZigbeeClusterLibrary::DirectionClientToServer;
    frameControl.disableDefaultResponse = false;

    // Build ZCL header
    ZigbeeClusterLibrary::Header header;
    header.frameControl = frameControl;
    header.command = command;
    header.transactionSequenceNumber = m_transactionSequenceNumber++;

    // Build ZCL frame
    ZigbeeClusterLibrary::Frame frame;
    frame.header = header;
    frame.payload = payload;

    request.setTxOptions(Zigbee::ZigbeeTxOptions(Zigbee::ZigbeeTxOptionAckTransmission));
    request.setAsdu(ZigbeeClusterLibrary::buildFrame(frame));

    ZigbeeClusterReply *zclReply = createClusterReply(request, frame);
    qCDebug(dcZigbeeCluster()) << "Executing command" << ZigbeeUtils::convertByteToHexString(command) << ZigbeeUtils::convertByteArrayToHexString(payload);
    ZigbeeNetworkReply *networkReply = m_network->sendRequest(request);
    connect(networkReply, &ZigbeeNetworkReply::finished, this, [this, networkReply, zclReply](){
        if (!verifyNetworkError(zclReply, networkReply)) {
            qCWarning(dcZigbeeClusterLibrary()) << "Failed to send request"
                                                << m_node << networkReply->error()
                                                << networkReply->zigbeeApsStatus();
            finishZclReply(zclReply);
            return;
        }

        // The request was successfully sent to the device
        // Now check if the expected indication response received already
        if (zclReply->isComplete()) {
            finishZclReply(zclReply);
            return;
        }
    });

    return zclReply;
}

ZigbeeNetworkRequest ZigbeeCluster::createGeneralRequest()
{
    // Build the request
    ZigbeeNetworkRequest request;
    request.setRequestId(m_network->generateSequenceNumber());
    request.setDestinationAddressMode(Zigbee::DestinationAddressModeShortAddress);
    request.setDestinationShortAddress(m_node->shortAddress());
    request.setProfileId(Zigbee::ZigbeeProfileHomeAutomation); // Note: in Zigbee 3.0 this is the Application Profile (0x0104)
    request.setClusterId(m_clusterId);
    request.setSourceEndpoint(0x01);
    request.setDestinationEndpoint(m_endpoint->endpointId());
    request.setRadius(10);
    request.setTxOptions(Zigbee::ZigbeeTxOptions(Zigbee::ZigbeeTxOptionAckTransmission));
    return request;
}

bool ZigbeeCluster::verifyNetworkError(ZigbeeClusterReply *zclReply, ZigbeeNetworkReply *networkReply)
{
    bool success = false;
    switch (networkReply->error()) {
    case ZigbeeNetworkReply::ErrorNoError:
        // The request has been transported successfully to he destination, now
        // wait for the expected indication or check if we already recieved it
        zclReply->m_apsConfirmReceived = true;
        zclReply->m_zigbeeApsStatus = networkReply->zigbeeApsStatus();
        zclReply->m_zigbeeNwkStatus = networkReply->zigbeeNwkStatus();
        success = true;
        break;
    case ZigbeeNetworkReply::ErrorTimeout:
        zclReply->m_error = ZigbeeClusterReply::ErrorTimeout;
        break;
    case ZigbeeNetworkReply::ErrorInterfaceError:
        zclReply->m_error = ZigbeeClusterReply::ErrorInterfaceError;
        break;
    case ZigbeeNetworkReply::ErrorNetworkOffline:
        zclReply->m_error = ZigbeeClusterReply::ErrorNetworkOffline;
        break;
    case ZigbeeNetworkReply::ErrorZigbeeApsStatusError:
        zclReply->m_apsConfirmReceived = true;
        zclReply->m_error = ZigbeeClusterReply::ErrorZigbeeApsStatusError;
        zclReply->m_zigbeeApsStatus = networkReply->zigbeeApsStatus();
        break;
    case ZigbeeNetworkReply::ErrorZigbeeNwkStatusError:
        zclReply->m_apsConfirmReceived = true;
        zclReply->m_error = ZigbeeClusterReply::ErrorZigbeeNwkStatusError;
        zclReply->m_zigbeeNwkStatus = networkReply->zigbeeNwkStatus();
        break;
    }

    return success;
}

void ZigbeeCluster::finishZclReply(ZigbeeClusterReply *zclReply)
{
    m_pendingReplies.remove(zclReply->transactionSequenceNumber());
    qCDebug(dcZigbeeCluster()) << "ZigbeeClusterReply finished" << zclReply->request() << zclReply->requestFrame() << zclReply->responseFrame();
    zclReply->finished();
}

void ZigbeeCluster::processDataIndication(ZigbeeClusterLibrary::Frame frame)
{
    // Increase the tsn for continuouse id increasing on both sides
    m_transactionSequenceNumber = frame.header.transactionSequenceNumber;

    // Warn about the unhandled cluster indication, you can override this method in cluster implementations
    qCWarning(dcZigbeeCluster()) << "Unhandled ZCL indication in" << m_node << m_endpoint << this << frame;
}

void ZigbeeCluster::processApsDataIndication(const QByteArray &asdu, const ZigbeeClusterLibrary::Frame &frame)
{
    qCDebug(dcZigbeeCluster()) << "Received data indication" << this << frame;

    // Check if this indication is for a pending reply
    if (m_pendingReplies.contains(frame.header.transactionSequenceNumber)) {
        ZigbeeClusterReply *reply = m_pendingReplies.value(frame.header.transactionSequenceNumber);
        reply->m_responseData = asdu;
        reply->m_responseFrame = frame;
        reply->m_zclIndicationReceived = true;
        if (reply->isComplete())
            finishZclReply(reply);

        // Update the attributes internally if this was a read command
        if (frame.header.frameControl.frameType == ZigbeeClusterLibrary::FrameTypeGlobal) {
            ZigbeeClusterLibrary::Command globalCommand = static_cast<ZigbeeClusterLibrary::Command>(frame.header.command);
            if (globalCommand == ZigbeeClusterLibrary::CommandReadAttributesResponse) {
                // Update the attributes from the attribut status reports internally
                QList<ZigbeeClusterLibrary::ReadAttributeStatusRecord> attributeStatusRecords = ZigbeeClusterLibrary::parseAttributeStatusRecords(frame.payload);
                foreach (const ZigbeeClusterLibrary::ReadAttributeStatusRecord &attributeStatusRecord, attributeStatusRecords) {
                    qCDebug(dcZigbeeCluster()) << "Received read attribute status record" << this << attributeStatusRecord;
                    setAttribute(ZigbeeClusterAttribute(attributeStatusRecord.attributeId, attributeStatusRecord.dataType));
                }
            }
        }

        // Increase the tsn for continuouse id increasing on both sides
        m_transactionSequenceNumber = frame.header.transactionSequenceNumber;
        return;
    }

    // Check for server clusters and if this is an attribute report
    if (m_direction == Server && frame.header.frameControl.frameType == ZigbeeClusterLibrary::FrameTypeGlobal) {
        ZigbeeClusterLibrary::Command globalCommand = static_cast<ZigbeeClusterLibrary::Command>(frame.header.command);
        if (globalCommand == ZigbeeClusterLibrary::CommandReportAttributes) {
            // Read the attribute reports and update/set the attributes
            QDataStream stream(frame.payload);
            stream.setByteOrder(QDataStream::LittleEndian);
            while (!stream.atEnd()) {
                quint16 attributeId = 0; quint8 type = 0;
                stream >> attributeId >> type;
                ZigbeeDataType dataType = ZigbeeClusterLibrary::readDataType(&stream, static_cast<Zigbee::DataType>(type));
                qCDebug(dcZigbeeCluster()) << "Received attributes report" << this << frame;
                setAttribute(ZigbeeClusterAttribute(attributeId, dataType));
            }

            // Increase the tsn for continuouse id increasing on both sides
            m_transactionSequenceNumber = frame.header.transactionSequenceNumber;
            return;
        }
    }

    // Not for a reply or not an attribute report, let the cluster process this message internally
    processDataIndication(frame);
}

QDebug operator<<(QDebug debug, ZigbeeCluster *cluster)
{
    debug.nospace().noquote() << "ZigbeeCluster("
                              << ZigbeeUtils::convertUint16ToHexString(static_cast<quint16>(cluster->clusterId())) << ", "
                              << cluster->clusterName() << ", "
                              << cluster->direction()
                              << ")";
    return debug.space();
}

QDebug operator<<(QDebug debug, const ZigbeeClusterAttributeReport &attributeReport)
{
    debug.nospace().noquote() << "AttributeReport("
                              << attributeReport.clusterId << ", "
                              << attributeReport.attributeId << ", "
                              << attributeReport.attributeStatus << ", "
                              << attributeReport.dataType << ", "
                              << ZigbeeUtils::convertByteArrayToHexString(attributeReport.data)
                              << ")";

    return debug.space();
}

