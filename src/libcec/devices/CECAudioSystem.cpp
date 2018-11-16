/*
 * This file is part of the libCEC(R) library.
 *
 * libCEC(R) is Copyright (C) 2011-2015 Pulse-Eight Limited.  All rights reserved.
 * libCEC(R) is an original work, containing original code.
 *
 * libCEC(R) is a trademark of Pulse-Eight Limited.
 *
 * This program is dual-licensed; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 *
 * Alternatively, you can license this library under a commercial license,
 * please contact Pulse-Eight Licensing for more information.
 *
 * For more information contact:
 * Pulse-Eight Licensing       <license@pulse-eight.com>
 *     http://www.pulse-eight.com/
 *     http://www.pulse-eight.net/
 */

#include "env.h"
#include "CECAudioSystem.h"

#include "CECProcessor.h"
#include "implementations/CECCommandHandler.h"
#include "LibCEC.h"
#include "CECTypeUtils.h"

using namespace CEC;
using namespace P8PLATFORM;

#define LIB_CEC     m_processor->GetLib()
#define ToString(p) CCECTypeUtils::ToString(p)

CCECAudioSystem::CCECAudioSystem(CCECProcessor *processor, cec_logical_address address, uint16_t iPhysicalAddress /* = CEC_INVALID_PHYSICAL_ADDRESS */) :
    CCECBusDevice(processor, address, iPhysicalAddress),
    m_systemAudioStatus(CEC_SYSTEM_AUDIO_STATUS_ON),
    m_audioStatus(CEC_AUDIO_VOLUME_STATUS_UNKNOWN)
{
  m_type = CEC_DEVICE_TYPE_AUDIO_SYSTEM;
}

bool CCECAudioSystem::SetAudioStatus(uint8_t status)
{
  CLockObject lock(m_mutex);
  //printf(">>>> arrived A20.A <<<< m_audioStatus = %i \n", m_audioStatus);
  //printf(">>>> arrived A20.B <<<< mode = %i \n", status);
  if (m_audioStatus != status)
  {
    LIB_CEC->AddLog(CEC_LOG_DEBUG, ">> %s (%X): audio status changed from %2x to %2x", GetLogicalAddressName(), m_iLogicalAddress, m_audioStatus, status);
    m_audioStatus = status;
    
    //printf(">>>> arrived A20 <<<< m_audioStatus = %i \n", m_audioStatus);
    return true;
  }

  return false;
}

bool CCECAudioSystem::SetSystemAudioModeStatus(const cec_system_audio_status mode)
{
  CLockObject lock(m_mutex);

  //printf(">>>> arrived 20.A <<<< m_systemAudioStatus = %i \n", m_systemAudioStatus);
  //printf(">>>> arrived 20.B <<<< mode = %i \n", mode);

  if (m_systemAudioStatus != mode)
  {
    LIB_CEC->AddLog(CEC_LOG_DEBUG, ">> %s (%X): system audio mode status changed from %s to %s", GetLogicalAddressName(), m_iLogicalAddress, ToString(m_systemAudioStatus), ToString(mode));
    m_systemAudioStatus = mode;

    //printf(">>>> arrived 20 <<<< m_systemAudioStatus = %i \n", m_systemAudioStatus);

    //printf(">>>> arrived 20.C <<<< m_systemAudioStatus = %i \n", m_systemAudioStatus);
    return true;
  }

  return false;
}


//HIB
bool CCECAudioSystem::SetShortAudioDescriptor(uint32_t status)
{
  CLockObject lock(m_mutex);
  //printf(">>>> arrived CCECAudioSystem::SetShortAudioDescriptor <<<< m_audioDescriptorStatus = %i \n", m_audioDescriptorStatus);
  //printf(">>>> arrived CCECAudioSystem::SetShortAudioDescriptor <<<< New value = %i \n", status);
  if (m_audioDescriptorStatus != status)
  {
    LIB_CEC->AddLog(CEC_LOG_DEBUG, ">> %s (%X): audio status changed from %2x to %2x", GetLogicalAddressName(), m_iLogicalAddress, m_audioStatus, status);
    m_audioDescriptorStatus = status;
    
    //printf(">>>> arrived CCECAudioSystem::SetShortAudioDescriptor <<<< m_audioDescriptorStatus = %i \n", m_audioDescriptorStatus);
    return true;
  }

  return false;
}


