
/** Multi-page file stream

	fipMultiPage encapsulates the multi-page API. It supports reading/writing 
	multi-page TIFF, ICO and GIF files. 
*/
class FIP_API fipMultiPage : public fipObject 
{
protected:
	/// Pointer to a multi-page file stream
	FIMULTIBITMAP *_mpage;
	/// TRUE when using a memory cache, FALSE otherwise
	bool _bMemoryCache;

public:
	/**
	Constructor
	@param keep_cache_in_memory When it is TRUE, all gathered bitmap data in the page manipulation process is kept in memory, otherwise it is lazily flushed to a temporary file on the hard disk in 64 Kb blocks.
	*/
	fipMultiPage (bool keep_cache_in_memory = FALSE);

	/**
	Destructor
	Close the file stream if not already done. 
	*/
	virtual ~fipMultiPage ();

	/// Returns TRUE if the multi-page stream is opened
	bool isValid () const;

	/**
	Returns a pointer to the FIMULTIBITMAP data. Used for direct access from FREEIMAGE functions 
	or from your own low level C functions.
	*/
	operator FIMULTIBITMAP* () { 
		return _mpage; 
	}

	/**
	Open a multi-page file stream
	@param lpszPathName Name of the multi-page bitmap file
	@param create_new When TRUE, it means that a new bitmap will be created rather than an existing one being opened
	@param read_only When TRUE the bitmap is opened read-only
	@param flags Load flags. The signification of this flag depends on the image to be loaded.
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_OpenMultiBitmap
	*/
	bool open (const char* lpszPathName, bool create_new, bool read_only, int flags = 0);

	/**
	Open a multi-page memory stream as read/write. 
	@param memIO Memory stream. The memory stream MUST BE a wrapped user buffer. 
	@param flags Load flags. The signification of this flag depends on the image to be loaded.
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_LoadMultiBitmapFromMemory
	*/
	bool open (fipMemoryIO& memIO, int flags = 0);

	/**
	Open a multi-page image as read/write, using the specified FreeImageIO struct and fi_handle, and an optional flag.
	@param io FreeImageIO structure
	@param handle FreeImage fi_handle
	@param flag The signification of this flag depends on the image to be read.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_OpenMultiBitmapFromHandle
	*/
	bool open (FreeImageIO *io, fi_handle handle, int flags = 0);

	/**
	Close a file stream
	@param flags Save flags. The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_CloseMultiBitmap
	*/
	bool close (int flags = 0);

	/**
	Saves a multi-page image using the specified FreeImageIO struct and fi_handle, and an optional flag.
	@param fif Format identifier (FreeImage format)
	@param io FreeImageIO structure
	@param handle FreeImage fi_handle
	@param flag The signification of this flag depends on the multi-page image to be saved.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_SaveMultiBitmapToHandle, FreeImage documentation
	*/
	bool saveToHandle (FREE_IMAGE_FORMAT fif, FreeImageIO *io, fi_handle handle, int flags = 0) const;

	/**
	Saves a multi-page image using the specified memory stream and an optional flag.
	@param fif Format identifier (FreeImage format)
	@param memIO FreeImage memory stream
	@param flag The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_SaveMultiBitmapToMemory, FreeImage documentation
	*/
	bool saveToMemory (FREE_IMAGE_FORMAT fif, fipMemoryIO& memIO, int flags = 0) const;

	/**
	Returns the number of pages currently available in the multi-paged bitmap
	@see FreeImage_GetPageCount
	*/
	int getPageCount () const;

	/**
	Appends a new page to the end of the bitmap
	@param image Image to append
	@see FreeImage_AppendPage
	*/
	void appendPage (fipImage& image);

	/**
	Inserts a new page before the given position in the bitmap
	@param page Page number. Page has to be a number smaller than the current number of pages available in the bitmap.
	@param image Image to insert
	@see FreeImage_InsertPage
	*/
	void insertPage (int page, fipImage& image);

	/**
	Deletes the page on the given position
	@param page Page number
	@see FreeImage_DeletePage
	*/
	void deletePage (int page);

	/**
	Moves the source page to the position of the target page. 
	@param target Target page position
	@param source Source page position
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_MovePage
	*/
	bool movePage (int target, int source);

	/**
	Locks a page in memory for editing. You must call unlockPage to free the page<br>
	<b>Usage : </b><br>
	<pre>
	fipMultiPage mpage;
	// ...
	fipImage image;		// You must declare this before
	image = mpage.lockPage (2);
	if (image.isValid ()) {
	  // ...
	  mpage.unlockPage (image, TRUE);
	}
	</pre>
	@param page Page number
	@return Returns the page if successful, returns nullptr otherwise
	@see FreeImage_LockPage
	*/
	FIBITMAP* lockPage (int page);

	/**
	Unlocks a previously locked page and gives it back to the multi-page engine
	@param image Page to unlock
	@param changed When TRUE, the page is marked changed and the new page data is applied in the multi-page bitmap.
	@see FreeImage_UnlockPage
	*/
	void unlockPage (fipImage& image, bool changed);

	/**
	Returns an array of page-numbers that are currently locked in memory. 
	When the pages parameter is nullptr, the size of the array is returned in the count variable. 
	You can then allocate the array of the desired size and call 
	getLockedPageNumbers again to populate the array.
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_GetLockedPageNumbers
	*/
	bool getLockedPageNumbers (int *pages, int *count) const;
};
