
/** Abstract base class for all objects used by the library.
	@version FreeImage 3
	@author Hervé Drolon
*/

class FIP_API fipObject
{
public:
	/// Destructor
	virtual ~fipObject(){};
	
	/**@name Information functions */
	//@{
	/// Returns TRUE if the object is allocated, FALSE otherwise
	virtual BOOL isValid() const = 0;
	//@}
};