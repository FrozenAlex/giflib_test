#include <iostream>
#include "gif_lib.h"
#include "png/TinyPngOut.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <vector>

struct Color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	/* data */
};

int main(int, char **)
{
	int error;

	// Open the file
	GifFileType *gif = DGifOpenFileName("./../gifs/test.gif", &error);

	int slurpres = DGifSlurp(gif);

	std::cout << "Hello, world!\n";

	//  Get width and height
	int width = gif->SWidth;
	int height = gif->SHeight;

	Color *pixels = new Color[width * height];

	int WIDTH = width;
	int HEIGHT = height;

	// Persist data from the previous frame
	GifColorType *color;
	GifColorType *transparentColor;
	SavedImage *frame;
	ExtensionBlock *ext = nullptr;
	GifImageDesc *frameInfo;
	ColorMapObject *colorMap;
	// Graphic control ext block
	GraphicsControlBlock GCB;
	int GCBResult = GIF_ERROR;

	// gif->ImageCount
	for (int frame_id = 0; frame_id < gif->ImageCount; ++frame_id)
	{
		int x, y, j, loc;

		frame = &(gif->SavedImages[frame_id]);
		frameInfo = &(frame->ImageDesc);

		// gETTING INFO
		if (frameInfo->ColorMap)
		{
			colorMap = frameInfo->ColorMap;
		}
		else
		{
			colorMap = gif->SColorMap;
		}
		for (j = 0; j < frame->ExtensionBlockCount; ++j)
		{
			if (frame->ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE)
			{
				ext = &(frame->ExtensionBlocks[j]);
				break;
			}
		}

		// Get additional info about the frame
		if (ext != nullptr)
		{
			GCBResult = DGifExtensionToGCB(ext->ByteCount, (const GifByteType *)ext->Bytes, &GCB);
		}

		long pixelDataOffset = frameInfo->Top * width + frameInfo->Left;
		// it's easier to understand iteration from 0 -> value, than it is to understand value -> value
		for (y = 0; y < frameInfo->Height; ++y)
		{
			for (x = 0; x < frameInfo->Width; ++x)
			{
				// location within the frame (smaller than the gif)
				loc = y * frameInfo->Width + x;

				// Transparency
				if (frame->RasterBits[loc] == ext->Bytes[3] && ext->Bytes[0])
				{
					continue;
				}

				// Skip transparent pix
				if (GCB.TransparentColor > 0)
				{
					transparentColor = &colorMap->Colors[GCB.TransparentColor];
					if (GCB.TransparentColor == frame->RasterBits[loc])
					{
						continue;
					}
				}
				// 52 - x / 47 - y
				color = &colorMap->Colors[frame->RasterBits[loc]];

				// for now we just use this method to determine where to draw on the image, we will probably come across a better way though

				long locWithinFrame = x + pixelDataOffset;

				// For the first frame x should be 52
				int tx = locWithinFrame % width; // Correct!
				// For the first frame y should be 48
				int ty = locWithinFrame / width; 

				// pixelData.get(locWithinFrame) = UnityEngine::Color32(color->Red, color->Green, color->Blue, 0xff);

				
					(&pixels[locWithinFrame])->red = color->Red;
					(&pixels[locWithinFrame])->green = color->Green;
					(&pixels[locWithinFrame])->blue = color->Blue;
					std::cout << "Drew a pixel at: " << x << ":" << y << " to: "  << tx << ":" << ty << " Loc within: " << locWithinFrame <<"\n";
			

				
			}
			// update pixel offset to a new row
			pixelDataOffset = pixelDataOffset + width;
		}
	}

	// get_frame()

	try
	{
		std::ofstream out("demo-rgb.png", std::ios::binary | std::ios::trunc);
		TinyPngOut pngout(static_cast<uint32_t>(WIDTH), static_cast<uint32_t>(HEIGHT), out);
		pngout.write((std::uint8_t *)pixels, static_cast<size_t>(WIDTH * HEIGHT));
		return EXIT_SUCCESS;
	}
	catch (const char *msg)
	{
		std::cerr << msg << std::endl;
		return EXIT_FAILURE;
	}
}

/**
 * @brief Get the frame object
 *
 * @param i
 * @return Color*
 */
Color *get_frame(GifFileType *gif, int i)
{
	//  Get width and height
	int width = gif->SWidth;
	int height = gif->SHeight;
}