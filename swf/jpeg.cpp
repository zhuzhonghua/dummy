#include "stdafx.h"

#include "jpeg.h"

namespace JPEG{
// jpeglib data source constructors, for using tu_file* instead
// of stdio for jpeg IO.
void	setupRWSource(jpeg_decompress_struct* cinfo, File* instream);
//void	setup_rw_dest(jpeg_compress_struct* cinfo, tu_file* outstream);
// Helper object for reading jpeg image data.  Basically a thin
static const int	IO_BUF_SIZE = 4096;

// A jpeglib source manager that reads from a tu_file.  Paraphrased
// from IJG jpeglib jdatasrc.c.
class RWSource{
public:
  struct jpeg_source_mgr	pub;		/* public fields */

  File*	instream;		/* source stream */
  bool	startOfFile;		/* have we gotten any data yet? */
  JOCTET	buffer[IO_BUF_SIZE];	/* start of buffer */

  // Constructor.  The caller is responsible for closing the input stream
  // after it's done using us.
  RWSource(File* in):instream(in),startOfFile(true)
  {
    // fill in function pointers...
    pub.init_source = initSource;
    pub.fill_input_buffer = fillInputBuffer;
    pub.skip_input_data = skipInputData;
    pub.resync_to_restart = jpeg_resync_to_restart;	// use default method
    pub.term_source = termSource;
    pub.bytes_in_buffer = 0;
    pub.next_input_byte = NULL;
  }

  static void initSource(j_decompress_ptr cinfo)
  {
    RWSource*	src = (RWSource*) cinfo->src;
    src->startOfFile = true;
  }

  // Read data into our input buffer.  Client calls this
  // when it needs more data from the file.
  static boolean	fillInputBuffer(j_decompress_ptr cinfo)
  {
    RWSource*	src = (RWSource*) cinfo->src;

    size_t	bytes_read = src->instream->readBytes(src->buffer, IO_BUF_SIZE);

    if (bytes_read <= 0) {
      // Is the file completely empty?
      if (src->startOfFile) {
        // Treat this as a fatal error.
        fatalError("empty jpeg source stream.");
      }
      // warn("jpeg end-of-stream");

      // Insert a fake EOI marker.
      src->buffer[0] = (JOCTET) 0xFF;
      src->buffer[1] = (JOCTET) JPEG_EOI;
      bytes_read = 2;
    }

    // Hack to work around SWF bug: sometimes data
    // starts with FFD9FFD8, when it should be
    // FFD8FFD9!
    if (src->startOfFile && bytes_read >= 4)
    {
      if (src->buffer[0] == 0xFF
          && src->buffer[1] == 0xD9 
          && src->buffer[2] == 0xFF
          && src->buffer[3] == 0xD8)
      {
        src->buffer[1] = 0xD8;
        src->buffer[3] = 0xD9;
      }
    }

    // Expose buffer state to clients.
    src->pub.next_input_byte = src->buffer;
    src->pub.bytes_in_buffer = bytes_read;
    src->startOfFile = false;

    return TRUE;
  }

  // Called by client when it wants to advance past some
  // uninteresting data.
  static void	skipInputData(j_decompress_ptr cinfo, long num_bytes)
  {
    RWSource*	src = (RWSource*) cinfo->src;

    // According to jpeg docs, large skips are
    // infrequent.  So let's just do it the simple
    // way.
    if (num_bytes > 0) {
      while (num_bytes > (long) src->pub.bytes_in_buffer) {
        num_bytes -= (long) src->pub.bytes_in_buffer;
        fillInputBuffer(cinfo);
      }
      // Handle remainder.
      src->pub.next_input_byte += (size_t) num_bytes;
      src->pub.bytes_in_buffer -= (size_t) num_bytes;
    }
  }

  // Terminate the source.  Make sure we get deleted.
  static void termSource(j_decompress_ptr cinfo)
  {
    /*rw_source*	src = (rw_source*) cinfo->src;
			assert(src);

			// @@ it's kind of bogus to be deleting here
			// -- term_source happens at the end of
			// reading an image, but we're probably going
			// to want to init a source and use it to read
			// many images, without reallocating our
			// buffer.
			delete src;
			cinfo->src = NULL;*/
  }

  void	discardPartialBuffer()
  {
    // Discard existing bytes in our buffer.
    pub.bytes_in_buffer = 0;
    pub.next_input_byte = NULL;
  }
};
// Set up the given decompress object to read from the given
// stream.
void	setupRWSource(jpeg_decompress_struct* cinfo, File* instream)
{
  // assert(cinfo->src == NULL);
  cinfo->src = (jpeg_source_mgr*) (new RWSource(instream));
}

// Called when jpeglib has a fatal error.
void	jpeg_error_exit(j_common_ptr cinfo)
{
  (*cinfo->err->output_message) (cinfo);
  fatalError("internal error in jpeglib\n");
}

// Set up some error handlers for the jpeg lib.
static void	setup_jpeg_err(jpeg_error_mgr* jerr)
{
  // Set up defaults.
  jpeg_std_error(jerr);

  jerr->error_exit = jpeg_error_exit;
}

class InputImpl:public Input{
public:
  // State needed for input.
  struct jpeg_decompress_struct	cinfo;
  struct jpeg_error_mgr	jerr;

