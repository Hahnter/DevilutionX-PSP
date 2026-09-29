/**
 * @file dx.cpp
 *
 * Implementation of functions setting up the graphics pipeline.
 */
#include "engine/dx.h"

#include <cstdint>

#include <SDL.h>

#include "controls/plrctrls.h"
#include "engine.h"
#include "options.h"
#include "utils/display.h"
#include "utils/log.hpp"
#include "utils/sdl_wrap.h"

#ifndef USE_SDL1
#include "controls/touch/renderers.h"
#endif

#ifdef __3DS__
#include <3ds.h>
#endif

namespace devilution {

int refreshDelay;
SDL_Renderer *renderer;
#ifndef USE_SDL1
SDLTextureUniquePtr texture;
#ifdef PSP
SDLTextureUniquePtr PspRightTexture;
#endif
#endif

/** Currently active palette */
SDLPaletteUniquePtr Palette;
unsigned int pal_surface_palette_version = 0;

/** 24-bit renderer texture surface */
SDLSurfaceUniquePtr RendererTextureSurface;

/** 8-bit surface that we render to */
SDL_Surface *PalSurface;
namespace {
SDLSurfaceUniquePtr PinnedPalSurface;
} // namespace

/** Whether we render directly to the screen surface, i.e. `PalSurface == GetOutputSurface()` */
bool RenderDirectlyToOutputSurface;

namespace {

bool CanRenderDirectlyToOutputSurface()
{
#ifdef USE_SDL1
#ifdef SDL1_FORCE_DIRECT_RENDER
	return true;
#else
	auto *outputSurface = GetOutputSurface();
	return ((outputSurface->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF
	    && outputSurface->w == gnScreenWidth && outputSurface->h == gnScreenHeight
	    && outputSurface->format->BitsPerPixel == 8);
#endif
#else // !USE_SDL1
	return false;
#endif
}

/**
 * @brief Limit FPS to avoid high CPU load, use when v-sync isn't available
 */
void LimitFrameRate()
{
	if (!*sgOptions.Graphics.limitFPS)
		return;
	static uint64_t frameDeadline;
	const uint64_t now = static_cast<uint64_t>(SDL_GetTicks()) * 1000;
	if (frameDeadline > now && frameDeadline - now <= static_cast<uint64_t>(refreshDelay)) {
		const uint64_t remaining = frameDeadline - now;
		SDL_Delay(static_cast<Uint32>((remaining + 999) / 1000));
	}
	frameDeadline = static_cast<uint64_t>(SDL_GetTicks()) * 1000 + refreshDelay;
}

} // namespace

void dx_init()
{
#ifndef USE_SDL1
	SDL_RaiseWindow(ghMainWnd);
	SDL_ShowWindow(ghMainWnd);
#endif

	palette_init();
	CreateBackBuffer();
	pal_surface_palette_version = 1;
}

Surface GlobalBackBuffer()
{
	return Surface(PalSurface, SDL_Rect { 0, 0, gnScreenWidth, gnScreenHeight });
}

void dx_cleanup()
{
#ifndef USE_SDL1
	if (ghMainWnd != nullptr)
		SDL_HideWindow(ghMainWnd);
#endif

	PalSurface = nullptr;
	PinnedPalSurface = nullptr;
	Palette = nullptr;
	RendererTextureSurface = nullptr;
#ifndef USE_SDL1
	texture = nullptr;
#ifdef PSP
	PspRightTexture = nullptr;
#endif
	if (*sgOptions.Graphics.upscale)
		SDL_DestroyRenderer(renderer);
#endif
	SDL_DestroyWindow(ghMainWnd);
}

void CreateBackBuffer()
{
	if (CanRenderDirectlyToOutputSurface()) {
		Log("{}", "Will render directly to the SDL output surface");
		PalSurface = GetOutputSurface();
		RenderDirectlyToOutputSurface = true;
	} else {
		PinnedPalSurface = SDLWrap::CreateRGBSurfaceWithFormat(
		    /*flags=*/0,
		    /*width=*/gnScreenWidth,
		    /*height=*/gnScreenHeight,
		    /*depth=*/8,
		    SDL_PIXELFORMAT_INDEX8);
		PalSurface = PinnedPalSurface.get();
	}

#ifndef USE_SDL1
	// In SDL2, `PalSurface` points to the global `palette`.
	if (SDL_SetSurfacePalette(PalSurface, Palette.get()) < 0)
		ErrSdl();
#else
	// In SDL1, `PalSurface` owns its palette and we must update it every
	// time the global `palette` is changed. No need to do anything here as
	// the global `palette` doesn't have any colors set yet.
#endif
}

void InitPalette()
{
	Palette = SDLWrap::AllocPalette();
}

void BltFast(SDL_Rect *srcRect, SDL_Rect *dstRect)
{
	if (RenderDirectlyToOutputSurface)
		return;
	Blit(PalSurface, srcRect, dstRect);
}

void Blit(SDL_Surface *src, SDL_Rect *srcRect, SDL_Rect *dstRect)
{
	if (HeadlessMode)
		return;

	SDL_Surface *dst = GetOutputSurface();
#ifndef USE_SDL1
	if (SDL_BlitSurface(src, srcRect, dst, dstRect) < 0)
		ErrSdl();
#else
	if (!OutputRequiresScaling()) {
		if (SDL_BlitSurface(src, srcRect, dst, dstRect) < 0)
			ErrSdl();
		return;
	}

	SDL_Rect scaledDstRect;
	if (dstRect != NULL) {
		scaledDstRect = *dstRect;
		ScaleOutputRect(&scaledDstRect);
		dstRect = &scaledDstRect;
	}

	// Same pixel format: We can call BlitScaled directly.
	if (SDLBackport_PixelFormatFormatEq(src->format, dst->format)) {
		if (SDL_BlitScaled(src, srcRect, dst, dstRect) < 0)
			ErrSdl();
		return;
	}

	// If the surface has a color key, we must stretch first and can then call BlitSurface.
	if (SDL_HasColorKey(src)) {
		SDLSurfaceUniquePtr stretched = SDLWrap::CreateRGBSurface(SDL_SWSURFACE, dstRect->w, dstRect->h, src->format->BitsPerPixel,
		    src->format->Rmask, src->format->Gmask, src->format->BitsPerPixel, src->format->Amask);
		SDL_SetColorKey(stretched.get(), SDL_SRCCOLORKEY, src->format->colorkey);
		if (src->format->palette != NULL)
			SDL_SetPalette(stretched.get(), SDL_LOGPAL, src->format->palette->colors, 0, src->format->palette->ncolors);
		SDL_Rect stretched_rect = { 0, 0, dstRect->w, dstRect->h };
		if (SDL_SoftStretch(src, srcRect, stretched.get(), &stretched_rect) < 0
		    || SDL_BlitSurface(stretched.get(), &stretched_rect, dst, dstRect) < 0) {
			ErrSdl();
		}
		return;
	}

	// A surface with a non-output pixel format but without a color key needs scaling.
	// We can convert the format and then call BlitScaled.
	SDLSurfaceUniquePtr converted = SDLWrap::ConvertSurface(src, dst->format, 0);
	if (SDL_BlitScaled(converted.get(), srcRect, dst, dstRect) < 0)
		ErrSdl();
#endif
}

void RenderPresent()
{
	if (HeadlessMode)
		return;

	SDL_Surface *surface = GetOutputSurface();

	if (!gbActive) {
		LimitFrameRate();
		return;
	}

#ifndef USE_SDL1
	if (renderer != nullptr) {
#ifdef PSP
		const auto *pixels = static_cast<const std::uint8_t *>(surface->pixels);
		const auto *rightPixels = pixels + PspFirstTextureWidth * surface->format->BytesPerPixel;
		if (SDL_UpdateTexture(texture.get(), nullptr, pixels, surface->pitch) <= -1
		    || SDL_UpdateTexture(PspRightTexture.get(), nullptr, rightPixels, surface->pitch) <= -1) {
			ErrSdl();
		}
#else
		if (SDL_UpdateTexture(texture.get(), nullptr, surface->pixels, surface->pitch) <= -1) { // pitch is 2560
			ErrSdl();
		}
#endif

		// Clear buffer to avoid artifacts in case the window was resized
		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) <= -1) { // TODO only do this if window was resized
			ErrSdl();
		}

