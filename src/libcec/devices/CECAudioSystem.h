#pragma once
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
#include "CECBusDevice.h"

namespace CEC
{
  class CCECAudioSystem : public CCECBusDevice
  {
  public:
    CCECAudioSystem(CCECProcessor *processor, cec_logical_address address, uint16_t iPhysicalAddress = CEC_INVALID_PHYSICAL_ADDRESS);
    virtual ~CCECAudioSystem(void) {};

    bool SetAudioStatus(uint8_t status);
    bool SetSystemAudioModeStatus(const cec_system_audio_status mode);
    
    //HIB
    bool SetArcStatus(const cec_arc_status status);
    
    //HIB
    bool SetShortAudioDescriptor(uint32_t status);

    bool TransmitAudioStatus(cec_logical_address dest, bool bIsReply);
    bool TransmitSetSystemAudioMode(cec_logical_address dest, bool bIsReply);
    bool TransmitSystemAudioModeStatus(cec_logical_address dest, bool bIsReply);
    bool TransmitArcStartEnd(const cec_logical_address destination, int startOrEnd);
    
    //HIB
    cec_system_audio_status GetSystemAudioModeStatus(const cec_logical_address initiator, bool bUpdate);
    //HIB
    bool RequestSystemAudioModeStatus(const cec_logical_address initiator, bool bWaitForResponse);

    //HIB
    uint32_t TestRequestAudioDescriptor(const cec_logical_address initiator, uint8_t iAudioFormatIdCode, bool bUpdate);
    //HIB
    bool RequestTestRequestAudioDescriptor(const cec_logical_address initiator, uint8_t iAudioFormatIdCode, bool bWaitForResponse);

    //HIB
    cec_system_audio_status TestSystemAudioModeRequest(const cec_logical_address initiator, uint16_t iPhysicalAddress, bool bAddPhysicalAddress, bool bUpdate);
    //HIB
    bool RequestTestSystemAudioModeRequest(const cec_logical_address initiator, uint16_t iPhysicalAddress, bool bAddPhysicalAddress, bool bWaitForResponse);

    //HIB
    cec_arc_status TestRequestArcTermination(const cec_logical_address initiator, bool bUpdate);
    //HIB
    bool RequestTestRequestArcTermination(const cec_logical_address initiator, bool bWaitForResponse);

    //HIB
    cec_arc_status TestRequestArcInitiation(const cec_logical_address initiator, bool bUpdate);
    //HIB
    bool RequestTestRequestArcInitiation(const cec_logical_address initiator, bool bWaitForResponse);

    //HIB
    bool TestReportArcTerminated(const cec_logical_address initiator, bool bUpdate);
    //HIB
    bool RequestTestReportArcTerminated(const cec_logical_address initiator, bool bWaitForResponse);

    //HIB
    bool TestReportArcInitiated(const cec_logical_address initiator, bool bUpdate);
    //HIB
    bool RequestTestReportArcInitiated(const cec_logical_address initiator, bool bWaitForResponse);

    //HIB
    cec_arc_status TestRequestArcInitiationWrongParam(const cec_logical_address initiator, uint16_t iWrongParam, bool bUpdate);
    //*HIB
    bool RequestTestRequestArcInitiationWrongParam(const cec_logical_address initiator, uint16_t iWrongParam, bool bWaitForResponse);

    uint8_t VolumeUp(const cec_logical_address source, bool bSendRelease = true);
    uint8_t VolumeDown(const cec_logical_address source, bool bSendRelease = true);
    uint8_t MuteAudio(const cec_logical_address source);
    uint8_t GetAudioStatus(const cec_logical_address initiator, bool bUpdate = false);
    bool EnableAudio(CCECBusDevice* device = nullptr);

    bool TransmitActiveSource(void) { return false; }

  protected:
    bool RequestAudioStatus(const cec_logical_address initiator, bool bWaitForResponse = true);
    

    cec_system_audio_status m_systemAudioStatus;
    cec_arc_status          m_arcStatus; 
    uint8_t                 m_audioStatus;
    uint32_t                m_audioDescriptorStatus;

    bool arcStarted;
  };
}
