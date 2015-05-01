#include "display.hpp"

Display *Display::GetInstance()
{
	static Display instance;
	return &instance;
}
SDL_Surface *Display::LoadImage(const char filename[],bool trans)
{
	SDL_Surface *loadedImage=NULL;
	SDL_Surface *optimizedImage=NULL;
	loadedImage=SDL_LoadBMP(filename);
	if (loadedImage)
	{
		optimizedImage=SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
		if (trans)
		{
			Uint32 color_key=SDL_MapRGB(optimizedImage->format,0XFF,0XFF,0XFF);
			SDL_SetColorKey(optimizedImage,SDL_SRCCOLORKEY,color_key);
		}
	}
	return optimizedImage;
}
void Display::ApplySurface(SDL_Surface *src,SDL_Surface *dst,short x,short y)
{
	SDL_Rect offset={x,y};
	SDL_BlitSurface(src,NULL,dst,&offset);
}
void Display::ApplySurface(SDL_Surface *src,SDL_Surface *dst,SDL_Rect offset)
{
	SDL_BlitSurface(src,NULL,dst,&offset);
}
void Display::Init()
{
	screen=SDL_SetVideoMode(screen_width,screen_height,screen_bpp,
							SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_DOUBLEBUF);
	char file_path[256];
	for (int i=0;i<1;++i)
	{
		sprintf(file_path,"../img/bullet/%X.bmp",i);
		bullet_clips[i]=LoadImage(file_path);
	}
	for (int i=0;i<6;++i) for (int j=0;j<5;++j)
	{
		sprintf(file_path,"../img/tank/%d-%d.bmp",i,j);
		tank_clips[i][j]=LoadImage(file_path,true);
	}
	for (int i=0;i<128;++i)
	{
		sprintf(file_path,"../img/terrain/%X.bmp",i);
		terrain_clips[i]=LoadImage(file_path,true);
	}
	bar=SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA,40,4,32,0,0,0,0);
	SDL_SetAlpha(bar,SDL_SRCALPHA|SDL_RLEACCEL,bar_alpha);
}
void Display::Quit()
{
	for (int i=0;i<1;++i) SDL_FreeSurface(bullet_clips[i]);
	for (int i=0;i<6;++i) for (int j=0;j<4;++j) SDL_FreeSurface(tank_clips[i][j]);
	for (int i=0;i<128;++i) SDL_FreeSurface(terrain_clips[i]);
	SDL_FreeSurface(bar);
}
//opt 0:bottom grid 1:top grid
void Display::ShowTerrain(const Terrain *terrain,bool opt)
{
	for (int i=0;i<map_size;++i) for (int j=0;j<map_size;++j)
	{
		char grid=terrain->GetGrid(i,j);
		if ((grid&1)==opt) ApplySurface(terrain_clips[(int)grid],screen,i*20,j*20);
	}
}
void Display::Show(const Model &model)
{
	const list<Tank> &tanks=model.tanks;
	const list<Bullet> &bullets=model.bullets;
	const Terrain *const &terrain=model.terrain;
	Timer timer;
	Timer fps;
	//int frame=0;
	
	LOOP:
	{
		//Fill the screen black
		//SDL_FillRect(screen,NULL,0);
		ShowTerrain(terrain,0);
		for (list<Tank>::const_iterator i=tanks.begin();i!=tanks.end();++i)
		{
			ApplySurface(tank_clips[i->GetModel()][i->GetDir()],screen,i->GetX(),i->GetY());
		}
		for (list<Bullet>::const_iterator i=bullets.begin();i!=bullets.end();++i)
		{
			ApplySurface(bullet_clips[i->GetModel()],screen,i->GetX(),i->GetY());
		}
		ShowTerrain(terrain,1);
		//Update Screen
		SDL_Flip(screen);
		
		if (timer.GetTicks()*screen_fps<1000)
		{
			SDL_Delay(1000/screen_fps-timer.GetTicks());
		}
		/*
		if (fps.GetTicks()>=1000)
		{
			printf("fps:%.3f\n",frame*1000.0f/fps.GetTicks());
			frame=0;
			fps.Start();
		}
		* */
	}
	goto LOOP;
}