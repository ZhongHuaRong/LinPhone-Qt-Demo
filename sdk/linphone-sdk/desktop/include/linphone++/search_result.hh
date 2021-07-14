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

#ifndef _LINPHONE_SEARCH_RESULT_HH
#define _LINPHONE_SEARCH_RESULT_HH

#include <string>
#include "enums.hh"
#include "object.hh"




struct _LinphoneSearchResult;


namespace linphone {

	class Address;
	class Friend;

	/**
	 * @brief The LinphoneSearchResult object represents a result of a search. 
	 *
	 */
	class SearchResult: public Object {
	
		public:

			SearchResult(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneSearchResult *cPtr() {return (_LinphoneSearchResult *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Gets the address of the search result if any. 
			 *
			 * @return The associed @ref Address or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getAddress() const;
			
			/**
			 * @brief Returns the capabilities mask of the search result. 
			 *
			 * @return the capabilities mask associated to the search result 
			 */
			LINPHONECXX_PUBLIC int getCapabilities() const;
			
			/**
			 * @brief Gets the friend of the search result if any. 
			 *
			 * @return The associated @ref Friend or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Friend> getFriend() const;
			
			/**
			 * @brief Gets the phone number of the search result if any. 
			 *
			 * @return The associed phone number or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::string getPhoneNumber() const;
			
			/**
			 * @brief Gets the weight of the search result. 
			 *
			 * @return the result weight 
			 */
			LINPHONECXX_PUBLIC unsigned int getWeight() const;
			
			/**
			 * @brief Returns whether or not the search result has the given capability. 
			 *
			 * @param capability the @ref FriendCapability to check 
			 * @return true if it has the capability, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool hasCapability(linphone::FriendCapability capability) const;
			
			
	};

};

#endif // _LINPHONE_SEARCH_RESULT_HH