//HIB
//bool CCECAudioSystem::SetShortAudioDescriptorUS(uint32_t status)
//{
//  CLockObject lock(m_mutex);
//  //printf(">>>> arrived CCECAudioSystem::SetShortAudioDescriptor <<<< m_audioDescriptorStatus = %i \n", m_audioDescriptorStatus);
//  //printf(">>>> arrived CCECAudioSystem::SetShortAudioDescriptor <<<< New value = %i \n", status);
//  if (m_audioDescriptorStatusUS != status)
//  {
//    LIB_CEC->AddLog(CEC_LOG_DEBUG, ">> %s (%X): audio status changed from %2x to %2x", GetLogicalAddressName(), m_iLogicalAddress, m_audioStatus, status);
//    m_audioDescriptorStatusUS = status;
//    
//    //printf(">>>> arrived CCECAudioSystem::SetShortAudioDescriptor <<<< m_audioDescriptorStatus = %i \n", m_audioDescriptorStatusUS);
//    return true;
//  }
//
//  return false;
//}


//HIB
bool CCECAudioSystem::SetArcStatus(const cec_arc_status status)
{
  CLockObject lock(m_mutex);

  //printf(">>>> arrived C20.A <<<< m_arcStatusStatus = %i \n", m_arcStatus);
  //printf(">>>> arrived C20.B <<<< mode = %i \n", status);

  if (m_arcStatus != status)
  {
    LIB_CEC->AddLog(CEC_LOG_DEBUG, ">> %s (%X): arc status changed from %s to %s", GetLogicalAddressName(), m_iLogicalAddress, ToString(m_arcStatus), ToString(status));
    m_arcStatus = status;

    //printf(">>>> arrived C20 <<<< m_arcStatus = %i \n", m_arcStatus);

    //printf(">>>> arrived C20.C <<<< m_arcStatus = %i \n", m_arcStatus);
    return true;
  }

  return false;
}


bool CCECAudioSystem::TransmitAudioStatus(cec_logical_address dest, bool bIsReply)
{
  uint8_t state;
  {
    CLockObject lock(m_mutex);
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< %x -> %x: audio status '%2x'", m_iLogicalAddress, dest, m_audioStatus);
    state = m_audioStatus;
  }

  return m_handler->TransmitAudioStatus(m_iLogicalAddress, dest, state, bIsReply);
}

bool CCECAudioSystem::TransmitSetSystemAudioMode(cec_logical_address dest, bool bIsReply)
{
  cec_system_audio_status state;
  {
    CLockObject lock(m_mutex);
    if (arcStarted)
    {
      m_audioStatus = 0x50; //Set volume to 50, otherwise TV might detect that we are not actually playing
      //printf(">>>> arrived 21 <<<< \n");
      m_systemAudioStatus = CEC_SYSTEM_AUDIO_STATUS_ON;
    }
    else
    {
      //printf(">>>> arrived 22 <<<< \n");
      m_systemAudioStatus = CEC_SYSTEM_AUDIO_STATUS_OFF;
    }
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< %x -> %x: set system audio mode '%2x'", m_iLogicalAddress, dest, m_audioStatus);
    state = m_systemAudioStatus;
  }

  return m_handler->TransmitSetSystemAudioMode(m_iLogicalAddress, dest, state, bIsReply);
}

bool CCECAudioSystem::TransmitSystemAudioModeStatus(cec_logical_address dest, bool bIsReply)
{
  cec_system_audio_status state;
  {
    CLockObject lock(m_mutex);

    if (arcStarted)
    {
      //printf(">>>> arrived 23 <<<< \n");
      m_systemAudioStatus = CEC_SYSTEM_AUDIO_STATUS_ON;
    }
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< %x -> %x: system audio mode '%s'", m_iLogicalAddress, dest, ToString(m_systemAudioStatus));
    state = m_systemAudioStatus;
  }

  return m_handler->TransmitSystemAudioModeStatus(m_iLogicalAddress, dest, state, bIsReply);
}

bool CCECAudioSystem::TransmitArcStartEnd(const cec_logical_address destination, int startOrEnd)
{
	bool bReturn(false);
  //printf(">>>> arrived C24 <<<< %i \n",startOrEnd);
	LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< %s (%X) -> %s (%X): TransmitArcStart param(%d)", GetLogicalAddressName(), m_iLogicalAddress, ToString(destination), destination, startOrEnd);
	MarkBusy();
	arcStarted = (bool)startOrEnd;
	bReturn = m_handler->TransmitArcStartEnd(m_iLogicalAddress, destination, true, startOrEnd);
	MarkReady();

	return bReturn;
}

