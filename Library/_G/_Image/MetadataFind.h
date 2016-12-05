
/**
Metadata iterator

<b>Usage : </b><br>
<pre>
fipImage image;
// ...
fipTag tag;
fipMetadataFind finder;
if ( finder.findFirstMetadata (FIMD_EXIF_MAIN, image, tag) ) {
  do {
    // process the tag
	cout << tag.getKey () << "\n";

  } while ( finder.findNextMetadata (tag) );
}
// the class can be called again with another metadata model
if ( finder.findFirstMetadata (FIMD_EXIF_EXIF, image, tag) ) {
  do {
    // process the tag
	cout << tag.getKey () << "\n";

  } while ( finder.findNextMetadata (tag) );
}
</pre>
*/
class FIP_API fipMetadataFind : public fipObject
{
protected:
	/// Pointer to a search handle
	FIMETADATA *_mdhandle;

public:
	/// Returns TRUE if the search handle is allocated, FALSE otherwise
	bool isValid () const;

	/// Constructor
	fipMetadataFind ();
	/**
	Destructor
	@see FreeImage_FindCloseMetadata
	*/
	virtual ~fipMetadataFind ();
	/**
	Provides information about the first instance of a tag that matches 
	the metadata model specified in the <i>model</i> argument. 
	@param model Metadata model
	@param image Input image
	@param tag Returned tag
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_FindFirstMetadata
	*/
	bool findFirstMetadata (FREE_IMAGE_MDMODEL model, fipImage& image, fipTag& tag);
	/**
	Find the next tag, if any, that matches the metadata model argument 
	in a previous call to findFirstMetadata
	@param tag Returned tag
	@return Returns TRUE if successful, returns FALSE otherwise, indicating that no more matching tags could be found
	@see FreeImage_FindNextMetadata
	*/
	bool findNextMetadata (fipTag& tag);

};
