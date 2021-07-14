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

#ifndef _LINPHONE_CONFERENCE_LISTENER_HH
#define _LINPHONE_CONFERENCE_LISTENER_HH

#include <string>
#include "conference.hh"
#include "object.hh"






namespace linphone {

	class Conference;
	class Participant;
	class ParticipantDevice;

	/**
	 *
	 */
	class ConferenceListener: public Listener {
	
		public:


	
		public:
			
		
			/**
			 * @brief Callback used to notify a chat room that a participant has been added. 
			 *
			 * @param participant LinphoneParticipant that has been added to the conference   
			 */
			LINPHONECXX_PUBLIC virtual void onParticipantAdded(const std::shared_ptr<linphone::Conference> & conference, const std::shared_ptr<const linphone::Participant> & participant) {};
			
			/**
			 * @brief Callback used to notify a chat room that a participant has been removed. 
			 *
			 * @param participant LinphoneParticipant that has been removed to the conference 
			 *  
			 */
			LINPHONECXX_PUBLIC virtual void onParticipantRemoved(const std::shared_ptr<linphone::Conference> & conference, const std::shared_ptr<const linphone::Participant> & participant) {};
			
			/**
			 * @brief Callback used to notify a chat room that a participant has been added. 
			 *
			 * @param participantDevice LinphoneParticipantDevice that has been added to the
			 * conference   
			 */
			LINPHONECXX_PUBLIC virtual void onParticipantDeviceAdded(const std::shared_ptr<linphone::Conference> & conference, const std::shared_ptr<const linphone::ParticipantDevice> & participantDevice) {};
			
			/**
			 * @brief Callback used to notify a chat room that a participant has been removed. 
			 *
			 * @param participantDevice LinphoneParticipantDevice that has been removed to the
			 * conference   
			 */
			LINPHONECXX_PUBLIC virtual void onParticipantDeviceRemoved(const std::shared_ptr<linphone::Conference> & conference, const std::shared_ptr<const linphone::ParticipantDevice> & participantDevice) {};
			
			/**
			 * @brief Callback used to notify a chat room that the admin status of a
			 * participant has been changed. 
			 *
			 * @param participant LinphoneParticipant whose admin status has changed   
			 */
			LINPHONECXX_PUBLIC virtual void onParticipantAdminStatusChanged(const std::shared_ptr<linphone::Conference> & conference, const std::shared_ptr<const linphone::Participant> & participant) {};
			
			/**
			 * @brief Callback used to notify a chat room state has changed. 
			 *
			 * @param newState The new state of the chat room 
			 */
			LINPHONECXX_PUBLIC virtual void onStateChanged(const std::shared_ptr<linphone::Conference> & conference, linphone::Conference::State newState) {};
			
			/**
			 * @brief Callback used to notify that the subject of a chat room has changed. 
			 *
			 * @param subject subject of the conference   
			 */
			LINPHONECXX_PUBLIC virtual void onSubjectChanged(const std::shared_ptr<linphone::Conference> & conference, const std::string & subject) {};
			
			
	};

};

#endif // _LINPHONE_CONFERENCE_LISTENER_HH
