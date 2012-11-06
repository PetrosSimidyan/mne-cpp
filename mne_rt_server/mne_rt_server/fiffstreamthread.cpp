//=============================================================================================================
/**
* @file     fiffstreamthread.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti H�m�l�inen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2012
*
* @section  LICENSE
*
* Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Contains the implementation of the FiffStreamThread Class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "fiffstreamthread.h"
#include "fiffstreamserver.h"


#include "mne_rt_commands.h"

#include "../../MNE/fiff/fiff_constants.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtNetwork>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MSERVER;
using namespace FIFFLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

FiffStreamThread::FiffStreamThread(quint8 id, int socketDescriptor, QObject *parent)
: QThread(parent)
, m_iDataClientId(id)
, m_sDataClientAlias(QString(""))
, socketDescriptor(socketDescriptor)
{
}


//*************************************************************************************************************

void FiffStreamThread::read_command(FiffStream& p_FiffStreamIn, qint32 size)
{
    if(size >= 4)
    {
        qint32 t_command;
        p_FiffStreamIn >> t_command;

        if(t_command == MNE_RT_SET_CLIENT_ALIAS)
        {
            quint32 v = size-4;
            char* buf = new char[v+1];
            p_FiffStreamIn.readRawData(buf, v);
            buf[v] = 0;
            m_sDataClientAlias = QString(buf);
            delete[] buf;
            printf("\tnew fiff stream client (ID %d) alias = '%s'\r\n\n", m_iDataClientId, m_sDataClientAlias.toUtf8().constData());
        }
        else
        {
            printf("\tfiff stream client (ID %d): unknown command\r\n\n", m_iDataClientId);
        }
    }
    else
    {
        printf("\tfiff stream client (ID %d): unknown command\r\n\n", m_iDataClientId);
    }
}


//*************************************************************************************************************

void FiffStreamThread::getAndSendMeasurementInfo(qint32 ID, FiffInfo* p_pFiffInfo)
{
    mutex.lock();
    FiffStream t_FiffStreamOut(&m_qSendBlock, QIODevice::WriteOnly);
    t_FiffStreamOut.setVersion(QDataStream::Qt_5_0);

    qint32 init_info[2];
    init_info[0] = MNE_RT_GET_CLIENT_LIST;
    init_info[1] = m_iDataClientId;

    t_FiffStreamOut.start_block(FIFFB_MNE_RT_CLIENT_LIST);
    mutex.unlock();
}


////*************************************************************************************************************

//void FiffStreamThread::readFiffStreamServerInstruction(quint8 id, quint8 instruction)
//{
//    qDebug() << "Received Instruction: ID " << id << "; Instruction " << instruction;
//}

//*************************************************************************************************************
//ToDo Move this to command thread
//void FiffStreamThread::write_client_list(QTcpSocket& p_qTcpSocket)
//{

//    QByteArray m_qSendBlock;

//    FiffStream t_FiffStreamOut(&m_qSendBlock, QIODevice::WriteOnly);
//    t_FiffStreamOut.setVersion(QDataStream::Qt_5_0);

//    qint32 init_info[2];
//    init_info[0] = MNE_RT_GET_CLIENT_LIST;
//    init_info[1] = m_iDataClientId;

//    t_FiffStreamOut.start_block(FIFFB_MNE_RT_CLIENT_LIST);

//    p_qTcpSocket.write(m_qSendBlock);
//    p_qTcpSocket.waitForBytesWritten();

//    m_qSendBlock.clear();
//}


//*************************************************************************************************************

void FiffStreamThread::run()
{
    FiffStreamServer* parentServer = qobject_cast<FiffStreamServer*>(this->parent());
//    connect(parentServer, &FiffStreamServer::sendFiffStreamThreadInstruction,
//            this, &FiffStreamThread::readFiffStreamServerInstruction);

    connect(parentServer, &FiffStreamServer::sendMeasInfo,
            this, &FiffStreamThread::getAndSendMeasurementInfo);



    QTcpSocket t_qTcpSocket;
    if (!t_qTcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(t_qTcpSocket.error());
        return;
    }
    else
    {
        printf("Fiff stream client (assigned ID %d) accepted from\n\tIP:\t%s\n\tPort:\t%d\n\n",
               m_iDataClientId,
               QHostAddress(t_qTcpSocket.peerAddress()).toString().toUtf8().constData(),
               t_qTcpSocket.peerPort());
    }



//    write_client_info(t_qTcpSocket);

//    QByteArray block;

//    FiffStream t_FiffStream(&block, QIODevice::WriteOnly);
//    t_FiffStream.setVersion(QDataStream::Qt_5_0);

//    qint32 init_info[2];
//    init_info[0] = MNE_RT_CLIENT_ID;
//    init_info[1] = m_iClientId;

//    t_FiffStream.write_int(FIFF_MNE_RT_COMMAND, init_info, 2);

//    t_qTcpSocket.write(block);
//    t_qTcpSocket.waitForBytesWritten();
//    t_qTcpSocket.flush();

//    block.clear();


//    tcpSocket.disconnectFromHost();
//    tcpSocket.waitForDisconnected();


    FiffStream t_FiffStreamIn(&t_qTcpSocket);
    t_FiffStreamIn.setVersion(QDataStream::Qt_5_0);

    while(true)
    {
        //
        // Write available data
        //
        if(m_qSendBlock.size() > 0)
        {
            qDebug() << "data available" << m_qSendBlock.size();
            mutex.lock();
            t_qTcpSocket.write(m_qSendBlock);
            t_qTcpSocket.waitForBytesWritten();
            m_qSendBlock.clear();
            mutex.unlock();
        }

//        write_client_info(t_qTcpSocket);

        //
        // Wait 10ms for incomming data, read and continue
        //
        t_qTcpSocket.waitForReadyRead(10);

        if (t_qTcpSocket.bytesAvailable() >= (int)sizeof(qint32)*4)
        {
//            qDebug() << "in Size before" << t_qTcpSocket.bytesAvailable();

            FiffTag* t_pTag = NULL;
            FiffTag::read_tag_info(&t_FiffStreamIn, t_pTag, false);

            while (true)
            {
                t_qTcpSocket.waitForReadyRead(10);
                if(t_qTcpSocket.bytesAvailable() >= t_pTag->size())
                    break;
            }

            if(t_pTag->kind == FIFF_MNE_RT_COMMAND)
            {
                read_command(t_FiffStreamIn, t_pTag->size());
            }


//            FiffTag::read_tag_data(&t_FiffStreamIn, t_pTag);

//            qDebug() << "in Size after" << t_qTcpSocket.bytesAvailable();

        }

//        usleep(1000);
    }
}