		if (SDL_RenderClear(renderer) <= -1) {
			ErrSdl();
		}
#ifdef PSP
		// PSPDEV's SDL2 PSP renderer ignores the logical viewport offset when
		// drawing textures. Apply it here so the 4:3 image has bars on both sides.
		SDL_Rect viewport;
		SDL_RenderGetViewport(renderer, &viewport);
		const SDL_Rect leftRect = { viewport.x, viewport.y, PspFirstTextureWidth, gnScreenHeight };
		const SDL_Rect rightRect = { viewport.x + PspFirstTextureWidth, viewport.y, gnScreenWidth - PspFirstTextureWidth, gnScreenHeight };
		if (SDL_RenderCopy(renderer, texture.get(), nullptr, &leftRect) <= -1
		    || SDL_RenderCopy(renderer, PspRightTexture.get(), nullptr, &rightRect) <= -1) {
			ErrSdl();
		}
#else
		if (SDL_RenderCopy(renderer, texture.get(), nullptr, nullptr) <= -1) {
			ErrSdl();
		}
#endif
		if (ControlMode == ControlTypes::VirtualGamepad) {
			RenderVirtualGamepad(renderer);
		}
		SDL_RenderPresent(renderer);

		if (!*sgOptions.Graphics.vSync) {
			LimitFrameRate();
		}
	} else {
		if (ControlMode == ControlTypes::VirtualGamepad) {
			RenderVirtualGamepad(surface);
		}
		if (SDL_UpdateWindowSurface(ghMainWnd) <= -1) {
			ErrSdl();
		}
		LimitFrameRate();
	}
#else
	if (SDL_Flip(surface) <= -1) {
		ErrSdl();
	}
	if (RenderDirectlyToOutputSurface)
		PalSurface = GetOutputSurface();
	LimitFrameRate();
#endif
}

void PaletteGetEntries(int dwNumEntries, SDL_Color *lpEntries)
{
	for (int i = 0; i < dwNumEntries; i++) {
		lpEntries[i] = system_palette[i];
	}
}
} // namespace devilution
