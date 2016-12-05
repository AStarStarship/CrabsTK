
/** A class designed for MS Windows (TM) platforms.

    fipWinImage provides methods used to :
	<ul>
	<li>Display a DIB on the screen
	<li>Copy / Paste a DIB to/from Windows devices (HANDLE, HBITMAP, Clipboard)
	<li>Capture a window (HWND) and convert it to an image
	</ul>
	@version FreeImage 3
	@author Hervé Drolon
*/
#ifdef _WIN32

class FIP_API fipWinImage : public fipImage
{
public:
	/**@name Creation & Destruction */
	//@{	
	/// Constructor
	fipWinImage (FREE_IMAGE_TYPE image_type = FIT_BITMAP, unsigned width = 0, unsigned height = 0, unsigned bpp = 0);

	/// Destructor
	virtual ~fipWinImage ();

	/// Destroy image data
	virtual void clear ();

	/// Returns TRUE if the image is allocated, FALSE otherwise
	bool isValid () const;
	//@}

	/**@name Copying */
	//@{	

	/**
	Copy constructor. 
	Delete internal _display_dib data and copy the base class image data. 
	Tone mapping parameters are left unchanged. 
	@see FreeImage_Clone
	*/
	fipWinImage& operator= (const fipImage& src);

	/**
	Copy constructor
	Delete internal _display_dib data and copy tone mapping parameters. 
	Copy also the base class image data. 
	@see FreeImage_Clone
	*/
	fipWinImage& operator= (const fipWinImage& src);

	/** Clone function used for clipboard copy.<br>
	Convert the FIBITMAP image to a DIB, 
	and transfer the DIB in a global bitmap handle.<br>
	For non standard bitmaps, the BITMAPINFOHEADER->biCompression field is set to 0xFF + FreeImage_GetImageType (_dib), 
	in order to recognize the bitmap as non standard. 
	*/
	HANDLE copyToHandle () const;

	/** Copy constructor used for clipboard paste.<br>
	Converts a global object to a FIBITMAP. The clipboard format must be CF_DIB.<br>
	When the BITMAPINFOHEADER->biCompression field is set to 0xFF + [one of the predefined FREE_IMAGE_TYPE], 
	the bitmap is recognized as non standard and correctly copied. 
	@return Returns TRUE if successful, returns FALSE otherwise
	*/
	bool copyFromHandle (HANDLE hMem);

	/** Copy constructor.<br>
	Converts a HBITMAP object to a FIBITMAP.
	@return Returns TRUE if successful, returns FALSE otherwise
	*/
	bool copyFromBitmap (HBITMAP hbmp);
	//@}

	/**@name Clipboard operations */
	//@{	
	/**
	Clipboard copy.
	@param hWndNewOwner Handle to the window to be associated with the open clipboard. 
	In MFC, you can use AfxGetApp ()->m_pMainWnd->GetSafeHwnd ().
	@return Returns TRUE if successful, returns FALSE otherwise
	*/
	bool copyToClipboard (HWND hWndNewOwner) const;

	/**
	Retrieves data from the clipboard. The clipboard format must be CF_DIB.
	@return Returns TRUE if successful, returns FALSE otherwise
	*/
	bool pasteFromClipboard ();
	//@}

	/**@name Screen capture */
	//@{	
	/** Capture a window and convert it to an image
	@param hWndApplicationWindow Handle to the application main window
	@param hWndSelectedWindow Handle to the window to be captured
	@return Returns TRUE if successful, returns FALSE otherwise
	*/
	bool captureWindow (HWND hWndApplicationWindow, HWND hWndSelectedWindow);
	//@}


	/**@name Painting operations */
	//@{	

	/** @brief Draw (stretch) the image on a HDC, using StretchDIBits.

    When the image is transparent or has a file background, this function composite 
	the foreground image against a checkerboard background image.
	@param hDC Handle to the device context
	@param rcDest Destination rectangle
	@see FreeImage_Composite
	*/
	void draw (HDC hDC, RECT& rcDest) const {
		drawEx (hDC, rcDest, FALSE, nullptr, nullptr);
	}

	/** @brief Draw (stretch) the image on a HDC, using StretchDIBits.

    When the image is transparent or has a file background, this function can composite 
	the foreground image against a checkerboard background image, against a single background color or 
	against a user background image.<br>
	When the image is a High Dynamic Range image (48-bit or RGB float), this function will apply a 
	tone mapping operator before drawing the image.<br>
	The original image (located in the fipImage class) will not be affected by any of the operations 
	that could be done in order to display it. 
	@param hDC Handle to the device context
	@param rcDest Destination rectangle
	@param useFileBkg When set to TRUE, the function uses the file color background if there is one
	@param appBkColor When a color is given, the function uses it as the background color
	@param bg When a FIBITMAP is given, the function uses it as the background image
	@see FreeImage_Composite
	@see setToneMappingOperator
	*/
	void drawEx (HDC hDC, RECT& rcDest, bool useFileBkg = FALSE, RGBQUAD *appBkColor = nullptr, FIBITMAP *bg = nullptr) const;

	/**
	Select a tone mapping algorithm used for drawing and set the image as modified 
	so that the display will be refreshed.
	@param tmo Tone mapping operator
	@param first_param First tone mapping algorithm parameter
	@param second_param Second tone mapping algorithm parameter
	@param third_param Third tone mapping algorithm parameter
	@param fourth_param Fourth tone mapping algorithm parameter
	@see FreeImage_ToneMapping
	*/
	void setToneMappingOperator (FREE_IMAGE_TMO tmo, double first_param = 0, double second_param = 0, double third_param = 1, double fourth_param = 0);

	/**
	Get the tone mapping algorithm used for drawing, with its parameters.
	@param tmo Tone mapping operator
	@param first_param First tone mapping algorithm parameter
	@param second_param Second tone mapping algorithm parameter
	@param third_param Third tone mapping algorithm parameter
	@param fourth_param Fourth tone mapping algorithm parameter
	@see FreeImage_ToneMapping
	*/
	void getToneMappingOperator (FREE_IMAGE_TMO *tmo, double *first_param, double *second_param, double *third_param, double *fourth_param) const;

	//@}

protected:
	/// DIB used for display (this allow to display non-standard bitmaps)
	mutable FIBITMAP *_display_dib;
	/// remember to delete _display_dib
	mutable bool _bDeleteMe;
	/// tone mapping operator
	FREE_IMAGE_TMO _tmo;
	/// first tone mapping algorithm parameter
	double _tmo_param_1;
	/// second tone mapping algorithm parameter
	double _tmo_param_2;
	/// third tone mapping algorithm parameter
	double _tmo_param_3;
	/// fourth tone mapping algorithm parameter
	double _tmo_param_4;
};

#endif // _WIN32

