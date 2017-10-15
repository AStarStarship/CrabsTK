////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (c) 2007-2016 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Texture.h>
#include <SFML/Graphics/Image.h>
#include <SFML/Graphics/GLCheck.h>
#include <SFML/Graphics/TextureSaver.h>
#include <SFML/Window/Context.h>
#include <SFML/Window/Window.h>
#include <SFML/System/Mutex.h>
#include <SFML/System/Lock.h>
#include <_/Err.h>
#include <cassert>
#include <cstring>


namespace
{
    _G::Mutex idMutex;
    _G::Mutex maximumSizeMutex;

    // Thread-safe unique identifier generator,
    // is used for states cache (see RenderTarget)
    _G::Uint64 getUniqueId()
    {
        _G::Lock lock(idMutex);

        static _G::Uint64 id = 1; // start at 1, zero is "no texture"

        return id++;
    }
}


namespace _G
{
////////////////////////////////////////////////////////////
Texture::Texture() :
m_size         (0, 0),
m_actualSize   (0, 0),
m_texture      (0),
m_isSmooth     (false),
m_sRgb         (false),
m_isRepeated   (false),
m_pixelsFlipped(false),
m_fboAttachment(false),
m_hasMipmap    (false),
m_cacheId      (getUniqueId())
{
}


////////////////////////////////////////////////////////////
Texture::Texture(const Texture& copy) :
m_size         (0, 0),
m_actualSize   (0, 0),
m_texture      (0),
m_isSmooth     (copy.m_isSmooth),
m_sRgb         (copy.m_sRgb),
m_isRepeated   (copy.m_isRepeated),
m_pixelsFlipped(false),
m_fboAttachment(false),
m_hasMipmap    (false),
m_cacheId      (getUniqueId())
{
    if (copy.m_texture)
        loadFromImage(copy.copyToImage());
}


////////////////////////////////////////////////////////////
Texture::~Texture()
{
    // Destroy the OpenGL texture
    if (m_texture)
    {
        TransientContextLock lock;

        GLuint texture = static_cast<GLuint>(m_texture);
        glCheck(glDeleteTextures(1, &texture));
    }
}


////////////////////////////////////////////////////////////
bool Texture::create(unsigned int width, unsigned int height)
{
    // Check if texture parameters are valid before creating it
    if ((width == 0) || (height == 0))
    {
        err() << "Failed to create texture, invalid size (" << width << "x" << height << ")" << std::endl;
        return false;
    }

    // Compute the internal texture dimensions depending on NPOT textures support
    Vector2u actualSize(getValidSize(width), getValidSize(height));

    // Check the maximum texture size
    unsigned int maxSize = getMaximumSize();
    if ((actualSize.x > maxSize) || (actualSize.y > maxSize))
    {
        err() << "Failed to create texture, its internal size is too high "
              << "(" << actualSize.x << "x" << actualSize.y << ", "
              << "maximum is " << maxSize << "x" << maxSize << ")"
              << std::endl;
        return false;
    }

    // All the validity checks passed, we can store the new texture settings
    m_size.x        = width;
    m_size.y        = height;
    m_actualSize    = actualSize;
    m_pixelsFlipped = false;
    m_fboAttachment = false;

    TransientContextLock lock;

    // Create the OpenGL texture if it doesn't exist yet
    if (!m_texture)
    {
        GLuint texture;
        glCheck(glGenTextures(1, &texture));
        m_texture = static_cast<unsigned int>(texture);
    }

    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    static bool textureEdgeClamp = GLEXT_texture_edge_clamp || GLEXT_EXT_texture_edge_clamp;

    if (!m_isRepeated && !textureEdgeClamp)
    {
        static bool warned = false;

        if (!warned)
        {
            err() << "OpenGL extension SGIS_texture_edge_clamp unavailable" << std::endl;
            err() << "Artifacts may occur along texture edges" << std::endl;
            err() << "Ensure that hardware acceleration is enabled if available" << std::endl;

            warned = true;
        }
    }

    static bool textureSrgb = GLEXT_texture_sRGB;

    if (m_sRgb && !textureSrgb)
    {
        static bool warned = false;

        if (!warned)
        {
#ifndef SFML_OPENGL_ES
            err() << "OpenGL extension EXT_texture_sRGB unavailable" << std::endl;
#else
            err() << "OpenGL ES extension EXT_sRGB unavailable" << std::endl;
#endif
            err() << "Automatic sRGB to linear conversion disabled" << std::endl;

            warned = true;
        }

        m_sRgb = false;
    }

    // Initialize the texture
    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(glTexImage2D(GL_TEXTURE_2D, 0, (m_sRgb ? GLEXT_GL_SRGB8_ALPHA8 : GL_RGBA), m_actualSize.x, m_actualSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GLEXT_GL_CLAMP_TO_EDGE : GLEXT_GL_CLAMP)));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GLEXT_GL_CLAMP_TO_EDGE : GLEXT_GL_CLAMP)));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
    m_cacheId = getUniqueId();

    m_hasMipmap = false;

    return true;
}


