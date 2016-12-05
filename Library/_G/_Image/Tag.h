
/**
FreeImage Tag

FreeImage uses this structure to store metadata information. 
*/
class FIP_API fipTag : public fipObject
{
protected:
	/// Pointer to a FreeImage tag
	FITAG *_tag;

public:
	/**@name Creation & Destruction */
	//@{	
	/**
	Constructor
	@see FreeImage_CreateTag
	*/
	fipTag ();
	/** 
	Destructor
	@see FreeImage_DeleteTag
	*/
	virtual ~fipTag ();
	/**
	Construct a FIDT_ASCII tag (ASCII string).<br>
	This method is useful to store comments or IPTC tags. 
	@param name Field name
	@param value Field value
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_CreateTag
	*/
	bool setKeyValue (const char *key, const char *value);

	//@}

	/**@name Copying */
	//@{	
	/**
	Copy constructor
	@see FreeImage_CloneTag
	*/
	fipTag (const fipTag& tag);
	/**
	Copy constructor
	@see FreeImage_CloneTag
	*/
	fipTag& operator= (const fipTag& tag);
	/**
	<b>Assignement operator</b><br>
	Copy the input pointer and manage its destruction
	@see operator FITAG* ()
	*/
	fipTag& operator= (FITAG *tag);
	//@}

	/**
	Returns a pointer to the FITAG data. Used for direct access from FREEIMAGE functions 
	or from your own low level C functions.
	@see operator= (FITAG *tag)
	*/
	operator FITAG* () { 
		return _tag; 
	}

	/// Returns TRUE if the tag is allocated, FALSE otherwise
	bool isValid () const;

	/**@name Tag accessors */
	//@{	
	/**
	Returns the tag field name (unique inside a metadata model).
	@see FreeImage_GetTagKey
	*/
	const char *getKey () const;
	/**
	Returns the tag description if available, returns nullptr otherwise
	@see FreeImage_GetTagDescription
	*/
	const char *getDescription () const;
	/**
	Returns the tag ID if available, returns 0 otherwise
	@see FreeImage_GetTagID
	*/
	WORD getID () const;
	/**
	Returns the tag data type 
	@see FreeImage_GetTagType
	*/
	FREE_IMAGE_MDTYPE getType () const;
	/**
	Returns the number of components in the tag (in tag type units)
	@see FreeImage_GetTagCount
	*/
	DWORD getCount () const;
	/**
	Returns the length of the tag value in bytes
	@see FreeImage_GetTagLength
	*/
	DWORD getLength () const;
	/**
	Returns the tag value
	@see FreeImage_GetTagValue
	*/
	const void *getValue () const;
	/**
	Set the tag field name 
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SetTagKey
	*/
	bool setKey (const char *key);
	/**
	Set the (usually optional) tag description
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SetTagDescription
	*/
	bool setDescription (const char *description);
	/**
	Set the (usually optional) tad ID
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SetTagID
	*/
	bool setID (WORD id);
	/**
	Set the tag data type 
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SetTagType
	*/
	bool setType (FREE_IMAGE_MDTYPE type);
	/**
	Set the number of data in the tag 
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SetTagCount
	*/
	bool setCount (DWORD count);
	/**
	Set the length of the tag value, in bytes 
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SetTagLength
	*/
	bool setLength (DWORD length);
	/**
	Set the tag value 
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SetTagValue
	*/
	bool setValue (const void *value);

	//@}

	/**
	Converts a FreeImage tag structure to a string that represents the interpreted tag value
	@param model Metadata model specification (metadata model from which the tag was extracted)
	@param Make Camera model (not used yet)
	*/
	const char* toString (FREE_IMAGE_MDMODEL model, char *Make = nullptr) const;

};