uint8_t CCECAudioSystem::VolumeUp(const cec_logical_address source, bool bSendRelease /* = true */)
{
  TransmitKeypress(source, CEC_USER_CONTROL_CODE_VOLUME_UP);
  if (bSendRelease)
    TransmitKeyRelease(source);

  CLockObject lock(m_mutex);
  return m_audioStatus;
}

uint8_t CCECAudioSystem::VolumeDown(const cec_logical_address source, bool bSendRelease /* = true */)
{
  TransmitKeypress(source, CEC_USER_CONTROL_CODE_VOLUME_DOWN);
  if (bSendRelease)
    TransmitKeyRelease(source);

  CLockObject lock(m_mutex);
  return m_audioStatus;
}

uint8_t CCECAudioSystem::MuteAudio(const cec_logical_address source)
{
  TransmitKeypress(source, CEC_USER_CONTROL_CODE_MUTE);
  TransmitKeyRelease(source);

  return GetAudioStatus(source, true);
}

bool CCECAudioSystem::RequestAudioStatus(const cec_logical_address initiator, bool bWaitForResponse /* = true */)
{
  bool bReturn(false);

  if (!IsHandledByLibCEC() &&
      !IsUnsupportedFeature(CEC_OPCODE_GIVE_AUDIO_STATUS))
  {
    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< requesting audio status of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestAudioStatus(initiator, m_iLogicalAddress, bWaitForResponse);
    MarkReady();
  }
  return bReturn;
}

uint8_t CCECAudioSystem::GetAudioStatus(const cec_logical_address initiator, bool bUpdate /* = false */)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = bIsPresent &&
        (bUpdate || m_audioStatus == CEC_AUDIO_VOLUME_STATUS_UNKNOWN);
  }

  //if (bRequestUpdate)
  //{
    CheckVendorIdRequested(initiator);
    RequestAudioStatus(initiator);
  //}

  CLockObject lock(m_mutex);
  return m_audioStatus;
}

bool CCECAudioSystem::EnableAudio(CCECBusDevice* device /* = nullptr */)
{
  //printf(">>>> arrived B222 <<<< \n");
  uint16_t audioAddress = !!device ?
      device->GetCurrentPhysicalAddress() :
      CEC_INVALID_PHYSICAL_ADDRESS;
  return m_handler->TransmitSystemAudioModeRequest(m_iLogicalAddress, audioAddress);
}

//HIB
cec_system_audio_status CCECAudioSystem::GetSystemAudioModeStatus(const cec_logical_address initiator, bool bUpdate /* = false */)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
        (bUpdate || m_systemAudioStatus == CEC_SYSTEM_AUDIO_STATUS_ON ||
                    m_systemAudioStatus == CEC_SYSTEM_AUDIO_STATUS_OFF ||
                    m_systemAudioStatus == CEC_SYSTEM_AUDIO_STATUS_UNKNOWN));
  }
  
  m_systemAudioStatus = CEC_SYSTEM_AUDIO_STATUS_UNKNOWN;

  //if (bRequestUpdate)
  //{  
    //printf(">>>> arrived 2 <<<< m_systemAudioStatus: %i \n", m_systemAudioStatus);
    RequestSystemAudioModeStatus(initiator,true);
  //}

  //printf(">>>> arrived 2.1 <<<< m_systemAudioStatus: %i \n", m_systemAudioStatus);
  
  CLockObject lock(m_mutex);
  return m_systemAudioStatus;
}


//HIB
bool CCECAudioSystem::RequestSystemAudioModeStatus(const cec_logical_address initiator, bool bWaitForResponse /* = true */)
{
  bool bReturn(false);
  //printf(">>>> arrived 3 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> arrived 3.1 <<<< \n");
    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< requesting system audio mode status of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestSystemAudioModeStatus(initiator, m_iLogicalAddress, bWaitForResponse);
    MarkReady();

  }
  //printf(">>>> arrived 3.2 <<<< return val: %i \n", bReturn);

  return bReturn;
}

