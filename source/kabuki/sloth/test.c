/*$T test.c GC 1.140 01/07/05 19:35:38 */

/**
 *  @file       sMap.hpp
 *  @brief      The sMap class.
 */

/**
 *  @addtogroup Collection
 */

/*$I0
 * / \ | Copyright (C) 2004 Christophe Beaudet. | | All rights reserved. | | | |
 * This library is free software;
 * you can redistribute it and/or | | modify it under the terms of the GNU General
 * Public License | | as published by the Free Software Foundation;
 * either version 2 | | of the License, or (at your option) any later version. | |
 * | | This library is distributed in the hope that it will be useful, | | but
 * WITHOUT ANY WARRANTY;
 * without even the implied warranty of | | MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU | | Lesser General Public License for more
 * details. | | | | The text of the GNU Lesser General Public License is included
 * with | | this library in the file LICENCE.txt. | | | | You should have received
 * a copy of the GNU Lesser General Public | | License along with this library;
 * if not, write to : | | | | The Free Software Foundation, Inc. | | 59 Temple
 * Place - Suite 330 | | Boston, MA 02111-1307 | | USA. | \
 */
#define SMAP_MEMSIG "sLib::sMap"	///< Allocation signature.

/*$4-******************************************************************************************************************/

/**
 *  @brief      sMap class.
 *  @ingroup    Collection
 *
 *  @description
 *  sMap is used to associate a unique key to a value. Searching is more efficient than in a standard sList.
 */
