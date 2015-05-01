#ifndef _DISPLAY_HPP
#define _DISPLAY_HPP

#include <cstdio>
#include <SDL/SDL.h>
#include "config.hpp"
#include "model.hpp"
#include "timer.hpp"

class Display
{
	private:
		SDL_Surface *screen;
		SDL_Surface *background;
		SDL_Surface *bar;
		SDL_Surface *bullet_clips[6];
		SDL_Surface *tank_clips[6][6];
		SDL_Surface *terrain_clips[128];
		SDL_Surface *LoadImage(const char filename[],bool trans=false);
		void ApplySurface(SDL_Surface *src,SDL_Surface *dst,short x,short y);
		void ApplySurface(SDL_Surface *src,SDL_Surface *dst,SDL_Rect offset);
		void ShowTerrain(const Terrain *terrain,bool opt);
	public:
		static Display *GetInstance();
		void Init();
		void Show(const Model &model);
		void Quit();
};

#endif