// audio descriptor start
//HIB
uint32_t CCECAudioSystem::TestRequestAudioDescriptor(const cec_logical_address initiator, uint8_t iAudioFormatIdCode, bool bUpdate /* = false */)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
    (bUpdate )); //|| m_systemAudioStatus == CEC_AUDIO_STATUS_ON ||
                 //   m_systemAudioStatus == CEC_AUDIO_STATUS_OFF ||
                 //   m_systemAudioStatus == CEC_AUDIO_STATUS_UNKNOWN));
  }
  
  m_audioDescriptorStatus = 0xFF000000;

  //printf(">>>> #4 arrived CCECAudioSystem::TestRequestAudioDescriptor #1 <<<< \n");
  //printf(">>>> initiator         : %i \n", initiator);
  //printf(">>>> iAudioFormatIdCode : %i \n",iAudioFormatIdCode);
  //printf(">>>> ------------------------------------------------------- \n");

  //if (bRequestUpdate)
  //{  
    //printf(">>>> #5 arrived CCECBusDevice::TestRequestAudioDescriptor #2 m_audioDescriptorStatus: %i <<<< \n", m_audioDescriptorStatus);
    RequestTestRequestAudioDescriptor(initiator,iAudioFormatIdCode,true);
    //printf(">>>> #? arrived CCECBusDevice::TestRequestAudioDescriptor #3 m_audioDescriptorStatus: %i <<<< \n", m_audioDescriptorStatus);
  //}

  CLockObject lock(m_mutex);
  return m_audioDescriptorStatus;
}


//HIB
bool CCECAudioSystem::RequestTestRequestAudioDescriptor(const cec_logical_address initiator, uint8_t iAudioFormatIdCode, bool bWaitForResponse /* = true */)
{
  bool bReturn(false);

  //printf(">>>> #6 arrived CCECAudioSystem::RequestTestRequestAudioDescriptor #1 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> #7 arrived CCECAudioSystem::RequestTestRequestAudioDescriptor #2 <<<< \n");
    //printf(">>>> initiator         : %i \n", initiator);
    //printf(">>>> m_iLogicalAddress : %i \n",m_iLogicalAddress);
    //printf(">>>> iAudioFormatIdCode  : %i \n",iAudioFormatIdCode);
    //printf(">>>> bWaitForResponse  : %i \n",bWaitForResponse);
    //printf(">>>> ------------------------------------------------------- \n");

    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< requesting system audio mode status of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestTestRequestAudioDescriptor(initiator, m_iLogicalAddress, iAudioFormatIdCode, bWaitForResponse);
    MarkReady();

  }
  //printf(">>>> #? arrived CCECAudioSystem::RequestTestRequestAudioDescriptor #3 <<<< %i \n", bReturn);

  return bReturn;
}
// audio descriptor end

//HIB
cec_system_audio_status CCECAudioSystem::TestSystemAudioModeRequest(const cec_logical_address initiator, uint16_t iPhysicalAddress, bool bAddPhysicalAddress, bool bUpdate /* = false */)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
        (bUpdate || m_systemAudioStatus == CEC_SYSTEM_AUDIO_STATUS_ON ||
                    m_systemAudioStatus == CEC_SYSTEM_AUDIO_STATUS_OFF ||
                    m_systemAudioStatus == CEC_SYSTEM_AUDIO_STATUS_UNKNOWN));
  }
  
  m_systemAudioStatus = CEC_SYSTEM_AUDIO_STATUS_UNKNOWN;

    //printf(">>>> arrived CCECBusDevice::TestSystemAudioModeRequest #2 <<<< \n");
    //printf(">>>> initiator         : %i \n", initiator);
    //printf(">>>> m_iLogicalAddress : %i \n",m_iLogicalAddress);
    //printf(">>>> iPhysicalAddress  : %i \n",iPhysicalAddress);
    //printf(">>>> AddPhysicalAddress: %i \n",bAddPhysicalAddress);
    //printf(">>>> ------------------------------------------------------- \n");

  //if (bRequestUpdate)
  //{  
    //printf(">>>> arrived CCECBusDevice::TestSystemAudioModeRequest #2 m_systemAudioStatus: %i <<<< \n", m_systemAudioStatus);
    RequestTestSystemAudioModeRequest(initiator,iPhysicalAddress,bAddPhysicalAddress,true);
    //printf(">>>> arrived CCECBusDevice::TestSystemAudioModeRequest #3 m_systemAudioStatus: %i <<<< \n", m_systemAudioStatus);
  //}

    
  CLockObject lock(m_mutex);
  return m_systemAudioStatus;
}


