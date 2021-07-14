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

#ifndef _LINPHONE_PARTICIPANT_HH
#define _LINPHONE_PARTICIPANT_HH

#include <list>
#include "enums.hh"
#include "object.hh"




struct _LinphoneParticipant;


namespace linphone {

	class Address;
	class ParticipantDevice;

	/**
	 * @brief Identifies a member of a @ref Conference or @ref ChatRoom. 
	 *
	 * A participant is identified by it's SIP address. It can have many @ref
	 * ParticipantDevice. 
	 */
	class Participant: public Object {
	
		public:

			Participant(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneParticipant *cPtr() {return (_LinphoneParticipant *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Get the address of a conference participant. 
			 *
			 * @return The @ref Address of the participant   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getAddress() const;
			
			/**
			 * @brief Get the timestamp of the creation of the participant. 
			 *
			 * @return time of creation of the participant as returned by time(nullptr). For
			 * UNIX based systems it is the number of seconds since 00:00hours of the 1st of
			 * January 1970 
			 */
			LINPHONECXX_PUBLIC time_t getCreationTime() const;
			
			/**
			 * @brief Gets the list of devices from a chat room's participant. 
			 *
			 * @return List of devices.      
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::ParticipantDevice>> getDevices() const;
			
			/**
			 * @brief Tells whether a conference participant is an administrator of the
			 * conference. 
			 *
			 * @return A boolean value telling whether the participant is an administrator 
			 */
			LINPHONECXX_PUBLIC bool isAdmin() const;
			
			/**
			 * @brief Tells whether a conference participant is the focus of the conference. 
			 *
			 * @return A boolean value telling whether the participant is a focus of a
			 * conference 
			 */
			LINPHONECXX_PUBLIC bool isFocus() const;
			
			/**
			 * @brief Get the security level of a participant. 
			 *
			 * @return The @ref ChatRoomSecurityLevel of the participant 
			 */
			LINPHONECXX_PUBLIC linphone::ChatRoomSecurityLevel getSecurityLevel() const;
			
			/**
			 * @brief Find a device in the list of devices from a chat room's participant. 
			 *
			 * @param address A @ref Address object   
			 * @return a @ref ParticipantDevice or nullptr if not found.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ParticipantDevice> findDevice(const std::shared_ptr<const linphone::Address> & address) const;
			
			
	};

};

#endif // _LINPHONE_PARTICIPANT_HH