  bool	compressorOpened;


  enum SWF_DEFINE_BITS_JPEG2 { SWF_JPEG2 };
  enum SWF_DEFINE_BITS_JPEG2_HEADER_ONLY { SWF_JPEG2_HEADER_ONLY };

  // Constructor.  Read the header data from in, and
  // prepare to read data.
  InputImpl(File* in):compressorOpened(false)
  {
    setup_jpeg_err(&jerr);
    cinfo.err = &jerr;

    // Initialize decompression object.
    jpeg_create_decompress(&cinfo);

    setupRWSource(&cinfo, in);

    startImage();
  }

  // The SWF file format stores JPEG images with the
  // encoding tables separate from the image data.  This
  // constructor reads the encoding table only and keeps
  // them in this object.  You need to call
  // start_image() and finish_image() around any calls
  // to get_width/height/components and read_scanline.
  InputImpl(SWF_DEFINE_BITS_JPEG2_HEADER_ONLY e, File* in):compressorOpened(false)  
  {
    setup_jpeg_err(&jerr);
    cinfo.err = &jerr;

    // Initialize decompression object.
    jpeg_create_decompress(&cinfo);

    setupRWSource(&cinfo, in);

    // Read the encoding tables.
    jpeg_read_header(&cinfo, FALSE);

    // Don't start reading any image data!
    // App does that manually using start_image.
  }

  // Destructor.  Clean up our jpeg reader state.
  ~InputImpl()
  {
    finishImage();

    RWSource* src = (RWSource*) cinfo.src;
    delete src;
    cinfo.src = NULL;

    jpeg_destroy_decompress(&cinfo);
  }

  // Discard any data sitting in our input buffer.  Use
  // this before/after reading headers or partial image
  // data, to avoid screwing up future reads.
  void	discardPartialBuffer()
  {
    RWSource* src = (RWSource*) cinfo.src;

    // We only have to discard the input buffer after reading the tables.
    if (src)
    {
      src->discardPartialBuffer();
    }
  }

  // This is something you can do with "abbreviated"
  // streams; i.e. if you constructed this inputter
  // using (SWF_JPEG2_HEADER_ONLY) to just load the
  // tables, or if you called finish_image() and want to
  // load another image using the existing tables.
  void	startImage()
  {
    ASSERT(compressorOpened == false);

    // Now, read the image header.
    // hack, 202 means that found SOS, ready for start_decompress
    while (cinfo.global_state != 202)
    {
      jpeg_read_header(&cinfo, FALSE);
    }
    jpeg_start_decompress(&cinfo);
    compressorOpened = true;
  }

  void	finishImage()
  {
    if (compressorOpened)
    {
      jpeg_finish_decompress(&cinfo);
      compressorOpened = false;
    }
  }

  // Return the height of the image.  Take the data from our m_cinfo struct.
  int	getHeight() const
  {
    ASSERT(compressorOpened);
    return cinfo.output_height;
  }

  // Return the width of the image.  Take the data from our m_cinfo struct.
  int	getWidth() const
  {
    ASSERT(compressorOpened);
    return cinfo.output_width;
  }

  // Return number of components (i.e. == 3 for RGB
  // data).  The size of the data for a scanline is
  // get_width() * get_components().
  int	getComponents() const
  {
    ASSERT(compressorOpened);
    return cinfo.output_components;
  }

  // Read a scanline's worth of image data into the
  // given buffer.  The amount of data read is
  // get_width() * get_components().
  void	readScanline(unsigned char* rgbData)
  {
    ASSERT(compressorOpened);
    ASSERT(cinfo.output_scanline < cinfo.output_height);
    int	lines_read = jpeg_read_scanlines(&cinfo, &rgbData, 1);
    ASSERT(lines_read == 1);
    lines_read = lines_read;	// avoid warning in NDEBUG
  }
};

// Create and return a jpeg-input object that will read from the
// given input stream.
/*static*/ Input*	Input::create(File* in)
{
  return new InputImpl(in);
}

// Read SWF JPEG2-style header.  App needs to call
// start_image() before loading any image data.  Multiple
// images can be loaded by bracketing within
// start_image()/finish_image() pairs.
/*static*/ Input*	Input::createSWFJPEG2HeaderOnly(File* in)
{
  return new InputImpl(InputImpl::SWF_JPEG2_HEADER_ONLY, in);
}
};