////////////////////////////////////////////////////////////
bool Texture::loadFromFile(const std::string& filename, const IntRect& area)
{
    Image image;
    return image.loadFromFile(filename) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area)
{
    Image image;
    return image.loadFromMemory(data, size) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromStream(InputStream& stream, const IntRect& area)
{
    Image image;
    return image.loadFromStream(stream) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromImage(const Image& image, const IntRect& area)
{
    // Retrieve the image size
    int width = static_cast<int>(image.getSize().x);
    int height = static_cast<int>(image.getSize().y);

    // Load the entire image if the source area is either empty or contains the whole image
    if (area.width == 0 || (area.height == 0) ||
       ((area.left <= 0) && (area.top <= 0) && (area.width >= width) && (area.height >= height)))
    {
        // Load the entire image
        if (create(image.getSize().x, image.getSize().y))
        {
            update(image);

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // Load a sub-area of the image

        // Adjust the rectangle to the size of the image
        IntRect rectangle = area;
        if (rectangle.left   < 0) rectangle.left = 0;
        if (rectangle.top    < 0) rectangle.top  = 0;
        if (rectangle.left + rectangle.width > width)  rectangle.width  = width - rectangle.left;
        if (rectangle.top + rectangle.height > height) rectangle.height = height - rectangle.top;

        // Create the texture and upload the pixels
        if (create(rectangle.width, rectangle.height))
        {
            TransientContextLock lock;

            // Make sure that the current texture binding will be preserved
            priv::TextureSaver save;

            // Copy the pixels to the texture, row by row
            const Uint8* pixels = image.getPixelsPtr() + 4 * (rectangle.left + (width * rectangle.top));
            glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
            for (int i = 0; i < rectangle.height; ++i)
            {
                glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, rectangle.width, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
                pixels += 4 * width;
            }

            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
            m_hasMipmap = false;

            // Force an OpenGL flush, so that the texture will appear updated
            // in all contexts immediately (solves problems in multi-threaded apps)
            glCheck(glFlush());

            return true;
        }
        else
        {
            return false;
        }
    }
}


////////////////////////////////////////////////////////////
Vector2u Texture::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
Image Texture::copyToImage() const
{
    // Easy case: empty texture
    if (!m_texture)
        return Image();

    TransientContextLock lock;

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    // Create an array of pixels
    std::vector<Uint8> pixels(m_size.x * m_size.y * 4);

#ifdef SFML_OPENGL_ES

    // OpenGL ES doesn't have the glGetTexImage function, the only way to read
    // from a texture is to bind it to a FBO and use glReadPixels
    GLuint frameBuffer = 0;
    glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
    if (frameBuffer)
    {
        GLint previousFrameBuffer;
        glCheck(glGetIntegerv(GLEXT_GL_FRAMEBUFFER_BINDING, &previousFrameBuffer));

        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, frameBuffer));
        glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0));
        glCheck(glReadPixels(0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
        glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));

        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, previousFrameBuffer));
    }

