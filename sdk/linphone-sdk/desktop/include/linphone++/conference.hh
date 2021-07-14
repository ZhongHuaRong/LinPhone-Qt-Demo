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

#ifndef _LINPHONE_CONFERENCE_HH
#define _LINPHONE_CONFERENCE_HH

#include <string>
#include <list>
#include "object.hh"




struct _LinphoneConference;


namespace linphone {

	class Address;
	class Core;
	class ConferenceParams;
	class Participant;
	class Call;
	class CallParams;
	class ConferenceListener;

	/**
	 * @brief TODO. 
	 *
	 */
	class Conference: public MultiListenableObject {
	
		public:
			/**
			* @ref State is used to indicate the current state of a conference. 
			*
			*/
			enum class State {
				/**
				* Initial state. 
				*/
				None,
				/**
				* Conference is now instantiated on local. 
				*/
				Instantiated,
				/**
				* One creation request was sent to the server. 
				*/
				CreationPending,
				/**
				* Conference was created on the server. 
				*/
				Created,
				/**
				* Conference creation failed. 
				*/
				CreationFailed,
				/**
				* Wait for conference termination. 
				*/
				TerminationPending,
				/**
				* Conference exists on server but not in local. 
				*/
				Terminated,
				/**
				* Conference termination failed. 
				*/
				TerminationFailed,
				/**
				* Conference was deleted on the server. 
				*/
				Deleted
			};


			Conference(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneConference *cPtr() {return (_LinphoneConference *)mPrivPtr;}

			LINPHONECXX_PUBLIC void addListener(const std::shared_ptr<ConferenceListener> &listener);
			LINPHONECXX_PUBLIC void removeListener(const std::shared_ptr<ConferenceListener> &listener);
	
		public:
			
		
			/**
			 * @brief Get the conference address of the conference. 
			 *
			 * @return The conference address of the conference or nullptr if it is unknown.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getConferenceAddress() const;
			
			/**
			 * @brief Returns core for a @ref Conference. 
			 *
			 * @return back pointer to @ref Core object.   Returns back pointer to @ref Core
			 * object. 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Core> getCore() const;
			
			/**
			 * @brief Get current parameters of the conference. 
			 *
			 * @return a @ref ConferenceParams .   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::ConferenceParams> getCurrentParams() const;
			
			/**
			 * @brief Get the conference id as string. 
			 *
			 * @return the conference id   
			 */
			LINPHONECXX_PUBLIC std::string getId() const;
			
			/**
			 * @brief Set the conference id as string. 
			 *
			 * @param conferenceId the conference id to set.   
			 */
			LINPHONECXX_PUBLIC void setId(const std::string & conferenceId);
			
			/**
			 * @brief For a local audio video conference, this function returns the
			 * participant hosting the conference For a remote audio video conference, this
			 * function returns the focus of the conference. 
			 *
			 * @return a @ref Participant .   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Participant> getMe() const;
			
			/**
			 * @brief Get number of participants. 
			 *
			 * @return the number of participants in a @ref Conference 
			 */
			LINPHONECXX_PUBLIC int getParticipantCount() const;
			
			/**
			 * @brief Get list of all participants of one conference. 
			 *
			 * @return A list of participants.      
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Participant>> getParticipantList() const;
			
			/**
			 * @brief Get URIs of all participants of one conference The returned bctbx_list_t
			 * contains URIs of all participant. 
			 *
			 * That list must be freed after use and each URI must be unref with
			 * linphone_address_unref 
			 * @return The list of the participants' address.      
			 * @deprecated 10/07/2020 Use getParticipantList() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::list<std::shared_ptr<linphone::Address>> getParticipants() const;
			
			/**
			 * @brief Get the conference subject. 
			 *
			 * @return conference subject.   
			 */
			LINPHONECXX_PUBLIC std::string getSubject() const;
			
			/**
			 * @brief Set the conference subject. 
			 *
			 * @param subject conference subject   
			 */
			LINPHONECXX_PUBLIC void setSubject(const std::string & subject);
			
			/**
			 * @brief Join an existing call to the conference. 
			 *
			 * @param call a @ref Call that has to be added to the conference.   
			 */
			LINPHONECXX_PUBLIC linphone::Status addParticipant(const std::shared_ptr<linphone::Call> & call);
			
			/**
			 * @brief Join a participant to the conference. 
			 *
			 * @param uri a @ref Address that has to be added to the conference.   
			 */
			LINPHONECXX_PUBLIC linphone::Status addParticipant(const std::shared_ptr<const linphone::Address> & uri);
			
			/**
			 * @brief Find a participant from a conference. 
			 *
			 * @param uri SIP URI of the participant to search.   
			 * @return a pointer to the participant found or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Participant> findParticipant(const std::shared_ptr<const linphone::Address> & uri);
			
			/**
			 * @brief Invite participants to the conference, by supplying a list of @ref
			 * Address. 
			 *
			 * @param params @ref CallParams to use for inviting the participants.   
			 */
			LINPHONECXX_PUBLIC linphone::Status inviteParticipants(const std::list<std::shared_ptr<linphone::Address>> & addresses, const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Call generic OpenGL render for a given conference. 
			 *
			 */
			LINPHONECXX_PUBLIC void oglRender();
			
			/**
			 * @brief Call generic OpenGL render preview for a given conference. 
			 *
			 */
			LINPHONECXX_PUBLIC void previewOglRender();
			
			/**
			 *
			 * @param uri URI of the participant to remove   
			 * @warning The passed participant uri must be one of those returned by
			 * getParticipants() 
			 * @return 0 if succeeded, -1 if failed 
			 * @deprecated 10/07/2020 Use removeParticipant() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED linphone::Status removeParticipant(const std::shared_ptr<const linphone::Address> & uri);
			
			/**
			 *
			 * @param participant participant to remove   
			 * @warning The passed participant must be one of those returned by
			 * getParticipantList() 
			 * @warning This method may destroy the conference if the only remaining
			 * participant had an existing call to the local participant before the conference
			 * was created 
			 * @return 0 if succeeded, -1 if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status removeParticipant(const std::shared_ptr<linphone::Participant> & participant);
			
			/**
			 *
			 * @param call call to remove   
			 * @return 0 if succeeded, -1 if failed 
			 * @deprecated 10/07/2020 Use removeParticipant() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED linphone::Status removeParticipant(const std::shared_ptr<linphone::Call> & call);
			
			/**
			 * @brief Terminates conference. 
			 *
			 * @return 0 if the termination is successful, -1 otherwise. 
			 */
			LINPHONECXX_PUBLIC int terminate();
			
			/**
			 * @brief Update parameters of the conference. 
			 *
			 * This is typically used enable or disable the video stream in the conference. 
			 * @param params the new parameters to apply.   
			 */
			LINPHONECXX_PUBLIC int updateParams(const std::shared_ptr<const linphone::ConferenceParams> & params);
			
			
		private:
			void *createCallbacks() override;
	};

};

#endif // _LINPHONE_CONFERENCE_HH
