/*
Copyright (C) 2017 Belledonne Communications SARL

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _LINPHONE_PARTICIPANT_DEVICE_HH
#define _LINPHONE_PARTICIPANT_DEVICE_HH

#include <string>
#include "enums.hh"
#include "object.hh"




struct _LinphoneParticipantDevice;


namespace linphone {

	class Address;

	/**
	 * @brief This object represents a unique device for a member of a @ref Conference
	 * or @ref ChatRoom. 
	 *
	 * Devices are identified by the gruu parameter inside the @ref Address which can
	 * be obtained by getAddress(). It is specially usefull to know the security level
	 * of each device inside an end-to-end encrypted @ref ChatRoom.
	 * You can get a list of all @ref ParticipantDevice using
	 * Participant::getDevices(). 
	 */
	class ParticipantDevice: public Object {
	
		public:

			ParticipantDevice(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneParticipantDevice *cPtr() {return (_LinphoneParticipantDevice *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Get the address of a participant's device. 
			 *
			 * @return The @ref Address of the participant's device   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getAddress() const;
			
			/**
			 * @brief Return whether the participant device is in a conference or not. 
			 *
			 * @return a boolean to state whether the device is in a conference 
			 */
			LINPHONECXX_PUBLIC bool isInConference() const;
			
			/**
			 * @brief Return the name of the device or nullptr. 
			 *
			 * @return the name of the device or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::string getName() const;
			
			/**
			 * @brief Get the security level of a participant's device. 
			 *
			 * @return The @ref ChatRoomSecurityLevel of the device 
			 */
			LINPHONECXX_PUBLIC linphone::ChatRoomSecurityLevel getSecurityLevel() const;
			
			/**
			 * @brief Get the timestamp the device joined a conference. 
			 *
			 * @return time of joining a conference as returned by time(nullptr). For UNIX
			 * based systems it is the number of seconds since 00:00hours of the 1st of
			 * January 1970 
			 */
			LINPHONECXX_PUBLIC time_t getTimeOfJoining() const;
			
			
	};

};

#endif // _LINPHONE_PARTICIPANT_DEVICE_HH