#else

    if ((m_size == m_actualSize) && !m_pixelsFlipped)
    {
        // Texture is not padded nor flipped, we can use a direct copy
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
    }
    else
    {
        // Texture is either padded or flipped, we have to use a slower algorithm

        // All the pixels will first be copied to a temporary array
        std::vector<Uint8> allPixels(m_actualSize.x * m_actualSize.y * 4);
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]));

        // Then we copy the useful pixels from the temporary array to the final one
        const Uint8* src = &allPixels[0];
        Uint8* dst = &pixels[0];
        int srcPitch = m_actualSize.x * 4;
        int dstPitch = m_size.x * 4;

        // Handle the case where source pixels are flipped vertically
        if (m_pixelsFlipped)
        {
            src += srcPitch * (m_size.y - 1);
            srcPitch = -srcPitch;
        }

        for (unsigned int i = 0; i < m_size.y; ++i)
        {
            std::memcpy(dst, src, dstPitch);
            src += srcPitch;
            dst += dstPitch;
        }
    }

#endif // SFML_OPENGL_ES

    // Create the image
    Image image;
    image.create(m_size.x, m_size.y, &pixels[0]);

    return image;
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels)
{
    // Update the whole texture
    update(pixels, m_size.x, m_size.y, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    assert(x + width <= m_size.x);
    assert(y + height <= m_size.y);

    if (pixels && m_texture)
    {
        TransientContextLock lock;

        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        // Copy pixels from the given array to the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
        m_hasMipmap = false;
        m_pixelsFlipped = false;
        m_cacheId = getUniqueId();

        // Force an OpenGL flush, so that the texture data will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glCheck(glFlush());
    }
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image)
{
    // Update the whole texture
    update(image.getPixelsPtr(), image.getSize().x, image.getSize().y, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image, unsigned int x, unsigned int y)
{
    update(image.getPixelsPtr(), image.getSize().x, image.getSize().y, x, y);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window)
{
    update(window, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window, unsigned int x, unsigned int y)
{
    assert(x + window.getSize().x <= m_size.x);
    assert(y + window.getSize().y <= m_size.y);

    if (m_texture && window.setActive(true))
    {
        TransientContextLock lock;

        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        // Copy pixels from the back-buffer to the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 0, 0, window.getSize().x, window.getSize().y));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
        m_hasMipmap = false;
        m_pixelsFlipped = true;
        m_cacheId = getUniqueId();

        // Force an OpenGL flush, so that the texture will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glCheck(glFlush());
    }
}


////////////////////////////////////////////////////////////
void Texture::setSmooth(bool smooth)
{
    if (smooth != m_isSmooth)
    {
        m_isSmooth = smooth;

        if (m_texture)
        {
            TransientContextLock lock;

            // Make sure that the current texture binding will be preserved
            priv::TextureSaver save;

            glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));

            if (m_hasMipmap)
            {
                glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR));
            }
            else
            {
                glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
            }
        }
    }
}


////////////////////////////////////////////////////////////
bool Texture::isSmooth() const
{
    return m_isSmooth;
}


////////////////////////////////////////////////////////////
void Texture::setSrgb(bool sRgb)
{
    m_sRgb = sRgb;
}


////////////////////////////////////////////////////////////
bool Texture::isSrgb() const
{
    return m_sRgb;
}


////////////////////////////////////////////////////////////
void Texture::setRepeated(bool repeated)
{
    if (repeated != m_isRepeated)
    {
        m_isRepeated = repeated;

        if (m_texture)
        {
            TransientContextLock lock;

            // Make sure that the current texture binding will be preserved
            priv::TextureSaver save;

            static bool textureEdgeClamp = GLEXT_texture_edge_clamp || GLEXT_EXT_texture_edge_clamp;

            if (!m_isRepeated && !textureEdgeClamp)
            {
                static bool warned = false;

                if (!warned)
                {
                    err() << "OpenGL extension SGIS_texture_edge_clamp unavailable" << std::endl;
                    err() << "Artifacts may occur along texture edges" << std::endl;
                    err() << "Ensure that hardware acceleration is enabled if available" << std::endl;

                    warned = true;
                }
            }

            glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GLEXT_GL_CLAMP_TO_EDGE : GLEXT_GL_CLAMP)));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GLEXT_GL_CLAMP_TO_EDGE : GLEXT_GL_CLAMP)));
        }
    }
}


