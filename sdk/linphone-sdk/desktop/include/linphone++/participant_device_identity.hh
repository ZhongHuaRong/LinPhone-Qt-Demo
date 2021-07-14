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

#ifndef _LINPHONE_PARTICIPANT_DEVICE_IDENTITY_HH
#define _LINPHONE_PARTICIPANT_DEVICE_IDENTITY_HH

#include <string>
#include "object.hh"




struct _LinphoneParticipantDeviceIdentity;


namespace linphone {

	class Address;

	/**
	 * @brief This object is only used on server side for @ref ChatRoom with @ref
	 * ChatRoomBackend::FlexisipChat backend. 
	 *
	 */
	class ParticipantDeviceIdentity: public Object {
	
		public:

			ParticipantDeviceIdentity(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneParticipantDeviceIdentity *cPtr() {return (_LinphoneParticipantDeviceIdentity *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Get the address of the participant device. 
			 *
			 * @return the address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getAddress() const;
			
			/**
			 * @brief Get the capability descriptor (currently +org.linphone.specs value) for
			 * this participant device identity. 
			 *
			 * @return the capability descriptor string. 
			 */
			LINPHONECXX_PUBLIC std::string getCapabilityDescriptor() const;
			
			/**
			 * @brief Set the capability descriptor (currently +org.linphone.specs value) for
			 * this participant device identity. 
			 *
			 * @param capabilityDescriptor the capability descriptor string. 
			 */
			LINPHONECXX_PUBLIC void setCapabilityDescriptor(const std::string & capabilityDescriptor);
			
			
	};

};

#endif // _LINPHONE_PARTICIPANT_DEVICE_IDENTITY_HH
