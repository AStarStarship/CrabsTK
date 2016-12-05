
/** Memory handle
	
	fipMemoryIO is a class that allows you to load / save images from / to a memory stream.
	@version FreeImage 3
	@author Hervé Drolon
*/
class FIP_API fipMemoryIO : public fipObject
{
protected:
	/// Pointer to a memory stream
	FIMEMORY *_hmem;

public :
	/** Constructor.
	Wrap a memory buffer containing image data.<br>
	The memory buffer is read only and has to be freed by the user 
	when no longer in use.<br>
	When default arguments are used, open a memory file as read/write. 
	@param data Pointer to the memory buffer
	@param size_in_bytes Buffer size in bytes
	@see FreeImage_OpenMemory
	*/
    fipMemoryIO (BYTE *data = nullptr, DWORD size_in_bytes = 0);

	/** Destructor.
	Free any allocated memory
	@see FreeImage_CloseMemory
	*/
	virtual ~fipMemoryIO ();

	/** Destructor.
	Free any allocated memory and invalidate the stream
	@see FreeImage_CloseMemory
	*/
	void close ();

	/** Returns TRUE if the internal memory buffer is a valid buffer, returns FALSE otherwise
	*/
	bool isValid () const;

	/** Returns the buffer image format
	@see FreeImage_GetFileTypeFromMemory
	*/
	FREE_IMAGE_FORMAT getFileType () const;

	/**
	Returns a pointer to the FIMEMORY data. Used for direct access from FREEIMAGE functions 
	or from your own low level C functions.
	*/
	operator FIMEMORY* () { 
		return _hmem; 
	}

	/**@name Memory IO routines */
	//@{	
	/**
	Loads a dib from a memory stream
	@param fif Format identifier (FreeImage format)
	@param flags The signification of this flag depends on the image to be loaded.
	@return Returns the loaded dib if successful, returns nullptr otherwise
	@see FreeImage_LoadFromMemory
	*/
	FIBITMAP* load (FREE_IMAGE_FORMAT fif, int flags = 0) const;
	/**
	Loads a multi-page bitmap from a memory stream
	@param fif Format identifier (FreeImage format)
	@param flags The signification of this flag depends on the multi-page to be loaded.
	@return Returns the loaded multi-page if successful, returns nullptr otherwise
	@see FreeImage_LoadMultiBitmapFromMemory
	*/
	FIMULTIBITMAP* loadMultiPage (FREE_IMAGE_FORMAT fif, int flags = 0) const;
	/**
	Saves a dib to a memory stream
	@param fif Format identifier (FreeImage format)
	@param dib Image to be saved
	@param flags The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SaveToMemory
	*/
	bool save (FREE_IMAGE_FORMAT fif, FIBITMAP *dib, int flags = 0);
	/**
	Saves a multi-page bitmap to a memory stream
	@param fif Format identifier (FreeImage format)
	@param bitmap Multi-page image to be saved
	@param flags The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SaveMultiBitmapToMemory
	*/
	bool saveMultiPage (FREE_IMAGE_FORMAT fif, FIMULTIBITMAP *bitmap, int flags = 0);
	/**
	Reads data from a memory stream
	@param buffer Storage location for data
	@param size Item size in bytes
	@param count Maximum number of items to be read
	@return Returns the number of full items actually read, which may be less than count if an error occurs
	@see FreeImage_ReadMemory
	*/
	unsigned read (void *buffer, unsigned size, unsigned count) const;
	/**
	Writes data to a memory stream
	@param buffer Pointer to data to be written
	@param size Item size in bytes
	@param count Maximum number of items to be written
	@return Returns the number of full items actually written, which may be less than count if an error occurs
	@see FreeImage_WriteMemory
	*/
	unsigned write (const void *buffer, unsigned size, unsigned count);
	/**
	Gets the current position of a memory pointer
	@see FreeImage_TellMemory
	*/
	long tell () const;
	/**
	Moves the memory pointer to a specified location
	@see FreeImage_SeekMemory
	*/
	bool seek (long offset, int origin);
	/**
	Provides a direct buffer access to a memory stream
	@param data Pointer to the memory buffer (returned value)
	@param size_in_bytes Buffer size in bytes (returned value)
	@see FreeImage_AcquireMemory
	*/
	bool acquire (BYTE **data, DWORD *size_in_bytes);
	//@}

private:
	/// Disable copy
	fipMemoryIO (const fipMemoryIO& src);
	/// Disable copy
	fipMemoryIO& operator= (const fipMemoryIO& src);

};