////////////////////////////////////////////////////////////
bool Texture::isRepeated() const
{
    return m_isRepeated;
}


////////////////////////////////////////////////////////////
bool Texture::generateMipmap()
{
    if (!m_texture)
        return false;

    TransientContextLock lock;

    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();

    if (!GLEXT_framebuffer_object)
        return false;

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(GLEXT_glGenerateMipmap(GL_TEXTURE_2D));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR));

    m_hasMipmap = true;

    return true;
}


////////////////////////////////////////////////////////////
void Texture::invalidateMipmap()
{
    if (!m_hasMipmap)
        return;

    TransientContextLock lock;

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));

    m_hasMipmap = false;
}


////////////////////////////////////////////////////////////
void Texture::bind(const Texture* texture, CoordinateType coordinateType)
{
    TransientContextLock lock;

    if (texture && texture->m_texture)
    {
        // Bind the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, texture->m_texture));

        // Check if we need to define a special texture matrix
        if ((coordinateType == Pixels) || texture->m_pixelsFlipped)
        {
            GLfloat matrix[16] = {1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f};

            // If non-normalized coordinates (= pixels) are requested, we need to
            // setup scale factors that convert the range [0 .. size] to [0 .. 1]
            if (coordinateType == Pixels)
            {
                matrix[0] = 1.f / texture->m_actualSize.x;
                matrix[5] = 1.f / texture->m_actualSize.y;
            }

            // If pixels are flipped we must invert the Y axis
            if (texture->m_pixelsFlipped)
            {
                matrix[5] = -matrix[5];
                matrix[13] = static_cast<float>(texture->m_size.y) / texture->m_actualSize.y;
            }

            // Load the matrix
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glLoadMatrixf(matrix));

            // Go back to model-view mode (_G::RenderTarget relies on it)
            glCheck(glMatrixMode(GL_MODELVIEW));
        }
    }
    else
    {
        // Bind no texture
        glCheck(glBindTexture(GL_TEXTURE_2D, 0));

        // Reset the texture matrix
        glCheck(glMatrixMode(GL_TEXTURE));
        glCheck(glLoadIdentity());

        // Go back to model-view mode (_G::RenderTarget relies on it)
        glCheck(glMatrixMode(GL_MODELVIEW));
    }
}


////////////////////////////////////////////////////////////
unsigned int Texture::getMaximumSize()
{
    Lock lock(maximumSizeMutex);

    static bool checked = false;
    static GLint size = 0;

    if (!checked)
    {
        checked = true;

        TransientContextLock lock;

        glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));
    }

    return static_cast<unsigned int>(size);
}


////////////////////////////////////////////////////////////
Texture& Texture::operator =(const Texture& right)
{
    Texture temp(right);

    std::swap(m_size,          temp.m_size);
    std::swap(m_actualSize,    temp.m_actualSize);
    std::swap(m_texture,       temp.m_texture);
    std::swap(m_isSmooth,      temp.m_isSmooth);
    std::swap(m_isRepeated,    temp.m_isRepeated);
    std::swap(m_pixelsFlipped, temp.m_pixelsFlipped);
    std::swap(m_fboAttachment, temp.m_fboAttachment);
    std::swap(m_hasMipmap,     temp.m_hasMipmap);
    m_cacheId = getUniqueId();

    return *this;
}


////////////////////////////////////////////////////////////
unsigned int Texture::getNativeHandle() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
unsigned int Texture::getValidSize(unsigned int size)
{
    TransientContextLock lock;

    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();

    if (GLEXT_texture_non_power_of_two)
    {
        // If hardware supports NPOT textures, then just return the unmodified size
        return size;
    }
    else
    {
        // If hardware doesn't support NPOT textures, we calculate the nearest power of two
        unsigned int powerOfTwo = 1;
        while (powerOfTwo < size)
            powerOfTwo *= 2;

        return powerOfTwo;
    }
}

