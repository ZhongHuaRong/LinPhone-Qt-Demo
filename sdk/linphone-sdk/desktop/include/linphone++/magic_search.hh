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

#ifndef _LINPHONE_MAGIC_SEARCH_HH
#define _LINPHONE_MAGIC_SEARCH_HH

#include <string>
#include <list>
#include "object.hh"




struct _LinphoneMagicSearch;


namespace linphone {

	class SearchResult;

	/**
	 * @brief A @ref MagicSearch is used to do specifics searchs. 
	 *
	 */
	class MagicSearch: public Object {
	
		public:

			MagicSearch(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneMagicSearch *cPtr() {return (_LinphoneMagicSearch *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Get the delimiter used for the search. 
			 *
			 * @return the delimiter used to find matched filter word   
			 */
			LINPHONECXX_PUBLIC std::string getDelimiter() const;
			
			/**
			 * @brief Set the delimiter used to find matched filter word. 
			 *
			 * @param delimiter delimiter (example "-_.,")   
			 */
			LINPHONECXX_PUBLIC void setDelimiter(const std::string & delimiter);
			
			/**
			 * @brief Return whether or not the search is limited. 
			 *
			 * @return true if the search is limited, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool getLimitedSearch() const;
			
			/**
			 * @brief Enable or disable the limited search. 
			 *
			 * @param limited true to limit the search, false otherwise 
			 */
			LINPHONECXX_PUBLIC void setLimitedSearch(bool limited);
			
			/**
			 * @brief Get the maximum value used to calculate the weight in search. 
			 *
			 * @return the maximum value used to calculate the weight in search 
			 */
			LINPHONECXX_PUBLIC unsigned int getMaxWeight() const;
			
			/**
			 * @brief Set the maximum value used to calculate the weight in search. 
			 *
			 * @param weight maximum weight 
			 */
			LINPHONECXX_PUBLIC void setMaxWeight(unsigned int weight);
			
			/**
			 * @brief Get the minimum value used to calculate the weight in search. 
			 *
			 * @return the minimum value used to calculate the weight in search 
			 */
			LINPHONECXX_PUBLIC unsigned int getMinWeight() const;
			
			/**
			 * @brief Set the minimum value used to calculate the weight in search. 
			 *
			 * @param weight minimum weight 
			 */
			LINPHONECXX_PUBLIC void setMinWeight(unsigned int weight);
			
			/**
			 * @brief Get the number of maximum search result the search will return. 
			 *
			 * @return the number of the maximum @ref SearchResult which will be returned 
			 */
			LINPHONECXX_PUBLIC unsigned int getSearchLimit() const;
			
			/**
			 * @brief Set the number of the maximum SearchResult which will be returned. 
			 *
			 * @param limit the maximum number of @ref SearchResult the search will return 
			 */
			LINPHONECXX_PUBLIC void setSearchLimit(unsigned int limit);
			
			/**
			 * @brief Returns whether the delimiter is being used for the search. 
			 *
			 * @return if the delimiter search is used 
			 */
			LINPHONECXX_PUBLIC bool getUseDelimiter();
			
			/**
			 * @brief Enable or disable the delimiter in search. 
			 *
			 * @param enable true to use the delimiter, false otherwise 
			 */
			LINPHONECXX_PUBLIC void setUseDelimiter(bool enable);
			
			/**
			 * @brief Create a sorted list of SearchResult from SipUri, Contact name, Contact
			 * displayname, Contact phone number, which match with a filter word The last item
			 * list will be an address formed with "filter" if a proxy config exist During the
			 * first search, a cache is created and used for the next search Use
			 * resetSearchCache() to begin a new search. 
			 *
			 * @param domain domain which we want to search only  
			 * @return sorted list of      
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::SearchResult>> getContactListFromFilter(const std::string & filter, const std::string & domain) const;
			
			/**
			 * @brief Reset the cache to begin a new search. 
			 *
			 */
			LINPHONECXX_PUBLIC void resetSearchCache();
			
			
	};

};

#endif // _LINPHONE_MAGIC_SEARCH_HH
