#pragma once

namespace JPEG
{
	// wrapper around jpeg_decompress_struct.
	struct Input {
		// Read header and create a jpeg input object.
		static Input*	create(File* in);

// 		// Read SWF JPEG2-style header (separate encoding
// 		// table followed by image data), and create jpeg
// 		// input object.
// 		static input*	create_swf_jpeg2(SDL_RWops* in);

		static Input*	createSWFJPEG2HeaderOnly(File* in);
		virtual ~Input(){}

		virtual void	discardPartialBuffer() = 0;
		virtual void	startImage() = 0;
		virtual void	finishImage() = 0;

		virtual int	getHeight() const = 0;
		virtual int	getWidth() const = 0;
		virtual void	readScanline(unsigned char* rgbData) = 0;
	};
};