bool drawLine (Canvas& c, _G::Color color, float xOne, float yOne, float xTwo, float yTwo)// returns false if the line is not drawn
{
    bool clipAlways = false, 
        pointONE    = false, 
        pointTWO    = false,
        rightEdge   = false,
        leftEdge    = false,
        topEdge     = false,
        bottomEdge  = false;
        
    float deltaX, 
        deltaY, 
        intersectX = 0.0, 
        intersectY = 0.0;

    if ((xOne>= 0.0 && xOne<= screenWidth ())&& (yOne>= 0.0 && yOne<= screenHeight ()))
        pointONE = true;
    if ((xTwo>= 0.0 && xTwo<= screenWidth ())&& (yTwo>= 0.0 && yTwo<= screenHeight ()))
        pointTWO = true;

    if (pointONE&&pointTWO)
    {  page.setColor (color);
        page.drawLine ((int)xOne, (int)yOne, (int)xTwo, (int)yTwo);
        return true;
    }

    if (!pointONE && !pointTWO)
     // this guy test to see if the line is in the cliping plane.
        if (((xOne<0     ) && (xTwo<0     )) || // left
              ((xOne>screenWidth ()) && (xTwo>screenWidth ())) || // right
              ((yOne<0     ) && (yTwo<0     )) || // top
              ((yOne>screenHeight ()) && (yTwo>screenHeight ())) ) // bottom
        {
            return false;
        }

    clipAlways = true;
 // Clip point two;
    if (pointONE|| (!pointONE&&!pointTWO))
    {
        deltaX = xTwo-xOne;
        deltaY = yTwo-yOne;
        if (xTwo<0)
        {
            leftEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * (0-xOne) + yOne;
            else
                intersectY = -1;
        }
        else
        if (xTwo>screenWidth ())
        {
            rightEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * ((float)screenWidth ()-xOne) + yOne;
            else
                intersectY = -1;
        }
    
        if (yTwo<0)
        {
            topEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * (0 - yOne) + xOne;
            else
                intersectX = -1;
        }
        else
        if (yTwo>screenHeight ())
        {
            bottomEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * ((float)screenHeight () - yOne) + xOne;
            else
                intersectX = -1;
        }
    
        if (rightEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xTwo = (float)screenWidth ();
            yTwo = intersectY;
        }
        else
        if (leftEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xTwo = 0;
            yTwo = intersectY;
        }
    
        if (bottomEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xTwo = intersectX;
            yTwo = (float)screenHeight ();
        }
        else
        if (topEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xTwo = intersectX;
            yTwo = 0;
        }
    }
    rightEdge = leftEdge = topEdge = bottomEdge = false;// reset flags
 // Clip point one
    if (pointTWO|| (!pointONE&&!pointTWO))
    {  deltaX = xOne-xTwo;
        deltaY = yOne-yTwo;
        if (xOne>screenWidth ())
        {
            rightEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * ((float)screenWidth ()-xTwo) + yTwo;
            else
                intersectY = -1;
        }
        else
        if (xOne<0)
        {
            leftEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * (0-xTwo) + yTwo;
            else
                intersectY = -1;
        }
        if (yOne>screenHeight ())
        {
            bottomEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * ((float)screenHeight () - yTwo) + xTwo;
            else
                intersectX = -1;
        }
        else
        if (yOne<0)
        {
            topEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * (0 - yTwo) + xTwo;
            else
                intersectX = -1;
        }
        if (rightEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xOne = (float)screenWidth ();
            yOne = intersectY;
        }
        else
        if (leftEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xOne = 0;
            yOne = intersectY;
        }
        if (bottomEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xOne = intersectX;
            yOne = (float)screenHeight ();
        }
        else
        if (topEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xOne = intersectX;
            yOne = 0;
        }
    }
    page.setColor (color);
    page.drawLine ((int)xOne, (int)yOne, (int)xTwo, (int)yTwo);
    return true;
}

