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

#ifndef _LINPHONE_CONFERENCE_PARAMS_HH
#define _LINPHONE_CONFERENCE_PARAMS_HH

#include "object.hh"




struct _LinphoneConferenceParams;


namespace linphone {

	class ConferenceParams;

	/**
	 * @brief TODO. 
	 *
	 */
	class ConferenceParams: public Object {
	
		public:

			ConferenceParams(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneConferenceParams *cPtr() {return (_LinphoneConferenceParams *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Returns whether local participant has to enter the conference. 
			 *
			 * @return true if local participant is by default part of the conference, false
			 * otherwise 
			 */
			LINPHONECXX_PUBLIC bool localParticipantEnabled() const;
			
			/**
			 * @brief Enable local participant to enter the conference. 
			 *
			 * The local participant is the one driving the local @ref Core. It uses the local
			 * sound devices. The default value is true. Setting to false is mostly helpful
			 * when using liblinphone on a server application. 
			 * @param enable true if local participant is automatically added to the
			 * conference, false otherwise 
			 */
			LINPHONECXX_PUBLIC void enableLocalParticipant(bool enable);
			
			/**
			 * @brief Check whether video will be enable at conference starting. 
			 *
			 * @return true if the video will be enable at conference starting, false
			 * otherwise 
			 */
			LINPHONECXX_PUBLIC bool videoEnabled() const;
			
			/**
			 * @brief Enable video when starting a conference. 
			 *
			 * @param enable If true, video will be enabled during conference 
			 */
			LINPHONECXX_PUBLIC void enableVideo(bool enable);
			
			/**
			 * @brief Clone a @ref ConferenceParams. 
			 *
			 * @return An allocated @ref ConferenceParams with the same parameters than params
			 *   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ConferenceParams> clone() const;
			
			
	};

};

#endif // _LINPHONE_CONFERENCE_PARAMS_HH