//HIB
bool CCECAudioSystem::RequestTestSystemAudioModeRequest(const cec_logical_address initiator, uint16_t iPhysicalAddress, bool bAddPhysicalAddress,bool bWaitForResponse /* = true */)
{
  bool bReturn(false);
  //printf(">>>> arrived B3 <<<< initiator: %i \n", initiator);
  //printf(">>>> arrived CCECBusDevice::RequestTestSystemAudioModeRequest #1 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> arrived CCECBusDevice::RequestTestSystemAudioModeRequest #2 <<<< \n");
    //printf(">>>> initiator         : %i \n", initiator);
    //printf(">>>> m_iLogicalAddress : %i \n",m_iLogicalAddress);
    //printf(">>>> iPhysicalAddress  : %i \n",iPhysicalAddress);
    //printf(">>>> AddPhysicalAddress: %i \n",bAddPhysicalAddress);
    //printf(">>>> ------------------------------------------------------- \n");

    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< requesting system audio mode status of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestTestSystemAudioModeRequest(initiator, m_iLogicalAddress, iPhysicalAddress, bAddPhysicalAddress,bWaitForResponse);
    MarkReady();

  }
  //printf(">>>> arrived CCECBusDevice::RequestTestSystemAudioModeRequest #3 <<<< \n");

  return bReturn;
}


//HIB
cec_arc_status CCECAudioSystem::TestRequestArcTermination(const cec_logical_address initiator, bool bUpdate /* = false */)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
        (bUpdate || m_arcStatus == CEC_ARC_STATUS_ON ||
                    m_arcStatus == CEC_ARC_STATUS_OFF ||
                    m_arcStatus == CEC_ARC_STATUS_UNKNOWN));
  }
  
  m_arcStatus = CEC_ARC_STATUS_UNKNOWN;

  //if (bRequestUpdate)
  //{  
    //printf(">>>> arrived C2 <<<< \n");
    RequestTestRequestArcTermination(initiator,true);
  //}

  //printf(">>>> arrived C2.1 <<<< \n");
  
  CLockObject lock(m_mutex);
  return m_arcStatus;
}

//HIB
bool CCECAudioSystem::RequestTestRequestArcTermination(const cec_logical_address initiator, bool bWaitForResponse /* = true */)
{
  bool bReturn(false);
  //printf(">>>> arrived C3 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> arrived C3.1 <<<< \n");
    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< requesting Arc Termination of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestTestRequestArcTermination(initiator, m_iLogicalAddress, bWaitForResponse);
    MarkReady();

  }
  //printf(">>>> arrived C3.2 <<<< return val: %i \n", bReturn);

  return bReturn;
}


//HIB
cec_arc_status CCECAudioSystem::TestRequestArcInitiation(const cec_logical_address initiator, bool bUpdate /* = false */)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
        (bUpdate || m_arcStatus == CEC_ARC_STATUS_ON ||
                    m_arcStatus == CEC_ARC_STATUS_OFF ||
                    m_arcStatus == CEC_ARC_STATUS_UNKNOWN));
  }
  
  m_arcStatus = CEC_ARC_STATUS_UNKNOWN;

  //if (bRequestUpdate)
  //{  
    //printf(">>>> arrived D2 <<<< %i \n", m_arcStatus);
    RequestTestRequestArcInitiation(initiator,true);
  //}

  //printf(">>>> arrived D2.1 <<<< %i \n", m_arcStatus);
  
  CLockObject lock(m_mutex);
  return m_arcStatus;
}

//HIB
bool CCECAudioSystem::RequestTestRequestArcInitiation(const cec_logical_address initiator, bool bWaitForResponse /* = true */)
{
  bool bReturn(false);
  //printf(">>>> arrived D3 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> arrived D3.1 <<<< \n");
    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< requesting arc Initiation of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestTestRequestArcInitiation(initiator, m_iLogicalAddress, bWaitForResponse);
    MarkReady();

  }
  //printf(">>>> arrived D3.2 <<<< return val: %i \n", bReturn);

  return bReturn;
}

//HIB
bool CCECAudioSystem::TestReportArcTerminated(const cec_logical_address initiator, bool bUpdate)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
        (bUpdate || m_arcStatus == CEC_ARC_STATUS_ON ||
                    m_arcStatus == CEC_ARC_STATUS_OFF ||
                    m_arcStatus == CEC_ARC_STATUS_UNKNOWN));
  }
  
  //printf(">>>> arrived CCECAudioSystem::TestReportArcTerminated #1 <<<< \n");
  return RequestTestReportArcTerminated(initiator,true);
  //printf(">>>> arrived CCECAudioSystem::TestReportArcTerminated #2 <<<< \n");
  
  CLockObject lock(m_mutex);
  return false;
}