class	sMap
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:
	typedef sPair<_TYPE_KEY, _TYPE_VAL> sMapNode;
	SLIB_U32							mu32_HashSize;	/* Size of hash map */
	SLIB_U32							mu32_Count;		/* Number of elements in the map */
	sList<sMapNode>						*mpo_List;		/* Hash map */

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:

	/*
	 ===================================================================================================================
	    @brief      Find the given key in its corresponding hash list.
	    @param      key     - Key to search.
	    @param      hash    - Hash value for the key.
	
	    @return     Position of the sMapNode. SCLC_INVALID_POSITION if key doesn't exist.
	 ===================================================================================================================
	 */
	SCLC_POSITION FindKey(const _TYPE_KEY &key, SLIB_U32 hash) const
	{
		/*~~~~~~~~~~~~~~~~~~~~~*/
		sList<sMapNode> *po_List;
		sMapNode		*pnode;
		SCLC_POSITION	pos;
		/*~~~~~~~~~~~~~~~~~~~~~*/

		po_List = &mpo_List[hash];
		for(pos = po_List->GetHeadPosition(); pos != SCLC_INVALID_POSITION; pos = po_List->GetNextPosition(pos))
		{
			pnode = po_List->GetAddressAt(pos);
			if(pnode->key == key) return pos;
		}

		return SCLC_INVALID_POSITION;
	}

	/*
	 ===================================================================================================================
	    @brief      Compute the hash key value for the given key.
	    @param      key     - Key to compute.
	
	    @return     Hash key value.
	 ===================================================================================================================
	 */
	SLIB_U32 GetHashKey(const _TYPE_KEY &key) const
	{
		return sTypeInfo<_TYPE_KEY>::Hash(key) % mu32_HashSize;
	}

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:

	/**
	 ===================================================================================================================
	    @brief      Construct a map.
	    @param      hash    - Size of hash table.
	    @param      gran    - Allocation granularity for sList composants.
	 ===================================================================================================================
	 */
	sMap(SLIB_U32 hash = 17, SLIB_U32 gran = 10)
	{
		/*~~~~~~~~~~*/
		SLIB_U32	i;
		/*~~~~~~~~~~*/

		SLIB_ASSERT(hash && gran);
		mu32_HashSize = hash;
		mu32_Count = 0;
		mpo_List = sNEWG(SMAP_MEMSIG, sList<sMapNode> )[mu32_HashSize];
		for(i = 0; i < mu32_HashSize; i++) mpo_List[i].SetGranularity(gran);
	}

	/**
	 ===================================================================================================================
	    @brief      Map is cleared, and all memory is deleted.
	 ===================================================================================================================
	 */
	~sMap(void)
	{
		RemoveAll();
		delete[] mpo_List;
		mpo_List = SLIB_NULL;
	}

	/**
	 ===================================================================================================================
	    @brief      Returns the number of elements in the map.
	
	    @return     Number of elements.
	 ===================================================================================================================
	 */
	SLIB_U32 GetCount(void) const
	{
		return mu32_Count;
	}

	/**
	 ===================================================================================================================
	    @brief      Add a new pair \a key / \a val in the map.
	    @param      key - Key to add.
	    @param      val     - Value to add.
	 ===================================================================================================================
	 */
	SCLC_POSITION Add(const sPair<_TYPE_KEY, _TYPE_VAL> &val)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
		SLIB_U32		hash;
		sMapNode		node, *pnode;
		SCLC_POSITION	pos;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

		hash = GetHashKey(val.key);
		pos = FindKey(val.key, hash);

		/* Yes, so just update in place the value */
		if(pos != SCLC_INVALID_POSITION)
		{
			pnode = mpo_List[hash].GetAddressAt(pos);
			pnode->value = val.value;
		}

		/* Else add new node */
		else
		{
			node.key = val.key;
			node.value = val.value;
			pos = mpo_List[hash].AddTail(node);
			mu32_Count++;
		}

		return pos;
	}

	/**
	 ===================================================================================================================
	    @brief      Add a new pair \a key / \a val in the map.
	    @param      key     - Key to add.
	    @param      val     - Value to add.
	 ===================================================================================================================
	 */
	SCLC_POSITION Add(const _TYPE_KEY &key, const _TYPE_VAL &val)
	{
		return Add(sPair<_TYPE_KEY, _TYPE_VAL> (key, val));
	}

	/**
	 ===================================================================================================================
	    @brief      Remove all elements of the map.
	 ===================================================================================================================
	 */
	void RemoveAll(void)
	{
		/*~~~~~~~~~~*/
		SLIB_U32	i;
		/*~~~~~~~~~~*/

		for(i = 0; i < mu32_HashSize; i++) mpo_List[i].RemoveAll();
		mu32_Count = 0;
	}

	/**
	 ===================================================================================================================
	    @brief      Remove a given element of the map.
	    @param      key     - Key of element to remove.
	
	    @remark     If \a key does not exist in the map, an assertion will be raised.
	 ===================================================================================================================
	 */
	void Remove(const _TYPE_KEY &key)
	{
		/*~~~~~~~~~~~~~~~~~*/
		SLIB_U32		hash;
		SCLC_POSITION	pos;
		/*~~~~~~~~~~~~~~~~~*/

		hash = GetHashKey(key);
		pos = FindKey(key, hash);
		SLIB_ASSERT(pos != SCLC_INVALID_POSITION);
		mpo_List[hash].RemoveAt(pos);
		mu32_Count--;
	}

	/**
	 ===================================================================================================================
	    @brief      Returns the first position of the map.
	
	    @return     First position.
	
	    @remark     To be used to scan all the elements of a map.
	 ===================================================================================================================
	 */
	SCLC_POSITION GetHeadPosition(void) const
	{
		/*~~~~~~~~~~*/
		SLIB_U32	i;
		/*~~~~~~~~~~*/

		for(i = 0; i < mu32_HashSize; i++)
		{
			if(mpo_List[i].GetCount()) return mpo_List[i].GetHeadPosition();
		}

		return SCLC_INVALID_POSITION;
	}

	/**
	 ===================================================================================================================
	    @brief      Returns the next position of the map.
	    @param      pos     - Reference position.
	
	    @return     Next position.
	
	    @remark     To be used to scan all the elements of a map.
	 ===================================================================================================================
	 */
	SCLC_POSITION GetNextPosition(SCLC_POSITION pos) const
	{
		/*~~~~~~~~~~~~~~~~~*/
		SCLC_POSITION	npos;
		SLIB_U32		i;
		/*~~~~~~~~~~~~~~~~~*/

		/* On renvoie la position suivante normale (indépendante de la liste) */
		npos = mpo_List[0].GetNextPosition(pos);
		if(npos) return npos;

		/* S'il y en a pas, on doit renvoyer la première position de la map suivante */
		for(i = 0; i < mu32_HashSize; i++)
		{
			if(pos == mpo_List[i].GetTailPosition())
			{
				i++;
				while(i < mu32_HashSize)
				{
					pos = mpo_List[i].GetHeadPosition();
					if(pos) return pos;
					i++;
				}

				return SCLC_INVALID_POSITION;
			}
		}

		return SCLC_INVALID_POSITION;
	}

	/**
	 ===================================================================================================================
	    @brief      Returns the key at a given position.
	    @param      pos     - Position to get the key.
	
	    @return     The key.
	 ===================================================================================================================
	 */
	_TYPE_KEY &GetKeyAt(SCLC_POSITION pos) const
	{
		/*~~~~~~~~~~~~~~~*/
		sMapNode	*pnode;
		/*~~~~~~~~~~~~~~~*/

		SLIB_ASSERT(pos != SCLC_INVALID_POSITION);
		pnode = mpo_List[0].GetAddressAt(pos);
		return pnode->key;
	}

	/**
	 ===================================================================================================================
	    @brief      Returns the value at a given position.
	    @param      pos     - Position to get the value.
	
	    @return     The value.
	 ===================================================================================================================
	 */
	_TYPE_VAL &GetValueAt(SCLC_POSITION pos) const
	{
		/*~~~~~~~~~~~~~~~*/
		sMapNode	*pnode;
		/*~~~~~~~~~~~~~~~*/

		SLIB_ASSERT(pos != SCLC_INVALID_POSITION);
		pnode = mpo_List[0].GetAddressAt(pos);
		return pnode->value;
	}

	/**
	 ===================================================================================================================
	    @brief      Return the sPair component at the given position.
	    @param      pos     - Position of pair to get.
	
	    @return     sPair value.
	 ===================================================================================================================
	 */
	const sPair<_TYPE_KEY, _TYPE_VAL> &GetAt(SCLC_POSITION pos) const
	{
		SLIB_ASSERT(pos != SCLC_INVALID_POSITION);
		return * (sMapNode *) pos;
	}

	/**
	 ===================================================================================================================
	    @brief      Find a given key in the map, and returns the corresponding value and position.
	    @param      key     - Key to search.
	    @param      val     - Value to retreive.
	
	    @return     The position of the key. ::SCLC_INVALID_POSITION if key is not present in the map.
	
	    @remark     If key is not present, \a val remains unchanged.
	 ===================================================================================================================
	 */
	SCLC_POSITION Find(const _TYPE_KEY &key, _TYPE_VAL &val) const
	{
		/*~~~~~~~~~~~~~~~~~*/
		SLIB_U32		hash;
		SCLC_POSITION	pos;
		/*~~~~~~~~~~~~~~~~~*/

		hash = GetHashKey(key);
		pos = FindKey(key, hash);
		if(pos != SCLC_INVALID_POSITION)
		{
			val = GetValueAt(pos);
			return pos;
		}

		return pos;
	}
};

/*$4-******************************************************************************************************************/