void Texture::drawTriangle (_G::Color color, Point2D one, Point2D two, Point2D three)
{
    if ((one.x! = two.x&& two.x! = three.x)|| (one.y! = two.y&& two.y! = three.y))
    {
        Point2D temp;            
        if (two.y < one.y)
        {
            temp = two;
            two = one;
            one = temp;
        }
        if (three.y<one.y)
        {
            temp = three;
            three = one;
            one = temp;
        }
        if (three.y<two.y)
        {
            temp = three;
            three = two;
            two = temp;
        }
        if (three.y < clipMinY || one.y > clipMaxY ||
            (one.x < clipMinX && two.x < clipMinX && three.x < clipMinX) ||
            (one.x > clipMaxX && two.x > clipMaxX && three.x > clipMaxX))
        {
            ;
        }
        else if (one.y = =  two.y)
        {
            drawTopTriangle (page, color, one, two, three);
        }
        else if (two.y = =  three.y)
        {
            drawBottomTriangle (page, color, one, two, three);
        }
        else
        {
            float newX = one.x + ((two.y - one.y) * (three.x - one.x) / (three.y - one.y));
            Point2D newPoint = new Point2D ((int)newX, two.y);
            drawBottomTriangle (page, color, one, newPoint, two);
            drawTopTriangle (page, color, two, newPoint, three);
        }
    }
}

void Texture::drawTopTriangle (_G::Color color, Point2D one, Point2D two, Point2D three)
{
    float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
    page.setColor (color);
    if (two.x<one.x)
    {
        int temp = two.x;
        two.x = one.x;
        one.x = temp;
    }
    height = three.y-one.y;
    deltaXleft = (three.x-one.x)/height;
    deltaXright  = (three.x-two.x)/height;
    xStart = one.x;
    xEnd = two.x;

    if (one.y<clipMinY)// perform y cliping
    {
        xStart = xStart + deltaXleft * (-one.y+clipMinY);
        xEnd    = xEnd    + deltaXright* (-one.y+clipMinY);
        one.y = clipMinY;
    }        
    // test if x cliping is nessisary
    if (one.x  >= clipMinX && one.x  <= clipMaxX &&
         two.x  >= clipMinX && two.x  <= clipMaxX &&
         three.x>= clipMinX && three.x<= clipMaxX)
    {
        for (int y = (int)one.y; y <= (int)three.y; y++)
        {
            page.DrawLine ((int)xStart, y, (int)xEnd, y);
            xStart+= deltaXleft;
            xEnd  += deltaXright;
        }
    }
    else
    {
        for (int y =  (int)one.y; y<=  (int)three.y; y++)
        {
            left = xStart;
            right =  xEnd;
            xStart+= deltaXleft;
            xEnd  += deltaXright;
            if (left< clipMinX)
                left = clipMinX;
            if (right>clipMaxX)
                right =  clipMaxX;
            page.drawLine ((int)left, y, (int)right, y);
        }
    
    }
}

void Texture::drawBottomTriangle (_G::Canvas page, _G::Color color, Point2D one, Point2D two, Point2D three)
{
    float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
    page.setColor (color);
    if (three.x<two.x)
    {
        int temp = two.x;
        two.x = three.x;
        three.x = temp;
    }
    height = three.y-one.y;
    deltaXleft = (two.x-one.x)/height;
    deltaXright =  (three.x-one.x)/height;
    xStart = xEnd = one.x;
    
    if (one.y < clipMinY)// perform y cliping
    {
        xStart = xStart + deltaXleft * (-one.y+clipMinY);
        xEnd    = xEnd    + deltaXright* (-one.y+clipMinY);
        one.y = clipMinY;
    }
    if (three.y>clipMaxY)
        three.y = clipMaxY;

    if (one.x  >= clipMinX && one.x  <= clipMaxX &&
        two.x  >= clipMinX && two.x  <= clipMaxX &&
        three.x>= clipMinX && three.x<= clipMaxX)
    {
        for (int y =  (int)one.y; y<=  (int)three.y; y++)
        {
            page.drawLine ((int)xStart,y, (int)xEnd,y);
            xStart +=  deltaXleft;
            xEnd    +=  deltaXright;             
        }
    }
    else
    {
        for (int y = (int)one.y; y <= (int)three.y; y++)
        {
            left = xStart;
            right = xEnd;
            xStart +=  deltaXleft;
            xEnd    +=  deltaXright;
            if (left<clipMinX)
                left = clipMinX;
            if (right>clipMaxX)
                right = clipMaxX;
            page.drawLine ((int)left,y, (int)right,y);
        }
    }
}

} // namespace _G