//HIB
bool CCECAudioSystem::RequestTestReportArcTerminated(const cec_logical_address initiator, bool bWaitForResponse)
{
  bool bReturn(false);
  //printf(">>>> arrived CCECAudioSystem::RequestTestReportArcTerminated #1 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> arrived CCECAudioSystem::RequestTestReportArcTerminated #2 <<<< \n");
    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< reporting Arc Termination of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestTestReportArcTerminated(initiator, m_iLogicalAddress, bWaitForResponse);
    MarkReady();

  }
  //printf(">>>> arrived CCECAudioSystem::RequestTestReportArcTerminated #3 <<<< \n");
  return bReturn;
}

//HIB
bool CCECAudioSystem::TestReportArcInitiated(const cec_logical_address initiator, bool bUpdate)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
        (bUpdate || m_arcStatus == CEC_ARC_STATUS_ON ||
                    m_arcStatus == CEC_ARC_STATUS_OFF ||
                    m_arcStatus == CEC_ARC_STATUS_UNKNOWN));
  }

  //printf(">>>> arrived CCECAudioSystem::TestReportArcInitiated #1 <<<< \n");
  return RequestTestReportArcInitiated(initiator,true);
  //printf(">>>> arrived CCECAudioSystem::TestReportArcInitiated #2 <<<< \n");
  
  CLockObject lock(m_mutex);
  return false;
}

//HIB
bool CCECAudioSystem::RequestTestReportArcInitiated(const cec_logical_address initiator, bool bWaitForResponse)
{
  bool bReturn(false);
  //printf(">>>> arrived CCECAudioSystem::RequestTestReportArcInitiated #1 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> arrived CCECAudioSystem::RequestTestReportArcInitiated #2 <<<< \n");
    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< reporting Arc Initiated of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestTestReportArcInitiated(initiator, m_iLogicalAddress, bWaitForResponse);
    MarkReady();
  }
  //printf(">>>> arrived CCECAudioSystem::RequestTestReportArcInitiated #3 <<<< \n");
  return bReturn;
}

//HIB
cec_arc_status CCECAudioSystem::TestRequestArcInitiationWrongParam(const cec_logical_address initiator, uint16_t iWrongParam, bool bUpdate /* = false */)
{
  bool bIsPresent(GetStatus() == CEC_DEVICE_STATUS_PRESENT);
  bool bRequestUpdate(false);
  {
    CLockObject lock(m_mutex);
    bRequestUpdate = (bIsPresent &&
        (bUpdate || m_arcStatus == CEC_ARC_STATUS_ON ||
                    m_arcStatus == CEC_ARC_STATUS_OFF ||
                    m_arcStatus == CEC_ARC_STATUS_UNKNOWN));
  }
  
  m_arcStatus = CEC_ARC_STATUS_UNKNOWN;

  //if (bRequestUpdate)
  //{  
    //printf(">>>> arrived D2 <<<< %i \n", m_arcStatus);
    RequestTestRequestArcInitiationWrongParam(initiator, iWrongParam,true);
  //}

  //printf(">>>> arrived D2.1 <<<< %i \n", m_arcStatus);
  
  CLockObject lock(m_mutex);
  return m_arcStatus;
}

//HIB
bool CCECAudioSystem::RequestTestRequestArcInitiationWrongParam(const cec_logical_address initiator, uint16_t iWrongParam, bool bWaitForResponse /* = true */)
{
  bool bReturn(false);
  //printf(">>>> arrived D3 <<<< \n");
  if (!IsHandledByLibCEC() && initiator != CECDEVICE_UNKNOWN)
  {
    //printf(">>>> arrived D3.1 <<<< \n");
    MarkBusy();
    LIB_CEC->AddLog(CEC_LOG_DEBUG, "<< requesting arc Initiation of '%s' (%X)", GetLogicalAddressName(), m_iLogicalAddress);
    bReturn = m_handler->TransmitRequestTestRequestArcInitiationWrongParam(initiator, m_iLogicalAddress, iWrongParam, bWaitForResponse);
    MarkReady();

  }
  //printf(">>>> arrived D3.2 <<<< return val: %i \n", bReturn);

  return bReturn;
}

